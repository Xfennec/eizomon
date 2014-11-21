Eizomon
=======

Small tool and lib to control the current mode for Eizo screens. Allow to a change to a specific mode (User, FPS, RTS, Web) in one shortcut.

Needs USB connection, but the genuine Eizo is not required.

Depends on [hidapi](https://github.com/signal11/hidapi).

This is an early WIP: 
 * windows: application for shortcuts only (CTRL+ALT+Fx)
 * Linux: only a prototype of a command line tool
 * Mac OS X: nothing
 * only for the Foris FG2421
 * no binary provided
 * shortcuts are hardcoded
 * supports only one screen
 * only mode switch and backlight on/off

The project aims to:
 * be protable (hidapi is available for Linux, Mac, and Windows)
 * provide a library, exporting simple function to control screens
 * provide a command line tool
 * provide background application (for shortcuts)
 * deal with multiple screens
 * supports more screen features (colors, volume, ...)
 * supports more Eizo screens

This tool is in a very early pre-alpha 0.0.0.1 stage and is the result of some empirical experimentations using reverse engineering from the original tool and driver. Use at your own risk, it can probably set your entire house in fire.
