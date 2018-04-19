// -*- c++ -*-

#pragma once

#include <Arduino.h>

#include "kaleidoglyph/Key.h"
#include <assert.h>


namespace kaleidoglyph {

__attribute__((weak)) extern
constexpr byte unshifter_type_id { 0b01000001 };


class UnshifterKey {

 private:
  uint16_t index_ : 8, type_id_ : 8;

 public:
  byte index() const { return index_; }

  UnshifterKey() : index_    (0),
                   type_id_ (unshifter_type_id) {}

  constexpr explicit
  UnshifterKey(byte index) : index_   (index),
                             type_id_ (unshifter_type_id) {}

  explicit
  UnshifterKey(Key key) : index_   (uint16_t(key)     ),
                          type_id_ (uint16_t(key) >> 8)  {
    assert(type_id_ == unshifter_type_id);
  }

  constexpr
  operator Key() const {
    return Key( index_        |
                type_id_ << 8   );
  }

  static bool verify(Key key) {
    return ((uint16_t(key) >> 8) == unshifter_type_id);
  }
};

} // namespace kaleidoglyph {
