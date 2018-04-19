// -*- c++ -*-

// TODO: decide the file names
#include "unshifter/Unshifter.h"

#include <Arduino.h>

#include KALEIDOGLYPH_HARDWARE_H
#include KALEIDOGLYPH_KEYADDR_H
#include <kaleidoglyph/Key.h>
#include <kaleidoglyph/Plugin.h>
#include <kaleidoglyph/KeyswitchState.h>
#include <kaleidoglyph/KeyArray.h>
#include <kaleidoglyph/KeyswitchEvent.h>
#include <kaleidoglyph/KeyswitchState.h>
#include <kaleidoglyph/hid/Report.h>


namespace kaleidoglyph {
namespace unshifter {

// This is our best guess as to whether the pressed key was *intended* to be interpreted
// as an explicit `shift` key by the user
bool isRealShift(Key key) {
  if (KeyboardKey::verify(key)) {

    KeyboardKey keyboard_key{key};

    return keyboard_key.isRealShift();
    // if (byte modifiers = keyboard_key.keycodeModifier()) {
    //   modifiers |= keyboard_key.modifierFlags();
    //   return modifiers & KeyboardKey::mod_shift_bits;
    // }
  }

  return false;
}

// Event handler
bool Plugin::keyswitchEventHook(KeyswitchEvent& event,
                                kaleidoglyph::Plugin*& caller) {
  // If Unkeys has already processed this event:
  if (checkCaller(caller))
    return true;

  // If the key toggled on, set the value based on the "true" shift state, and if
  // necessary set the shift-reverse flag
  if (event.state.toggledOn()) {
    if (const Unkey* unptr = lookupUnkey(event.key)) {
      if (shift_held_count_ == 0) {
        event.key = unptr->lower;
      } else {
        event.key = unptr->upper;
        if (KeyboardKey::verify(event.key)) {
          KeyboardKey keyboard_key{event.key};
          if (!(keyboard_key.modifiers() & keyboard_key.mods_mask_shift))
            reverse_shift_state_ = true;
        }
      }
    }
  }

  return true;
}


// Check timeouts and send necessary key events
bool Plugin::preReportHook(hid::keyboard::Report& keyboard_report) {
  if (reverse_shift_state_) {
    // release both shifts in report
    keyboard_report.remove(cKeyboardKey::LeftShift);
    keyboard_report.remove(cKeyboardKey::RightShift);
    reverse_shift_state_ = false;
  }
  return true;
}


// Update the count of "true" shift keys held
void Plugin::postReportHook(KeyswitchEvent event) {
  // I'm a bit concerned about the possibility of the count getting out of sync here, but
  // I'm going to trust it for now, and see how it plays out. If it doesn't work, we can
  // drop this hook function and just iterate through the array in the preReportHook to
  // find real shift keys.
  if (isRealShift(event.key)) {
    if (event.state.toggledOn())
      ++shift_held_count_;
    if (event.state.toggledOff())
      --shift_held_count_;
  }
}


// Check to see if the `Key` is an Unshifter key and if so, return a pointer to the
// corresponding Unkey object.
inline
const Unkey* Plugin::lookupUnkey(Key key) {
  if (UnshifterKey::verify(key)) {
    byte unkey_index = UnshifterKey(key).index();
    if (unkey_index < unkey_count_)
      return &(unkeys_[unkey_index]);
  }
  return nullptr;
}

} // namespace unshifter {
} // namespace kaleidoglyph {
