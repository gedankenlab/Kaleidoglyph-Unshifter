// -*- c++ -*-

#include <Arduino.h>

#include KALEIDOGLYPH_HARDWARE_H
#include KALEIDOGLYPH_KEYADDR_H
#include <kaleidoglyph/Key.h>
#include <kaleidoglyph/Plugin.h>
#include <kaleidoglyph/Keymap.h>
#include <kaleidoglyph/Controller.h>
#include <kaleidoglyph/cKey.h>
#include <kaleidoglyph/EventHanderResult.h>
#include <kaleidoglyph/hid/Report.h>

#include "unshifter/UnshifterKey.h"

namespace kaleidoglyph {
namespace unshifter {


// Unkey structure
struct Unkey {
  Key  lower;
  Key  upper;

  Unkey(Key base, Key alt) : lower(base),
                             upper(alt)   {}
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

  const Unkey* lookupUnkey(Key key);

};

} // namespace qukeys {
} // namespace kaleidoglyph {
