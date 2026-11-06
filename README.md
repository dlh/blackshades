# Black Shades

David Rosen's [2002 uDevGame entry](https://www.wolfire.com/black-shades/): you
are a psychic bodyguard protecting a VIP from assassins. See `Readme` for the
original notes, controls and scoring. This repository continues the
[icculus.org port](https://www.icculus.org/blackshades/) and keeps its history.

This port uses SDL3, OpenAL Soft and OpenGL 2.1. It is tested on macOS (Apple
Silicon) and Steam Deck; other platforms are untested.

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

## Steam Deck

The Deck build compiles in Valve's Steam Runtime SDK container on any machine with
Docker, and bundles the libraries SteamOS doesn't guarantee:

```sh
packaging/steamdeck/build.sh
```

To install `dist/blackshades-steamdeck.tar.gz`:

1. Unpack it somewhere writable on the Deck; the game saves `config.txt` and its
   high score next to `Data/`:
   `mkdir -p ~/Games && tar xzf blackshades-steamdeck.tar.gz -C ~/Games`
2. In Desktop Mode, choose Steam > Games > Add a Non-Steam Game to My Library and
   add `~/Games/blackshades/blackshades`.

Launched from Steam on a Deck, the game runs fullscreen. Use a gamepad controller
layout for the controls below; keyboard and mouse layouts also work.

## Controls

| Keyboard and mouse | Gamepad | Action |
| --- | --- | --- |
| WASD | Left stick | Walk |
| Shift | L1 or L3 (hold) | Run |
| Mouse | Right stick | Look |
| Left click | R2 | Fire, smash, disarm or pick up, depending on stance |
| Ctrl | B (hold) | Crouch / zoom |
| Q | L2 | Aim / un-aim |
| R | X | Reload |
| E | R1 | Psychic aim |
| Z | Y | Toggle soul release |
| Space | A | Dive (while running forward) |
| Esc | Start | Menu |
| Mouse, left click | D-pad or left stick, A or R2 | Choose a menu item |
| Ctrl+G | | Grab / release the mouse |
| Option+Return | | Toggle fullscreen |
| Cmd+Q | | Quit |

Gamepad buttons are named by position on an Xbox layout, which matches the Deck.

## Settings

`config.txt` and `Data/Highscore` are created on first run and are untracked. The
screen size in `config.txt` sets the initial window size in points; the window is
resizable. Set "Show fps and other info" to 1 to enable the debug keys in `Readme`.

## Credits

Black Shades is by David Rosen, with music by John Graham and Carlos Camacho and
weapon models by David Drew. The icculus.org port is by relnev, theoddone33,
icculus and zakk. See `Readme` and `IF_THIS_IS_A_README_YOU_HAVE_WON` for details.

## License

GNU General Public License version 3 or later
([source via Wikipedia](https://en.wikipedia.org/wiki/Black_Shades)); see `COPYING`.
The original 2002 uDevGame license is kept in `uDevGame_License` for reference.
