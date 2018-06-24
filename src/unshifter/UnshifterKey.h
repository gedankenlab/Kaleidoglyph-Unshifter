// -*- c++ -*-

#pragma once

#include <Arduino.h>

#include "kaleidoglyph/Key.h"
#include <assert.h>


#if defined(UNSHIFTER_CONSTANTS_H)
#include UNSHIFTER_CONSTANTS_H
#else
namespace kaleidoglyph {
namespace unshifter {

constexpr byte type_id_bits {8};
constexpr byte index_bits {8};

constexpr byte unshifter_type_id { 0b01000001 };

}
}
#endif

namespace kaleidoglyph {
namespace unshifter {


class UnshifterKey {

 private:
  uint16_t index_ : index_bits, type_id_ : type_id_bits;

 public:
  byte index() const { return index_; }

  UnshifterKey() : index_    (0),
                   type_id_ (unshifter_type_id) {}

  constexpr explicit
  UnshifterKey(byte index) : index_   (index),
                             type_id_ (unshifter_type_id) {}

  explicit
  UnshifterKey(Key key) : index_   (uint16_t(key)              ),
                          type_id_ (uint16_t(key) >> index_bits)  {
    assert(type_id_ == unshifter_type_id);
  }

  constexpr
  operator Key() const {
    return Key( index_                 |
                type_id_ << index_bits   );
  }

  static bool verify(Key key) {
    return ((uint16_t(key) >> index_bits) == unshifter_type_id);
  }
};

}
} // namespace kaleidoglyph {
