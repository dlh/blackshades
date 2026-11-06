# Black Shades

David Rosen's [2002 uDevGame entry](https://www.wolfire.com/black-shades/): you
are a psychic bodyguard protecting a VIP from assassins. See `Readme` for the
original notes, controls and scoring. This repository continues the
[icculus.org port](https://www.icculus.org/blackshades/) and keeps its history.

This tree runs on current systems using SDL3, OpenAL Soft and a legacy OpenGL 2.1
context. It has been tested on macOS on Apple Silicon. The CMake build is written
to be portable, but other platforms are untested.

## Build and run

```sh
brew install cmake pkgconf sdl3 openal-soft libvorbis
cmake -S . -B build
cmake --build build -j
./build/blackshades
```

The game finds `Data/` relative to the current directory or the executable, so it
runs from anywhere.

For AddressSanitizer and UBSan, configure a separate build directory with
`-DBLACKSHADES_SANITIZE=ON`.

## Controls

| Input | Action |
| --- | --- |
| WASD | Walk |
| Shift | Run |
| Mouse | Look |
| Left click | Fire, smash, disarm or pick up, depending on stance |
| Ctrl | Crouch / zoom |
| Q | Aim / un-aim |
| R | Reload |
| E | Psychic aim |
| Z | Toggle soul release |
| Space | Dive (while running forward) |
| Esc | Menu |
| Ctrl+G | Grab / release the mouse |
| Option+Return | Toggle fullscreen |
| Cmd+Q | Quit |

## Settings

`config.txt` and `Data/Highscore` are created on first run and are untracked. The
screen size in `config.txt` sets the initial window size in points; the window is
resizable. Set "Show fps and other info" to 1 to enable the debug keys in `Readme`.

## Credits

Black Shades is by David Rosen, with music by John Graham and Carlos Camacho and
weapon models by David Drew. The icculus.org port is by relnev, theoddone33,
icculus and zakk. See `Readme` and `IF_THIS_IS_A_README_YOU_HAVE_WON` for details.

## License

The uDevGame license in `uDevGame_License` allows modification for private use but
restricts redistribution. Check it before publishing this tree or any builds.
