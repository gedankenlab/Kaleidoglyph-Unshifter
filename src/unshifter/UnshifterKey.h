// -*- c++ -*-

#pragma once

#include <Arduino.h>

#include <assert.h>
#include "kaleidoglyph/Key.h"
#include "kaleidoglyph/Key/PluginKey.h"

#if defined(UNSHIFTER_CONSTANTS_H)
#include UNSHIFTER_CONSTANTS_H
#else
namespace kaleidoglyph {
namespace unshifter {

constexpr byte key_type_id{0b000001};

}  // namespace unshifter
}  // namespace kaleidoglyph
#endif

namespace kaleidoglyph {
namespace unshifter {

typedef PluginKey<key_type_id> UnshifterKey;

}  // namespace unshifter
}  // namespace kaleidoglyph
