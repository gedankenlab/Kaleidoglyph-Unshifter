// -*- c++ -*-

#include <Arduino.h>

#include <kaleidoglyph/KeyAddr.h>
#include <kaleidoglyph/Key.h>
#include <kaleidoglyph/Plugin.h>
#include <kaleidoglyph/Keymap.h>
#include <kaleidoglyph/Controller.h>
#include <kaleidoglyph/cKey.h>
#include <kaleidoglyph/EventHandlerResult.h>
#include <kaleidoglyph/hid/keyboard.h>

#include "unshifter/UnshifterKey.h"

namespace kaleidoglyph {
namespace unshifter {


// Unkey structure
struct Unkey {
  const Key  lower_;
  const Key  upper_;

  constexpr
  Unkey(const Key base, const Key alt) : lower_(base),
                                         upper_(alt)   {}

  const Key lower() const {
    return getProgmemKey(lower_);
  }
  const Key upper() const {
    return getProgmemKey(upper_);
  }
};


class Plugin : public kaleidoglyph::Plugin {

 public:
  Plugin(const Unkey* const unkeys, const byte unkey_count)
      : unkeys_(unkeys), unkey_count_(unkey_count) {}

  EventHandlerResult onKeyEvent(KeyEvent& event);

  bool preKeyboardReport(hid::keyboard::Report& keyboard_report);

  void postKeyboardReport(KeyEvent event);

 private:
  // An array of Unkey objects
  const Unkey* const unkeys_;
  const byte         unkey_count_;

  byte shift_held_count_{0};
  bool reverse_shift_state_{false};

  const Unkey* lookupUnkey(const Key key);

};

} // namespace qukeys {
} // namespace kaleidoglyph {
