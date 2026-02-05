# Extreme Minesweeper

A Minesweeper clone built with SDL2 featuring a 60-second time limit, background music, and explosion animations.

## Features

- **Classic Minesweeper gameplay** - Click tiles to reveal numbers or mines
- **Time pressure** - 60-second countdown timer with color-changing display (green to red)
- **Audio** - Background music during gameplay, victory/defeat sound effects, explosion sounds
- **Visual effects** - Animated explosion when hitting a mine
- **Auto-reveal** - Clicking a zero tile automatically reveals adjacent safe tiles
- **Play Again button** - Restart without closing the application

## Screenshots

| Start | Mid-game | Victory |
|-------|----------|---------|
| ![Start](cap1.PNG) | ![Playing](cap2.PNG) | ![Win](cap4.PNG) |

![Explosion](cap3.PNG)
![Game Over](cap5.PNG)

## Downloads

### Pre-built Binaries

**Recommended for most users:**

1. Go to the [Releases page](../../releases)
2. Download the ZIP for your operating system:
   - `minesweeper-windows.zip` - Windows (includes all DLLs, no installation needed)
   - `minesweeper-macos.zip` - macOS (requires SDL2)
   - `minesweeper-linux.zip` - Linux (requires SDL2)
3. Extract and run

**Windows users:** Double-click `Play.bat` or `minesweeper.exe`

**macOS/Linux users:** You'll need SDL2 runtime libraries installed (see Building section for installation commands).

### Development Builds

The latest development builds are available from [GitHub Actions](../../actions/workflows/build-minesweeper.yml).

## Building from Source

### Prerequisites

- C compiler (gcc or clang)
- SDL2, SDL2_image, SDL2_ttf, SDL2_mixer

### Linux (Ubuntu/Debian)

```bash
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev
make
./minesweeper
```

### macOS (Homebrew)

```bash
brew install sdl2 sdl2_image sdl2_ttf sdl2_mixer
make
./minesweeper
```

### Windows (MSYS2)

```bash
pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_ttf mingw-w64-x86_64-SDL2_mixer
make
./minesweeper.exe
```

## Game Assets

| File | Description |
|------|-------------|
| `grid_panel.bmp` | Main game board background |
| `clock_panel.bmp` | Timer panel background |
| `tile.bmp` | Unrevealed tile sprite |
| `mine.bmp` | Mine sprite |
| `explosion.png` | Explosion animation sprite sheet |
| `digital-7.ttf` | Clock display font |
| `Carre.ttf` | Tile number font |
| `game_loop.wav` | Background music |
| `victory.wav` | Win sound effect |
| `defeat.wav` | Lose sound effect |
| `explosion.wav` | Mine explosion sound |

## Controls

- **Left click** - Reveal a tile
- **Play Again button** - Restart the game (appears after game over)
- **ESC** - Exit after game over

## Configuration

Grid size and mine count can be adjusted in `main.c`:

```c
#define GRID_ROWS 10
#define GRID_COLS 10
#define MINES ((int) ((5.0 / 32.0) * ((GRID_ROWS * GRID_COLS) / 1.0)))
#define MAX_TIME 60
```

## Recent Improvements

- **Fixed app freeze on game over** - The game no longer becomes unresponsive during end-game animations
- **Added Play Again button** - Easily restart without closing and reopening the app
- **Fixed memory leaks** - Textures are now properly released during gameplay
- **Improved cleanup** - All SDL resources are properly freed on exit
- **Cross-platform builds** - GitHub Actions now builds and packages for Windows, macOS, and Linux

## Original Assignment

This was created for an Intermediate C Programming course assignment to implement a classic arcade game using SDL2. The original assignment allowed choosing from games like Asteroids, Pac-Man, Space Invaders, etc. Minesweeper was chosen with the added feature of a countdown timer for increased difficulty.

## License

Educational project - freely available for learning purposes.
