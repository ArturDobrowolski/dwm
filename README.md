# Artur's build of dwm
[dynamic window manager](https://git.suckless.org/dwm.git) with several touch-ups.

## Features
Following patches were applied:
1. [Statuscmd](https://dwm.suckless.org/patches/statuscmd/dwmblocks-statuscmd-20210402-96cbb45.diff)
2. [Bar height](https://dwm.suckless.org/patches/bar_height/dwm-bar-height-6.2.diff)
3. [Cool autostart](https://dwm.suckless.org/patches/cool_autostart/dwm-cool-autostart-6.2.diff)
4. [Cycle layouts](https://dwm.suckless.org/patches/cyclelayouts/dwm-cyclelayouts-20180524-6.2.diff)
5. [Hide vacant tags](https://dwm.suckless.org/patches/hide_vacant_tags/dwm-hide_vacant_tags-6.3.diff)
6. [Pertag](https://dwm.suckless.org/patches/pertag/dwm-pertag-20200914-61bb8b2.diff)
7. [Sticky](https://dwm.suckless.org/patches/sticky/dwm-sticky-6.1.diff)
8. [Swallow](https://dwm.suckless.org/patches/swallow/dwm-swallow-20201211-61bb8b2.diff)
9. [Warp](https://dwm.suckless.org/patches/warp/dwm-warp-6.2.diff)
10. [Xrdb](https://dwm.suckless.org/patches/xrdb/dwm-xrdb-6.2.diff)
11. [Vanity gaps](https://dwm.suckless.org/patches/vanitygaps/dwm-vanitygaps-20200610-f09418b.diff)

This builds allows the use of color emoji in dwmbar.

Best use with my [dwmblocks](https://github.com/arturdobrowolski/dwmblocks.git) build and my [scriptset](https://github.com/arturdobrowolski/scripts.git).

## Key bindings
This build does not use any external program to handle key shortcuts, therefore it is best to inspect the `config.h` file for the list of all available keybindings.

## Installation
To install dwm simply run:
```
make clean install
```

