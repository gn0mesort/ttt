# Standard C++20 Tic-Tac-Toe

This is a standard C++20 implementation of the game tic-tac-toe.

# Environment Variables

To find an acceptable storage location for its data file, this software will check the environment to determine the
user's home directory. On all systems, the environment variable `MEGATECH_TTT_HOME` is checked first. If
`MEGATECH_TTT_HOME` is set then it is assumed to contain a valid directory path. If `MEGATECH_TTT_HOME` is unset (but
not invalid) then the behavior is system dependent. On POSIX-like systems (as identified at build time), the `HOME`
environment variable is checked. On Windows, the `USERPROFILE` environment variable is checked. If `USERPROFILE` is
unset (but not invalid), then `HOMEPATH` is checked. 

Regardless of how the home directory is found it must satisfy the following tests:

1. The standard C++ runtime must be able to determine the directory's status
2. The directory must exist
3. It must actually be a directory

If any of these tests fail or the directory is not determined, a fatal error will occur.

# Files Accessed

All applications of the `ttt` library access exactly two files. First, they access the `.ttt` file under the detected
home directory. Second, they access `.~lock.ttt` under the same directory. The `.~lock.ttt` file is an empty lock file
and it is safe to delete it as long as none of the game applications are running.

Some of the tests generate their own `.ttt` and `.~lock.ttt` files in their working directory. It is safe to delete
these files after the corresponding tests finish executing.

# Data File Format

## Header

Game data files are made of a 9 byte header and an arbitrarily long body. The header contains an 8 byte magic string
that identifies the file as a valid game data file. The magic string must be exactly the bytes:

`0x89`, `0x74`, `0x74`, `0x74`, `0x0d`, `0x0a`, `0x1a`, `0x0a`

The 9th byte of the data file header is the version byte. The version byte must be interpreted as an unsigned integer
value. The version byte can never be 0. This allows for up to 255 different file body versions.

## Body Version 1

Version 1 game data files must always have the byte `0x01` in the version field of the data file header. The data file
body is exactly 8 bytes long and is made up of two fields. The first 4 bytes of the body make up an endianness check
value. When interpreted as an unsigned 32-bit integer value the endianness check field must be exactly `0xaabbccdd` or
`0xddccbbaa`. Any other value is invalid. The second 4 byte field makes up the stored game state. The game state is an
arbitrary 32-bit string that represents the game's state. All body values are stored in the system endianness.

# Building

The easiest way to build this software is to use [Meson](https://mesonbuild.com/). Building with Meson is a two step
process. First, prepare a build directory by executing:

```sh
meson setup build
```

Or

```sh
meson setup build -Dbuildtype=release
```

For an optimized build.


Next, compile and link the software by executing:

```sh
meson compile -C build
```

Meson should handle the rest. After building, if you would like to run tests, you can execute:

```sh
meson test -C build
```

Tests are only available for debug builds.

# Playing

Since C++20 lacks any standard interactive behavior, gameplay is achieved by executing several different applications.
To start a new game of tic-tac-toe run:

```sh
ttt-new-game
```

To start a multiplayer game run:

```sh
ttt-new-game multiplayer
```

In either case, a new game data file will be created and the current game state will be written to standard output. If
a game data file already exists, it will be reinitialized. Regardless of whether the game is single player or
multiplayer X always moves first. In single player games, the user is always X and the computer is always O.

During play, the current state of the game can be viewed by running:

```sh
ttt-display-game
```

This will write the current game state to standard output and immediately exit.

To take a turn, run:

```sh
ttt-take-turn <COLUMN> <ROW>
```

Where `<COLUMN>` is always an integer value between 0 and 2. Similarly, `<ROW>` is always an integer value between 0
and 2. In a single player game, after X takes a turn the computer (O) will immediately take its turn. In a multiplayer
game players will alternate taking turns. Each time `ttt-take-turn` is executed, it is assumed the correct player is
playing.

Finally, to delete an existing game data file run:

```sh
ttt-delete-game
```

This will read back the current game data file, verify that it is a valid game data file, and then delete it. If the
game data file can't be verified, no action will be taken.

