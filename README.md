# NC1020: A 文曲星 Simulator, adapted for ClockworkPi DevTerm.

Back then we are 星迷 (Stargazers I would now translate). 20 years later, we are still dreaming the 6502 dream.
This port is adapted from https://github.com/Wang-Yue/NC1020 which is optimized for another Clockwork device, the GameShell.

## Build instructions:
- Usual stuff. `build-essential` or `base-devel`.
- Install `sdl2`, `sdl2_image`
- make
- run `./nc1020`

## Key bindings
- Backquote is the power key.
- LCTRL for HELP
- LALT or 0 for SYM
- F1~F4 are mapped to DevTerm upper right keys `[ ] - =`
- F5~F11 are WQX function keys:
  - F5 = Dictionary
  - F6 = Address Book
  - F7 = Compute
  - F8 = Scheduler
  - F9 = Games
  - F10 = Main
  - F11 = NET

## TODO
- Map out LCD column 0 special display elements. Currently done: NUM, ENG, CAPS, SHIFT, DIGITS
- Periodic checkpoints
- RCTRL menu for save/load/quit etc.
- Desktop integration
- Virtual serial for data exchange
