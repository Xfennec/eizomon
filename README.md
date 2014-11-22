# Eizomon

Small tool and lib to control the current mode for Eizo screens (Foris FG2421 only, currently).

![eizomon, Linux and windows](http://ftp.cqfd-corp.org/eizomon_screen.jpg)

Needs USB connection, but the genuine Eizo tool is not required.

Depends on [hidapi](https://github.com/signal11/hidapi).

This is an early WIP:
 * windows: application for shortcuts only (Ctrl+Shit+Fx)
 * Linux: only a small command line tool
 * Mac OS X: nothing
 * only for the Foris FG2421
 * no binary provided
 * shortcuts are hardcoded
 * supports only one screen
 * only mode switch and backlight on/off (Linux supports volume change, too)

The project aims to:
 * be protable (hidapi is available for Linux, Mac, and Windows)
 * provide a library, exporting simple function to control screens
 * provide a command line tool
 * provide background application (for shortcuts)
 * deal with multiple screens
 * supports more screen features (image settings, ...)
 * supports more Eizo screens

This tool is in a very early pre-alpha 0.0.0.1 stage and is the result of some empirical experimentations using reverse engineering from the original tool and driver. It will send (moslty) garbage USB packets to your screen.
Use at your own risk, it can probably set your entire house in fire.

## Linux

### Features

A small command line tool to control current mode, backlight status and volume.

### Compilation

There's currently nothing clean to build the tool yet, but you'll find a few helpers for a quick test:

- get hidapi (https://github.com/signal11/hidapi)
- put it next to eizomon dir (eizomon and hidapi at the same level)
- ignore bootstrap / configure
- go to hidapi/linux/ directory
- make -f Makefile-manual
- go to eizomon/linux directory
- ./comp.sh && ./run.sh -h

### Usage

```
Usage: ./eizomon [-vqdhb] [-m mode]
  -v --version            show version
  -q --quiet              hides all messages
  -d --debug              shows all warning/error messages
  -h --help               this message
  -m --mode mode          change mode (user1, user2, user3, fps1, fps2, rts, web)
  -b --backlight on|off   turn the backlight on / off
  -V --volume 0..30       change volume
```

## Windows

### Features

A background application to change current mode and backlight status with some hardcoded shortcuts.

### Compilation

Test with Visual Studio Express 2013 for Desktop:

- get hidapi (https://github.com/signal11/hidapi)
- put it next to eizomon dir (eizomon and hidapi at the same level)
- open hidapi/windows solution
- generate hidapi
- open eizomon/windows solution
- generate
- copy hidapi/windows/xxx DLL to eizomon/windows/xxx (xxx = Debug and/or Release directories)
- run eizomon.exe (nothing is shown if it loads OK)

### Usage

Use Ctrl+Shit+Fx (Fx = function key)

![eizomon shortcuts](http://ftp.cqfd-corp.org/eizomon_keys.jpg)

- F1 = User1 mode
- F2 = User2 mode
- F3 = User3 mode
- F4 = Backlight switch (you may have to press twice, depending on startup status)
- F5 = FPS1 mode
- F6 = FPS2 mode

It's a bit light (no installer, no systray icon, no RTS/Web mode, no volume set, no startup default mode, …) but it's a very early version.

You can put a shortcut to eizomon.exe to your "Start" items.
