#include "make_VMagField.h"

#include <thread>
#include <unordered_map>

class Volume {
public:
  virtual ~Volume() {}
  virtual bool contains( Point const&) const = 0;
  virtual double value() const = 0;  
};

class VLeft : public Volume {
public:
  bool contains( Point const& iPoint) const override {
    return iPoint.x <0.;
  }

  double value() const override { return 1.;}
};

class VRight : public Volume {
public:
  bool contains( Point const& iPoint) const override {
    return iPoint.x >=0.;
  }

  double value() const override { return 2.;}
};

namespace { 
  thread_local int presentID{0};
  thread_local Volume const* presentVolume{nullptr};

  std::unordered_map<std::thread::id, Volume const*> mapCache;
}

class VMagField : public MagField {
public:
  VMagField() : id_{newID()} {
  }
  
  double value(Point const& ) const override;

  double valueUnorderedMap(Point const& ) const override;
    
  double value(Point const&, Cache& ) const override;
  
  Cache nominalCache() const override;
  
  int id_;
  
  static int newID() {
    static int s = 0;
    return ++s;
  }
  
  Volume const* findVolume(Point const&) const;

  VLeft left_;
  VRight right_;
};

double VMagField::value(Point const& iPoint) const {
  if(id_ != presentID) {
    presentVolume = findVolume(iPoint);
    presentID = id_;
  } else if( not presentVolume->contains(iPoint) ) {
    presentVolume = findVolume(iPoint);
  }
  return presentVolume->value();
}

double VMagField::valueUnorderedMap(Point const& iPoint) const {
  auto found = mapCache.find(std::this_thread::get_id());
  Volume const* curVolume = nullptr;
  if(found != mapCache.end()) {
    curVolume = found->second;
    if(not curVolume->contains(iPoint)) {
      curVolume = findVolume(iPoint);
      mapCache[std::this_thread::get_id()] = curVolume;
    }
  }
  else {
    curVolume = findVolume(iPoint);
    mapCache[std::this_thread::get_id()] = curVolume;
  }
  return curVolume->value();
}

double VMagField::value(Point const& iPoint, Cache& iCache) const {
  if(not static_cast<Volume const*>(iCache.value_)->contains(iPoint)) {
    iCache.value_ = findVolume(iPoint);
  }
  
  return static_cast<Volume const*>(iCache.value_)->value();
}

Volume const* VMagField::findVolume(Point const& iPoint) const {
  if(left_.contains(iPoint)) {
    return &left_;
  }

  return &right_;
};


Cache VMagField::nominalCache() const {
  return Cache(&left_);
};

std::unique_ptr<MagField> make_vmagfield() {

  return std::make_unique<VMagField>();
}
