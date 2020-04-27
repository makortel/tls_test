#if !defined(MAGFIELD_H)
#define MAGFIELD_H
struct Point {
  double x = 0;
  double y = 0;
  double z = 0;
};

struct Cache {
Cache(void const* iValue) : value_{iValue} {}

  void const* value_;
};

class MagField {
 public:
  virtual ~MagField() {}
  virtual double value(Point const& ) const = 0;

  virtual double valueUnorderedMap(Point const& ) const = 0;
  virtual double valueUnorderedMapLock(Point const& ) const = 0;

  virtual double value(Point const&, Cache& ) const = 0;

  virtual Cache nominalCache() const = 0;
};

#endif
