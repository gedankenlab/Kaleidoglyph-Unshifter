# Kaleidoglyph-Unshifter
Kaleidoglyph plugin for assigning arbitrary shifted and un-shifted symbols to any key

There are already two core Kaleidoglyph plugins that allow users to
change the effect of the `shift` modifier:
[ShapeShifter](https://github.com/keyboardio/Kaleidoglyph-ShapeShifter)
allows rearranging the effective keymap of the shifted "layer", but
doesn't make it possible to map a shifted symbol to a key on the
unshifted "layer".
[TopsyTurvy](https://github.com/keyboardio/Kaleidoglyph-TopsyTurvy)
allows a key to be "flipped", such that when it's pressed it toggles
the `shift` modifier, resulting in an "upside-down" key.  Neither of
these plugins allows mapping two shifted (or un-shifted) symbols to
the same key, thus limiting the possible re-arrangement of the keymap.
A further limitation of both plugins is that they are based on key
_codes_ sent to the host, rather than _keys_ on the keyboard, so it's
not possible to have two keys with the same symbol, but only one of
them "upside-down" or "shape-shifted".

Kaleidoglyph-Unshifter aims to treat the shift key like a momentary
layer, such that any two symbols can share the same key (one shifted,
one not), and any symbol can be mapped to either the shifted or
unshifted "layers". The mapping will be based on the key positions
(row, col) rather than mapped symbols, allowing each key to contain
any two arbitrary keycodes (though I may have to exclude modifiers for
the sake of sanity).

## Limitations

There is one unavoidable limitation, which is HID reports sent to the
host.  The report contains information about the modifiers currently
pressed, and a list of pressed keycodes.  On a standard US QWERTY
keyboard, for instance, it is not possible to send both the keycode
for an (unmodified) `8` in the same report as a (modified) `&`: both
keys would have the same `shift` state.  So in the case of overlap, it
will be necessary to suppress keys that want the "wrong" modifier
state, and remove the keycodes for those keys from the report until
there are no more keys held with the "correct" modifier state.

Since that's not very clear, here's a realistic example:

Suppose you want a key that's mapped to `,` (unshifted) and `;`
(shifted), and a second key that's mapped to `.` (unshifted) and `:`
(shifted).  If both of those keys were pressed at the same time,
without holding `shift`, there's no problem, but if `shift` is held,
we can't send both `;` and `:` to the host, because those two symbols
have the same keycode.

We also want to look out for overlap that would produce two output
symbols. For example, take the previous `,`/`;` key: suppose we're
trying to type `;`, but release `shift` just a few scan cycles before
releasing the `,`/`;` key.  Ordinarily, releasing `shift` early like
that doesn't result in an extra character, because the host limits
repeats, but Unshifter will have to suppress that key until it's
released to avoid getting a spurious `,` after the desired `;`.

## Problems

There's one big problem with this (currently hypothetical) plugin: it
cannot be done without internal changes to Kaleidoglyph (and probably
lower-level modules), because there's no way to remove a key from the
HID report during the loop hook pass before the HID report is sent to
the host.
