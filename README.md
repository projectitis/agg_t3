# ANTI-GRAIN GEOMETRY for Teensy

Based on Anti-Grain Geometry 2.4, which is copyright (C) 2002-2005 Maxim Shemanarev (McSeem)

For the original library, examples and docs, visit:
http://agg.sourceforge.net/antigrain.com/index.html

## Teensy 3.6 and above
agg_t3 is a minimal adaptation of the original source to enable use of the library on the Teensy_3.6 (and possibly other microcontrollers).

At this time a project compiled using agg_t3 on Arduino IDE requires around 110kb of program space and 8kb of dynamic memory usage.

For more information about Teensy 3.6, visit:
https://www.pjrc.com/teensy/

## Changes to original source
- Use Arduino [u]int_[8/16/32/64]_t style typedefs (agg_config.h)
- Explicit casts of some templated functions to determine between double and float
- Removed gamma LUT to free ~135kb of dynamic memory (agg_gamma_lut.h)

## LICENCE
Like the original library, this adaptation is licenced under a modified BSD licence.
See LICENCE.txt for full licence text.
