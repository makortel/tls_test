#if !defined(MAKE_VMAGFIELD_H)
#define MAKE_VMAGFIELD_H
#include "MagField.h"
#include <memory>

std::unique_ptr<MagField> make_vmagfield();
#endif
