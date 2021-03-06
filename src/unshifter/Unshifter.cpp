// -*- c++ -*-

// TODO: decide the file names
#include "unshifter/Unshifter.h"

#include <Arduino.h>

#include <kaleidoglyph/Key.h>
#include <kaleidoglyph/KeyAddr.h>
#include <kaleidoglyph/KeyArray.h>
#include <kaleidoglyph/KeyEvent.h>
#include <kaleidoglyph/KeyState.h>
#include <kaleidoglyph/Plugin.h>
#include <kaleidoglyph/hid/keyboard.h>


namespace kaleidoglyph {
namespace unshifter {

// This is our best guess as to whether the pressed key was *intended* to be interpreted
// as an explicit `shift` key by the user
bool isRealShift(Key key) {
  if (KeyboardKey::verifyType(key)) {

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
EventHandlerResult Plugin::onKeyEvent(KeyEvent& event) {

  // If the key toggled on, set the value based on the "true" shift state, and if
  // necessary set the shift-reverse flag
  if (event.state.toggledOn()) {
    if (const Unkey* unptr = lookupUnkey(event.key)) {
      // An Unkey was just pressed
      if (shift_held_count_ == 0) {
        event.key = unptr->lower();
      } else {
        event.key = unptr->upper();
        if (KeyboardKey::verifyType(event.key)) {
          KeyboardKey keyboard_key{event.key};
          // If there's no shift modifier attached to this key, record that the applied
          // shift from real modifiers should be removed before sending the report. Also,
          // record which key is responsible for undoing the shift.
          if (!(keyboard_key.modifiers() & KeyboardKey::mods_mask_shift)) {
            unkey_addr_ = event.addr;
            reverse_shift_state_ = true;
          } else {
            reverse_shift_state_ = false;
          }
        }
      }
    } else {
      // Some other key was pressed, possibly while an Unkey is being held. We assume the
      // OS will mask the Unkey for us, so we don't bother asking the controller to do
      // it. But since it's masked, we unset its other state. We could first test to see
      // if there's actually an Unkey being held (i.e. unkey_addr_.isValid()), but there's
      // almost nothing to be gained.
      unkey_addr_ = cKeyAddr::invalid;
      reverse_shift_state_ = false;
    }
  } else {
    if (unkey_addr_.isValid()) {
      if (event.addr == unkey_addr_) {
        // The current UnKey has been released, so we clear it.
        unkey_addr_ = cKeyAddr::invalid;
        reverse_shift_state_ = false;
      }
    }
  }

  return EventHandlerResult::proceed;
}


// Check timeouts and send necessary key events
bool Plugin::preKeyboardReport(hid::keyboard::Report& keyboard_report) {
  if (reverse_shift_state_) {
    // release both shifts in report
    keyboard_report.removeModifiers(cKeyboardKey::LeftShift.keycodeModifier() |
                                    cKeyboardKey::RightShift.keycodeModifier());
  }
  return true;
}


// Update the count of "real" shift keys held
void Plugin::postKeyboardReport(KeyEvent event) {
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
  if (UnshifterKey::verifyType(key)) {
    byte unkey_index = UnshifterKey(key).data();
    if (unkey_index < unkey_count_)
      return &(unkeys_[unkey_index]);
  }
  return nullptr;
}

} // namespace unshifter {
} // namespace kaleidoglyph {
