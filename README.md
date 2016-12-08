ST7735 for AVR
==============

![example of working display](./images/example.jpg)

My simplified plain C version of https://github.com/adafruit/Adafruit-ST7735-Library
and parts of https://github.com/adafruit/Adafruit-GFX-Library/

*Use at your own risk!*

Tweaks
------

* Uses my own bitmap format for fullcolor and monochrome bitmaps
* Faster line drawing based on: https://github.com/adafruit/Adafruit-GFX-Library/pull/36
* Faster font rendering based on https://github.com/adafruit/Adafruit-GFX-Library/issues/69
* Refactored draw_char function that does not load glyph struct twice
