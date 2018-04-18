// -*- c++ -*-

#include <Arduino.h>

#include KALEIDOSCOPE_HARDWARE_H
#include KALEIDOSCOPE_KEYADDR_H
#include <kaleidoscope/Key/Keyboard.h>
#include <kaleidoscope/Key.h>
#include <kaleidoscope/Plugin.h>
#include <kaleidoscope/Keymap.h>
#include <kaleidoscope/Controller.h>
#include <kaleidoscope/cKey.h>
#include <kaleidoscope/hid/Report.h>

#include "unshifter/UnshifterKey.h"

namespace kaleidoscope {
namespace unshifter {


// Unkey structure
struct Unkey {
  Key  lower;
  Key  upper;

  Unkey(Key base, Key alt) : lower(base),
                             upper(alt)   {}
};


class Plugin : public kaleidoscope::Plugin {

 public:
  Plugin(const Unkey* const unkeys, const byte unkey_count)
      : unkeys_(unkeys), unkey_count_(unkey_count) {}

  bool keyswitchEventHook(KeyswitchEvent& event,
                          kaleidoscope::Plugin*& caller) override;

  bool preReportHook(hid::keyboard::Report& keyboard_report) override;

  void postReportHook(KeyswitchEvent event) override;

 private:
  // An array of Unkey objects
  const Unkey* const unkeys_;
  const byte         unkey_count_;

  byte shift_held_count_{0};
  bool reverse_shift_state_{false};

  const Unkey* lookupUnkey(Key key);

};

} // namespace qukeys {
} // namespace kaleidoscope {
