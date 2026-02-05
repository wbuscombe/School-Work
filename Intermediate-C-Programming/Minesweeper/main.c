#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define GRID_ROWS 10
#define GRID_COLS 10
#define MINES ((int) ((5.0 / 32.0) * ((GRID_ROWS * GRID_COLS) / 1.0)))

#define MAX_TIME 60

// Window Dimensions
#define WINDOW_W 1280
#define WINDOW_H 720

// Tile Grid Dimensions
#define GRID_PANEL_W (WINDOW_W * (13.0 / 16.0))
#define GRID_PANEL_H (WINDOW_H / 1.0)

#define GRID_W (GRID_PANEL_W * (8.0 / 13.0))
#define GRID_H (GRID_PANEL_H * (8.0 / 9.0))

#define GRID_GUTTER_X ((GRID_PANEL_W * (5.0 / 13.0)) / 2.0)
#define GRID_GUTTER_Y ((GRID_PANEL_H * (1.0 / 9.0)) / 2.0)

// Tile Dimensions
#define TILE_W (GRID_W / (GRID_ROWS / 1.0))
#define TILE_H (GRID_H / (GRID_COLS / 1.0))

// Clock Dimensions
#define CLOCK_PANEL_W (WINDOW_W * (3.0 / 16.0))
#define CLOCK_PANEL_H (WINDOW_H / 1.0)

#define CLOCK_W (CLOCK_PANEL_W * (2.0 / 3.0))
#define CLOCK_H (CLOCK_PANEL_H * (2.0 / 9.0))

#define CLOCK_GUTTER_X ((CLOCK_PANEL_W * (1.0 / 3.0)) / 2.0)
#define CLOCK_GUTTER_Y ((CLOCK_PANEL_H * (7.0 / 9.0)) / 2.0)

// Side Panel Button Dimensions (NEW GAME, RESET, EXIT)
#define BUTTON_W (CLOCK_PANEL_W * 0.75)
#define BUTTON_H (CLOCK_H * 0.35)
#define BUTTON_X (GRID_PANEL_W + (CLOCK_PANEL_W - BUTTON_W) / 2)
#define BUTTON_SPACING 10
#define NEWGAME_BTN_Y (CLOCK_GUTTER_Y + CLOCK_H + 30)
#define RESET_BTN_Y (NEWGAME_BTN_Y + BUTTON_H + BUTTON_SPACING)
#define EXIT_BTN_Y (RESET_BTN_Y + BUTTON_H + BUTTON_SPACING)

// Menu Button Dimensions (centered on grid panel)
#define MENU_BTN_W 300
#define MENU_BTN_H 60
#define MENU_BTN_X ((GRID_PANEL_W - MENU_BTN_W) / 2)
#define MENU_BTN_SPACING 20

// Popup Dimensions
#define POPUP_W 500
#define POPUP_H 300
#define POPUP_X ((GRID_PANEL_W - POPUP_W) / 2)
#define POPUP_Y ((GRID_PANEL_H - POPUP_H) / 2)

// Difficulty time settings (in seconds)
#define TIME_EASY 120
#define TIME_MEDIUM 90
#define TIME_HARD 60
#define TIME_EXTREME 30

// Screen states
typedef enum {
	SCREEN_MAIN_MENU,
	SCREEN_DIFFICULTY,
	SCREEN_PLAYING,
	SCREEN_EXTREME_PLUS_PROMPT,
	SCREEN_CONGRATURATION
} ScreenState;

// Difficulty levels
typedef enum {
	DIFF_EASY,
	DIFF_MEDIUM,
	DIFF_HARD,
	DIFF_EXTREME,
	DIFF_EXTREME_PLUS
} Difficulty;

// Button hover states for side panel
typedef enum {
	HOVER_NONE,
	HOVER_NEWGAME,
	HOVER_RESET,
	HOVER_EXIT
} SidePanelHover;

// Button hover states for menus
typedef enum {
	MENU_HOVER_NONE,
	MENU_HOVER_NEWGAME,
	MENU_HOVER_EASY,
	MENU_HOVER_MEDIUM,
	MENU_HOVER_HARD,
	MENU_HOVER_EXTREME,
	MENU_HOVER_YES,
	MENU_HOVER_NO,
	MENU_HOVER_CONTINUE
} MenuHover;

typedef struct GameState
{
	SDL_Window *window;
	SDL_Renderer *renderer;

	SDL_Texture *buffer;
	SDL_Texture *gridPanel;		// Grid Panel Background Image
	SDL_Texture *clockPanel;	// Clock Panel Background Image
	SDL_Texture *tile;			// Tile Image
	SDL_Texture *mine;			// Mine Image

	SDL_Texture *clockText;		// Clock Text
	TTF_Font *clockFont;		// Clock Font

	SDL_Texture *tileText[GRID_ROWS][GRID_COLS];		// Tile Text
	TTF_Font *tileFont;									// Tile Font
	TTF_Font *buttonFont;								// Button Font (supports letters)
	TTF_Font *smallButtonFont;							// Smaller button font for longer text
	TTF_Font *menuFont;									// Larger font for menu titles

	Mix_Music *gameLoopMusic;	// Game Loop Music
	Mix_Chunk *victorySound;	// Victory Sound
	Mix_Chunk *defeatSound;		// Defeat Sound
	Mix_Chunk *explosionSound;	// Explosion Sound

	SDL_Texture *explosionAnimation;	// Explosion Animation
	SDL_Texture *buttonText;			// Button Text (reused)

	int adjNum[GRID_ROWS][GRID_COLS];		// Adjacent Mines
	bool mines[GRID_ROWS][GRID_COLS];		// Mines
	bool revealed[GRID_ROWS][GRID_COLS];	// Tile Revealed

	bool gameStarted;		// Has Game Started Yet
	bool gameOver;			// Game Over Flag
	bool victory;			// Victory Flag
	bool defeat;			// Defeat Flag
	bool mineFound;			// Mine Found Flag
	int time;				// Current Game Time
	int baseTime;			// Time to reset to (depends on difficulty)
	unsigned int lastTime;	// Time Last Checked
	int numRevealed;		// Number of Tiles Revealed
	int mineX;				// X Coordinate of Mine Found
	int mineY;				// Y Coordinate of Mine Found

	// Non-blocking end-game animation state
	int endGamePhase;				// 0=playing, 1=defeat sound, 2=explosion, 3=done, 4=game over screen
	unsigned int endGameStartTime;	// Timestamp when end-game phase started
	bool explosionSoundPlayed;		// Has explosion sound been played

	// UI state
	ScreenState screenState;		// Current screen
	Difficulty difficulty;			// Current difficulty
	int extremePlusTime;			// Current EXTREME+ time limit
	SidePanelHover sidePanelHover;	// Which side panel button is hovered
	MenuHover menuHover;			// Which menu button is hovered
} GameState;

void cleanupAndClose(int exitCode, GameState *state);

bool setupSDL(char *title, int width, int height, GameState *state);

void gameLoop(GameState *state);

void initGrid(GameState *state);

void resetGame(GameState *state);

void startNewGame(GameState *state, Difficulty diff);

void updateRevealed(GameState *state);

void clearLocalZeroes(GameState *state, int i, int j);

void draw(GameState *state, bool explosionFlag);

void drawSidePanelButtons(GameState *state);

void drawMainMenu(GameState *state);

void drawDifficultyMenu(GameState *state);

void drawExtremePlusPrompt(GameState *state);

void drawCongraturation(GameState *state);

void drawMenuButton(GameState *state, int x, int y, int w, int h, const char *text, bool hovered);

int getTimeForDifficulty(Difficulty diff, int extremePlusTime);

int main(int argc, char *argv[])
{
	// Initialize GameState
	GameState state = {.window=NULL, .renderer=NULL, .buffer=NULL, .gridPanel=NULL, .clockPanel=NULL, .tile=NULL, .mine=NULL, .clockText=NULL, .clockFont=NULL, .tileFont=NULL, .buttonFont=NULL, .smallButtonFont=NULL, .menuFont=NULL, .gameLoopMusic=NULL, .victorySound=NULL, .defeatSound=NULL, .explosionSound=NULL, .explosionAnimation=NULL, .buttonText=NULL};
	memset(state.tileText, 0, sizeof(state.tileText[0][0]) * GRID_ROWS * GRID_COLS);
	memset(state.adjNum, 0, sizeof(state.adjNum[0][0]) * GRID_ROWS * GRID_COLS);
	memset(state.mines, false, sizeof(state.mines[0][0]) * GRID_ROWS * GRID_COLS);
	memset(state.revealed, false, sizeof(state.revealed[0][0]) * GRID_ROWS * GRID_COLS);
	state.gameStarted = false;
	state.gameOver = false;
	state.victory = false;
	state.defeat = false;
	state.mineFound = false;
	state.time = MAX_TIME;
	state.baseTime = MAX_TIME;
	state.lastTime = 0;
	state.numRevealed = 0;
	state.mineX = 0;
	state.mineY = 0;
	state.endGamePhase = 0;
	state.endGameStartTime = 0;
	state.explosionSoundPlayed = false;

	// UI state - start at main menu
	state.screenState = SCREEN_MAIN_MENU;
	state.difficulty = DIFF_HARD;  // Default
	state.extremePlusTime = TIME_EXTREME;
	state.sidePanelHover = HOVER_NONE;
	state.menuHover = MENU_HOVER_NONE;

	if (setupSDL("Extreme Minesweeper - by Will Buscombe", WINDOW_W, WINDOW_H, &state))
	{
		gameLoop(&state);
	}

	cleanupAndClose(EXIT_SUCCESS, &state);
	return EXIT_SUCCESS;
}

void cleanupAndClose(int exitCode, GameState *state)
{
	if (state->window)		SDL_DestroyWindow(state->window);
	if (state->renderer)	SDL_DestroyRenderer(state->renderer);
	if (state->buffer)		SDL_DestroyTexture(state->buffer);
	if (state->gridPanel)	SDL_DestroyTexture(state->gridPanel);
	if (state->clockPanel)	SDL_DestroyTexture(state->clockPanel);
	if (state->tile)		SDL_DestroyTexture(state->tile);
	if (state->mine)		SDL_DestroyTexture(state->mine);
	if (state->clockText)	SDL_DestroyTexture(state->clockText);
	if (state->clockFont)	TTF_CloseFont(state->clockFont);
	for (int i = 0; i < GRID_ROWS; ++i)
		for (int j = 0; j < GRID_COLS; ++j)
			if (state->tileText[i][j])
				SDL_DestroyTexture(state->tileText[i][j]);
	if (state->tileFont)	TTF_CloseFont(state->tileFont);
	if (state->buttonFont)	TTF_CloseFont(state->buttonFont);
	if (state->smallButtonFont)	TTF_CloseFont(state->smallButtonFont);
	if (state->menuFont)	TTF_CloseFont(state->menuFont);
	if (state->gameLoopMusic)	Mix_FreeMusic(state->gameLoopMusic);
	if (state->victorySound)	Mix_FreeChunk(state->victorySound);
	if (state->defeatSound)		Mix_FreeChunk(state->defeatSound);
	if (state->explosionSound)	Mix_FreeChunk(state->explosionSound);
	if (state->explosionAnimation)	SDL_DestroyTexture(state->explosionAnimation);
	if (state->buttonText)	SDL_DestroyTexture(state->buttonText);
	// FIX: Added missing cleanup calls for SDL_image and SDL_mixer audio device.
	// Previously, IMG_Init() at line 331 and Mix_OpenAudio() at line 177 were called
	// but their corresponding cleanup functions were never invoked, leaking resources.
	Mix_CloseAudio();
	IMG_Quit();
	if (SDL_WasInit(0))		SDL_Quit();
	if (TTF_WasInit())		TTF_Quit();
	if (Mix_Init(0))		Mix_Quit();
	exit(exitCode);
}

bool setupSDL(char *title, int width, int height, GameState *state)
{
	if(SDL_Init(SDL_INIT_VIDEO) || TTF_Init() || SDL_Init(SDL_INIT_AUDIO))
		return false;

	if(!(state->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN)))
        return false;

    if(!(state->renderer = SDL_CreateRenderer(state->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)))
        return false;

    if(!(state->buffer = SDL_CreateTexture(state->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height)))
        return false;

    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    	return false;

    // Get file paths for images and load images into memory and then load images onto the GPU

    // Clock Panel
    char *filename = "clock_panel.bmp";
    char *basePath = SDL_GetBasePath();
    if (!basePath)		return false;
    char clockPath[strlen(filename) + strlen(basePath) + 1];
    strcpy(clockPath, basePath);
    SDL_free(basePath);
    strcat(clockPath, filename);

    SDL_Surface *imageSurface = SDL_LoadBMP(clockPath);
    if(!imageSurface) 	return false;

    state->clockPanel = SDL_CreateTextureFromSurface(state->renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    if (!(state->clockPanel))
    	return false;

    // Grid Panel
    filename = "grid_panel.bmp";
    basePath = SDL_GetBasePath();
    if (!basePath)		return false;
    char gridPath[strlen(filename) + strlen(basePath) + 1];
    strcpy(gridPath, basePath);
    SDL_free(basePath);
    strcat(gridPath, filename);

    imageSurface = SDL_LoadBMP(gridPath);
    if (!imageSurface)	return false;

    state->gridPanel = SDL_CreateTextureFromSurface(state->renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    if (!(state->gridPanel))
    	return false;

    // Mine Image
    filename = "mine.bmp";
    basePath = SDL_GetBasePath();
    if (!basePath)		return false;
    char minePath[strlen(filename) + strlen(basePath) + 1];
    strcpy(minePath, basePath);
    SDL_free(basePath);
    strcat(minePath, filename);

    imageSurface = SDL_LoadBMP(minePath);
    if (!imageSurface)		return false;
    SDL_SetColorKey(imageSurface, SDL_TRUE, SDL_MapRGB(imageSurface->format, 0, 0, 0));

    state->mine = SDL_CreateTextureFromSurface(state->renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    if (!(state->mine))
    	return false;

    // Tile Image
    filename = "tile.bmp";
    basePath = SDL_GetBasePath();
    if (!basePath)		return false;
    char tilePath[strlen(filename) + strlen(basePath) + 1];
    strcpy(tilePath, basePath);
    SDL_free(basePath);
    strcat(tilePath, filename);

    imageSurface = SDL_LoadBMP(tilePath);
    if (!imageSurface)		return false;
    SDL_SetColorKey(imageSurface, SDL_TRUE, SDL_MapRGB(imageSurface->format, 0, 0, 0));

    state->tile = SDL_CreateTextureFromSurface(state->renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    if (!(state->tile))
    	return false;

    // Clock Font
    filename = "digital-7.ttf";
    basePath = SDL_GetBasePath();
    if (!basePath)		return false;
    char clockFontPath[strlen(filename) + strlen(basePath) + 1];
    strcpy(clockFontPath, basePath);
    SDL_free(basePath);
    strcat(clockFontPath, filename);

    state->clockFont = TTF_OpenFont(clockFontPath, CLOCK_H);
    if (!(state->clockFont))
       	return false;

    // Tile Font
    filename = "Carre.ttf";
    basePath = SDL_GetBasePath();
    if (!basePath)		return false;
    char tileFontPath[strlen(filename) + strlen(basePath) + 1];
    strcpy(tileFontPath, basePath);
    SDL_free(basePath);
    strcat(tileFontPath, filename);

    state->tileFont = TTF_OpenFont(tileFontPath, TILE_H);
    if (!(state->tileFont))
       	return false;

    // Button Font - use system font that supports letters
    // Try common system font paths for different platforms
    state->buttonFont = TTF_OpenFont("/System/Library/Fonts/Helvetica.ttc", BUTTON_H * 0.6);
    if (!(state->buttonFont))
        state->buttonFont = TTF_OpenFont("/Library/Fonts/Arial.ttf", BUTTON_H * 0.6);
    if (!(state->buttonFont))
        state->buttonFont = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", BUTTON_H * 0.6);
    if (!(state->buttonFont))
        state->buttonFont = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", BUTTON_H * 0.6);
    if (!(state->buttonFont))
        // Fallback to tile font if no system font found (will show boxes but won't crash)
        state->buttonFont = state->tileFont;

    // Small Button Font - for longer button text like "NEW GAME"
    state->smallButtonFont = TTF_OpenFont("/System/Library/Fonts/Helvetica.ttc", BUTTON_H * 0.45);
    if (!(state->smallButtonFont))
        state->smallButtonFont = TTF_OpenFont("/Library/Fonts/Arial.ttf", BUTTON_H * 0.45);
    if (!(state->smallButtonFont))
        state->smallButtonFont = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", BUTTON_H * 0.45);
    if (!(state->smallButtonFont))
        state->smallButtonFont = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", BUTTON_H * 0.45);
    if (!(state->smallButtonFont))
        state->smallButtonFont = state->buttonFont;

    // Menu Font - larger font for menu buttons
    state->menuFont = TTF_OpenFont("/System/Library/Fonts/Helvetica.ttc", MENU_BTN_H * 0.5);
    if (!(state->menuFont))
        state->menuFont = TTF_OpenFont("/Library/Fonts/Arial.ttf", MENU_BTN_H * 0.5);
    if (!(state->menuFont))
        state->menuFont = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", MENU_BTN_H * 0.5);
    if (!(state->menuFont))
        state->menuFont = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", MENU_BTN_H * 0.5);
    if (!(state->menuFont))
        state->menuFont = state->buttonFont;

    // Game Loop Music
    filename = "game_loop.wav";
    basePath = SDL_GetBasePath();
    if (!basePath)		return false;
    char gameLoopMusicPath[strlen(filename) + strlen(basePath) + 1];
    strcpy(gameLoopMusicPath, basePath);
    SDL_free(basePath);
    strcat(gameLoopMusicPath, filename);

    state->gameLoopMusic = Mix_LoadMUS(gameLoopMusicPath);
    if (!(state->gameLoopMusic))
       	return false;

    // Victory Sound
    filename = "victory.wav";
    basePath = SDL_GetBasePath();
    if (!basePath)		return false;
    char victorySoundPath[strlen(filename) + strlen(basePath) + 1];
    strcpy(victorySoundPath, basePath);
    SDL_free(basePath);
    strcat(victorySoundPath, filename);

    state->victorySound = Mix_LoadWAV(victorySoundPath);
    if (!(state->victorySound))
       	return false;

    // Defeat Sound
    filename = "defeat.wav";
    basePath = SDL_GetBasePath();
    if (!basePath)		return false;
    char defeatSoundPath[strlen(filename) + strlen(basePath) + 1];
    strcpy(defeatSoundPath, basePath);
    SDL_free(basePath);
    strcat(defeatSoundPath, filename);

    state->defeatSound = Mix_LoadWAV(defeatSoundPath);
    if (!(state->defeatSound))
       	return false;

    // Explosion Sound
    filename = "explosion.wav";
    basePath = SDL_GetBasePath();
    if (!basePath)		return false;
    char explosionSoundPath[strlen(filename) + strlen(basePath) + 1];
    strcpy(explosionSoundPath, basePath);
    SDL_free(basePath);
    strcat(explosionSoundPath, filename);

    state->explosionSound = Mix_LoadWAV(explosionSoundPath);
    if (!(state->explosionSound))
       	return false;

    // Explosion Animation
    IMG_Init(IMG_INIT_PNG);
    filename = "explosion.png";
    basePath = SDL_GetBasePath();
    if (!basePath)		return false;
    char explosionAnimPath[strlen(filename) + strlen(basePath) + 1];
    strcpy(explosionAnimPath, basePath);
    SDL_free(basePath);
    strcat(explosionAnimPath, filename);

    imageSurface = IMG_Load(explosionAnimPath);
    if (!imageSurface)		return false;

    state->explosionAnimation = SDL_CreateTextureFromSurface(state->renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    if (!(state->explosionAnimation))
    	return false;

    return true;
}

void gameLoop(GameState *state)
{
	unsigned int currentTime = 0;
	bool running = true;

	while (running)
	{
		SDL_Event event;

		// Handle all pending events
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
				break;
			}

			int mx = 0, my = 0;
			if (event.type == SDL_MOUSEMOTION)
			{
				mx = event.motion.x;
				my = event.motion.y;
			}

			// Handle different screens
			switch (state->screenState)
			{
				case SCREEN_MAIN_MENU:
				{
					// Hover detection for NEW GAME button
					if (event.type == SDL_MOUSEMOTION)
					{
						int btnY = (GRID_PANEL_H - MENU_BTN_H) / 2;
						if (mx >= MENU_BTN_X && mx <= MENU_BTN_X + MENU_BTN_W &&
						    my >= btnY && my <= btnY + MENU_BTN_H)
							state->menuHover = MENU_HOVER_NEWGAME;
						else
							state->menuHover = MENU_HOVER_NONE;
					}

					// Click detection
					if (event.type == SDL_MOUSEBUTTONDOWN)
					{
						int btnY = (GRID_PANEL_H - MENU_BTN_H) / 2;
						if (event.button.x >= MENU_BTN_X && event.button.x <= MENU_BTN_X + MENU_BTN_W &&
						    event.button.y >= btnY && event.button.y <= btnY + MENU_BTN_H)
						{
							state->screenState = SCREEN_DIFFICULTY;
							state->menuHover = MENU_HOVER_NONE;
						}
					}
					break;
				}

				case SCREEN_DIFFICULTY:
				{
					// Calculate button positions (centered, stacked vertically)
					int startY = (GRID_PANEL_H - (4 * MENU_BTN_H + 3 * MENU_BTN_SPACING)) / 2;
					int easyY = startY;
					int mediumY = startY + MENU_BTN_H + MENU_BTN_SPACING;
					int hardY = startY + 2 * (MENU_BTN_H + MENU_BTN_SPACING);
					int extremeY = startY + 3 * (MENU_BTN_H + MENU_BTN_SPACING);

					// Hover detection
					if (event.type == SDL_MOUSEMOTION)
					{
						state->menuHover = MENU_HOVER_NONE;
						if (mx >= MENU_BTN_X && mx <= MENU_BTN_X + MENU_BTN_W)
						{
							if (my >= easyY && my <= easyY + MENU_BTN_H)
								state->menuHover = MENU_HOVER_EASY;
							else if (my >= mediumY && my <= mediumY + MENU_BTN_H)
								state->menuHover = MENU_HOVER_MEDIUM;
							else if (my >= hardY && my <= hardY + MENU_BTN_H)
								state->menuHover = MENU_HOVER_HARD;
							else if (my >= extremeY && my <= extremeY + MENU_BTN_H)
								state->menuHover = MENU_HOVER_EXTREME;
						}
					}

					// Click detection
					if (event.type == SDL_MOUSEBUTTONDOWN)
					{
						int clickX = event.button.x;
						int clickY = event.button.y;
						if (clickX >= MENU_BTN_X && clickX <= MENU_BTN_X + MENU_BTN_W)
						{
							if (clickY >= easyY && clickY <= easyY + MENU_BTN_H)
								startNewGame(state, DIFF_EASY);
							else if (clickY >= mediumY && clickY <= mediumY + MENU_BTN_H)
								startNewGame(state, DIFF_MEDIUM);
							else if (clickY >= hardY && clickY <= hardY + MENU_BTN_H)
								startNewGame(state, DIFF_HARD);
							else if (clickY >= extremeY && clickY <= extremeY + MENU_BTN_H)
								startNewGame(state, DIFF_EXTREME);
						}
					}
					break;
				}

				case SCREEN_PLAYING:
				{
					// Side panel button hover detection
					if (event.type == SDL_MOUSEMOTION)
					{
						state->sidePanelHover = HOVER_NONE;
						if (mx >= BUTTON_X && mx <= BUTTON_X + BUTTON_W)
						{
							if (my >= NEWGAME_BTN_Y && my <= NEWGAME_BTN_Y + BUTTON_H)
								state->sidePanelHover = HOVER_NEWGAME;
							else if (my >= RESET_BTN_Y && my <= RESET_BTN_Y + BUTTON_H)
								state->sidePanelHover = HOVER_RESET;
							else if (my >= EXIT_BTN_Y && my <= EXIT_BTN_Y + BUTTON_H)
								state->sidePanelHover = HOVER_EXIT;
						}
					}

					// Click detection
					if (event.type == SDL_MOUSEBUTTONDOWN)
					{
						int clickX = event.button.x;
						int clickY = event.button.y;

						// Side panel buttons
						if (clickX >= BUTTON_X && clickX <= BUTTON_X + BUTTON_W)
						{
							if (clickY >= NEWGAME_BTN_Y && clickY <= NEWGAME_BTN_Y + BUTTON_H)
							{
								Mix_HaltMusic();
								Mix_HaltChannel(-1);
								state->screenState = SCREEN_MAIN_MENU;
								state->menuHover = MENU_HOVER_NONE;
								continue;
							}
							else if (clickY >= RESET_BTN_Y && clickY <= RESET_BTN_Y + BUTTON_H)
							{
								resetGame(state);
								continue;
							}
							else if (clickY >= EXIT_BTN_Y && clickY <= EXIT_BTN_Y + BUTTON_H)
							{
								running = false;
								break;
							}
						}

						// Tile clicks during active gameplay
						if (state->endGamePhase == 0 && !state->gameOver)
						{
							if ((clickX >= GRID_GUTTER_X) && (clickX <= (GRID_GUTTER_X + GRID_W)) &&
							    (clickY >= GRID_GUTTER_Y) && (clickY <= (GRID_GUTTER_Y + GRID_H)))
							{
								// Start timer on first click
								if (!(state->gameStarted))
								{
									state->gameStarted = true;
									state->lastTime = SDL_GetTicks();
									Mix_PlayMusic(state->gameLoopMusic, -1);
								}

								int tileX = (clickX - GRID_GUTTER_X) / TILE_W;
								int tileY = (clickY - GRID_GUTTER_Y) / TILE_H;

								state->revealed[tileY][tileX] = true;

								if (state->mines[tileY][tileX])
								{
									state->gameOver = true;
									state->defeat = true;
									state->mineFound = true;
									state->mineX = tileX;
									state->mineY = tileY;
								}
								else if (state->adjNum[tileY][tileX] == 0)
								{
									clearLocalZeroes(state, tileY, tileX);
								}
							}
						}
					}

					// ESC to exit
					if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
					{
						running = false;
						break;
					}
					break;
				}

				case SCREEN_EXTREME_PLUS_PROMPT:
				{
					// YES/NO button positions
					int btnW = 120;
					int btnH = 50;
					int spacing = 40;
					int yesX = POPUP_X + (POPUP_W / 2) - btnW - (spacing / 2);
					int noX = POPUP_X + (POPUP_W / 2) + (spacing / 2);
					int btnY = POPUP_Y + POPUP_H - btnH - 30;

					// Hover detection
					if (event.type == SDL_MOUSEMOTION)
					{
						state->menuHover = MENU_HOVER_NONE;
						if (my >= btnY && my <= btnY + btnH)
						{
							if (mx >= yesX && mx <= yesX + btnW)
								state->menuHover = MENU_HOVER_YES;
							else if (mx >= noX && mx <= noX + btnW)
								state->menuHover = MENU_HOVER_NO;
						}
					}

					// Click detection
					if (event.type == SDL_MOUSEBUTTONDOWN)
					{
						int clickX = event.button.x;
						int clickY = event.button.y;
						if (clickY >= btnY && clickY <= btnY + btnH)
						{
							if (clickX >= yesX && clickX <= yesX + btnW)
							{
								// Accept EXTREME+ challenge
								// Calculate new time
								if (state->extremePlusTime > 20)
									state->extremePlusTime -= 5;
								else
									state->extremePlusTime -= 2;

								// Check for final victory (would go to 0 or below)
								if (state->extremePlusTime <= 0)
								{
									state->screenState = SCREEN_CONGRATURATION;
									state->menuHover = MENU_HOVER_NONE;
								}
								else
								{
									startNewGame(state, DIFF_EXTREME_PLUS);
								}
							}
							else if (clickX >= noX && clickX <= noX + btnW)
							{
								// Decline - go to main menu
								state->screenState = SCREEN_MAIN_MENU;
								state->menuHover = MENU_HOVER_NONE;
								state->extremePlusTime = TIME_EXTREME;  // Reset for next time
							}
						}
					}
					break;
				}

				case SCREEN_CONGRATURATION:
				{
					// Continue button
					int btnW = 150;
					int btnH = 50;
					int btnX = POPUP_X + (POPUP_W - btnW) / 2;
					int btnY = POPUP_Y + POPUP_H - btnH - 20;

					// Hover detection
					if (event.type == SDL_MOUSEMOTION)
					{
						if (mx >= btnX && mx <= btnX + btnW &&
						    my >= btnY && my <= btnY + btnH)
							state->menuHover = MENU_HOVER_CONTINUE;
						else
							state->menuHover = MENU_HOVER_NONE;
					}

					// Click detection
					if (event.type == SDL_MOUSEBUTTONDOWN)
					{
						if (event.button.x >= btnX && event.button.x <= btnX + btnW &&
						    event.button.y >= btnY && event.button.y <= btnY + btnH)
						{
							state->screenState = SCREEN_MAIN_MENU;
							state->menuHover = MENU_HOVER_NONE;
							state->extremePlusTime = TIME_EXTREME;  // Reset for next time
						}
					}
					break;
				}
			}
		}

		if (!running) break;

		// Game logic for SCREEN_PLAYING
		if (state->screenState == SCREEN_PLAYING)
		{
			// Active gameplay logic (phase 0)
			if (state->endGamePhase == 0 && !state->gameOver)
			{
				// Timer logic
				if (state->time == 0)
				{
					state->gameOver = true;
					state->defeat = true;
				}

				if (state->gameStarted && !state->gameOver)
				{
					currentTime = SDL_GetTicks();
					if (currentTime >= (state->lastTime + 1000))
					{
						state->time -= 1;
						state->lastTime = currentTime;
					}
				}

				// Check Number of Revealed Tiles
				updateRevealed(state);

				// Check if Victorious
				if (((GRID_ROWS * GRID_COLS) - state->numRevealed) == MINES)
				{
					state->gameOver = true;
					state->victory = true;
				}
			}

			// Handle transition to end-game phases
			if (state->gameOver && state->endGamePhase == 0)
			{
				Mix_HaltMusic();

				if (state->victory)
				{
					Mix_PlayChannel(-1, state->victorySound, 0);
					// Check if EXTREME mode victory - prompt for EXTREME+
					if (state->difficulty == DIFF_EXTREME || state->difficulty == DIFF_EXTREME_PLUS)
					{
						state->endGamePhase = 4;
						state->endGameStartTime = SDL_GetTicks();
					}
					else
					{
						state->endGamePhase = 4;
					}
				}
				else
				{
					Mix_PlayChannel(-1, state->defeatSound, 0);
					state->endGamePhase = 1;
					state->endGameStartTime = SDL_GetTicks();
				}
			}

			// Phase 1: Wait 8 seconds after defeat sound
			if (state->endGamePhase == 1)
			{
				if (SDL_GetTicks() >= state->endGameStartTime + 8000)
				{
					if (state->mineFound)
					{
						Mix_PlayChannel(-1, state->explosionSound, 0);
						state->endGamePhase = 2;
						state->endGameStartTime = SDL_GetTicks();
					}
					else
					{
						state->endGamePhase = 4;
					}
				}
			}

			// Phase 2: Explosion animation (350ms)
			if (state->endGamePhase == 2)
			{
				if (SDL_GetTicks() >= state->endGameStartTime + 350)
				{
					state->endGamePhase = 3;
					state->endGameStartTime = SDL_GetTicks();
				}
			}

			// Phase 3: Wait 7 seconds after explosion
			if (state->endGamePhase == 3)
			{
				if (SDL_GetTicks() >= state->endGameStartTime + 7000)
				{
					state->endGamePhase = 4;
				}
			}

			// Phase 4: Game over - check for EXTREME+ prompt after a delay
			if (state->endGamePhase == 4 && state->victory)
			{
				if (state->difficulty == DIFF_EXTREME || state->difficulty == DIFF_EXTREME_PLUS)
				{
					// Show EXTREME+ prompt after short delay
					if (SDL_GetTicks() >= state->endGameStartTime + 2000)
					{
						state->screenState = SCREEN_EXTREME_PLUS_PROMPT;
						state->menuHover = MENU_HOVER_NONE;
					}
				}
			}
		}

		// Draw based on current screen
		switch (state->screenState)
		{
			case SCREEN_MAIN_MENU:
				drawMainMenu(state);
				break;
			case SCREEN_DIFFICULTY:
				drawDifficultyMenu(state);
				break;
			case SCREEN_PLAYING:
			{
				bool showExplosion = (state->endGamePhase == 2);
				draw(state, showExplosion);
				break;
			}
			case SCREEN_EXTREME_PLUS_PROMPT:
				draw(state, false);  // Show game in background
				drawExtremePlusPrompt(state);
				break;
			case SCREEN_CONGRATURATION:
				draw(state, false);  // Show game in background
				drawCongraturation(state);
				break;
		}

		SDL_Delay(1);
	}

	return;
}

void initGrid(GameState *state)
{
	srand((unsigned int) time(NULL));

	// Randomly Select Positions of Mines
	for (int i = 0; i < (int) MINES; ++i)
	{
		int num = rand() % (GRID_ROWS * GRID_COLS) - 1;

		int mineRow = num / GRID_ROWS;
		int mineCol = num % GRID_COLS;

		state->mines[mineRow][mineCol] = true;
	}

	// Set Appropriate Adjacency Numbers for Each Tile
	for (int i = 0; i < GRID_ROWS; ++i)
	{
		for (int j = 0; j < GRID_COLS; ++j)
		{
			if (state->mines[i][j])
			{
				if (i > 0)
					state->adjNum[i - 1][j]++;

				if ((i > 0)  && (j < GRID_COLS - 1))
					state->adjNum[i - 1][j + 1]++;

				if (j < GRID_COLS - 1)
					state->adjNum[i][j + 1]++;

				if ((i < GRID_ROWS - 1) && (j < GRID_COLS - 1))
					state->adjNum[i + 1][j + 1]++;

				if (i < GRID_ROWS - 1)
					state->adjNum[i + 1][j]++;

				if ((i < GRID_ROWS - 1) && (j > 0))
					state->adjNum[i + 1][j - 1]++;

				if (j > 0)
					state->adjNum[i][j - 1]++;

				if ((i > 0) && (j > 0))
					state->adjNum[i - 1][j - 1]++;
			}
		}
	}

	// Set Timer based on current difficulty
	state->time = state->baseTime;
}

void resetGame(GameState *state)
{
	// Stop any playing music/sounds for a fresh start
	Mix_HaltMusic();
	Mix_HaltChannel(-1);  // Stop all sound effect channels

	// Clear tile textures to prevent memory leak on reset
	for (int i = 0; i < GRID_ROWS; ++i)
	{
		for (int j = 0; j < GRID_COLS; ++j)
		{
			if (state->tileText[i][j])
			{
				SDL_DestroyTexture(state->tileText[i][j]);
				state->tileText[i][j] = NULL;
			}
		}
	}

	// Reset grid state
	memset(state->adjNum, 0, sizeof(state->adjNum[0][0]) * GRID_ROWS * GRID_COLS);
	memset(state->mines, false, sizeof(state->mines[0][0]) * GRID_ROWS * GRID_COLS);
	memset(state->revealed, false, sizeof(state->revealed[0][0]) * GRID_ROWS * GRID_COLS);

	// Reset game flags
	state->gameStarted = false;
	state->gameOver = false;
	state->victory = false;
	state->defeat = false;
	state->mineFound = false;
	state->time = state->baseTime;  // Use baseTime for current difficulty
	state->lastTime = 0;
	state->numRevealed = 0;
	state->mineX = 0;
	state->mineY = 0;

	// Reset end-game animation state
	state->endGamePhase = 0;
	state->endGameStartTime = 0;
	state->explosionSoundPlayed = false;
	state->sidePanelHover = HOVER_NONE;

	// Re-initialize the grid with new mine positions
	initGrid(state);
}

void startNewGame(GameState *state, Difficulty diff)
{
	state->difficulty = diff;
	state->baseTime = getTimeForDifficulty(diff, state->extremePlusTime);
	state->screenState = SCREEN_PLAYING;
	state->menuHover = MENU_HOVER_NONE;

	// Reset game state
	resetGame(state);
}

int getTimeForDifficulty(Difficulty diff, int extremePlusTime)
{
	switch (diff)
	{
		case DIFF_EASY:   return TIME_EASY;
		case DIFF_MEDIUM: return TIME_MEDIUM;
		case DIFF_HARD:   return TIME_HARD;
		case DIFF_EXTREME: return TIME_EXTREME;
		case DIFF_EXTREME_PLUS: return extremePlusTime;
		default: return TIME_HARD;
	}
}

void updateRevealed(GameState *state)
{
	int temp = 0;

	// Count Number of Revealed Tiles and Update Game State
	for (int i = 0; i < GRID_ROWS; ++i)
	{
		for (int j = 0; j < GRID_COLS; ++j)
		{
			if (state->revealed[i][j])
				temp++;
		}
	}

	state->numRevealed = temp;
}

// FIX: Removed handleEvents() function - logic now integrated into gameLoop()
// to support non-blocking end-game animations and Play Again button.

void clearLocalZeroes(GameState *state, int i, int j)
{
	// Reveal section

	// Reveal node
	state->revealed[i][j] = true;

	// Reveal above-left
	if ((i > 0) && (j > 0) && (state->adjNum[i - 1][j - 1] != 0) && !(state->revealed[i - 1][j - 1]))
		state->revealed[i - 1][j - 1] = true;

	// Reveal above
	if ((i > 0) && (state->adjNum[i - 1][j] != 0) && !(state->revealed[i - 1][j]))
		state->revealed[i - 1][j] = true;

	// Reveal above-right
	if ((i > 0) && (j < GRID_COLS - 1) && (state->adjNum[i - 1][j + 1] != 0) && !(state->revealed[i - 1][j + 1]))
		state->revealed[i - 1][j + 1] = true;

	// Reveal right
	if ((j < GRID_COLS - 1) && (state->adjNum[i][j + 1] != 0) && !(state->revealed[i][j + 1]))
		state->revealed[i][j + 1] = true;

	// Reveal below-right
	if ((i < GRID_ROWS - 1) && (j < GRID_COLS - 1) && (state->adjNum[i + 1][j + 1] != 0) && !(state->revealed[i + 1][j + 1]))
		state->revealed[i + 1][j + 1] = true;

	// Reveal below
	if ((i < GRID_ROWS - 1) && (state->adjNum[i + 1][j] != 0) && !(state->revealed[i + 1][j]))
		state->revealed[i + 1][j] = true;

	// Reveal below-left
	if ((i < GRID_ROWS - 1) && (j > 0) && (state->adjNum[i + 1][j - 1] != 0) && !(state->revealed[i + 1][j - 1]))
		state->revealed[i + 1][j - 1] = true;

	// Reveal left
	if ((j > 0) && (state->adjNum[i][j - 1] != 0) && !(state->revealed[i][j - 1]))
		state->revealed[i][j - 1] = true;

	// Recrusive call section

	// Check above-left
	if ((i > 0) && (j > 0) && (state->adjNum[i - 1][j - 1] == 0) && !(state->revealed[i - 1][j - 1]))
		clearLocalZeroes(state, i - 1, j - 1);

	// Check above
	if ((i > 0) && (state->adjNum[i - 1][j] == 0) && !(state->revealed[i - 1][j]))
		clearLocalZeroes(state, i - 1, j);

	// Check above-right
	if ((i > 0) && (j < GRID_COLS - 1) && (state->adjNum[i - 1][j + 1] == 0) && !(state->revealed[i - 1][j + 1]))
		clearLocalZeroes(state, i - 1, j + 1);

	// Check right
	if ((j < GRID_COLS - 1) && (state->adjNum[i][j + 1] == 0) && !(state->revealed[i][j + 1]))
		clearLocalZeroes(state, i, j + 1);

	// Check below-right
	if ((i < GRID_ROWS - 1) && (j < GRID_COLS - 1) && (state->adjNum[i + 1][j + 1] == 0) && !(state->revealed[i + 1][j + 1]))
		clearLocalZeroes(state, i + 1, j + 1);

	// Check below
	if ((i < GRID_ROWS - 1) && (state->adjNum[i + 1][j] == 0) && !(state->revealed[i + 1][j]))
		clearLocalZeroes(state, i + 1, j);

	// Check below-left
	if ((i < GRID_ROWS - 1) && (j > 0) && (state->adjNum[i + 1][j - 1] == 0) && !(state->revealed[i + 1][j - 1]))
		clearLocalZeroes(state, i + 1, j - 1);

	// Check left
	if ((j > 0) && (state->adjNum[i][j - 1] == 0) && !(state->revealed[i][j - 1]))
		clearLocalZeroes(state, i, j - 1);
}

void draw(GameState *state, bool explosionFlag)
{
	// Clear Screen
	SDL_SetRenderDrawColor(state->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(state->renderer);

	// Set the dimensions and positions for the background images
	SDL_Rect gridDest, clockDest;

	// Grid Panel Image
	gridDest.x = 0;
	gridDest.y = 0;
	gridDest.w = GRID_PANEL_W;
	gridDest.h = GRID_PANEL_H;

	// Clock Panel Image
	clockDest.x = GRID_PANEL_W;
	clockDest.y = 0;
	clockDest.w = CLOCK_PANEL_W;
	clockDest.h = CLOCK_PANEL_H;

	// Render Images
	SDL_RenderCopy(state->renderer, state->gridPanel, NULL, &gridDest);
	SDL_RenderCopy(state->renderer, state->clockPanel, NULL, &clockDest);

	// Render Grid Box
	SDL_Rect gridBox;
	gridBox.x = GRID_GUTTER_X;
	gridBox.y = GRID_GUTTER_Y;
	gridBox.w = GRID_W;
	gridBox.h = GRID_H;
	SDL_SetRenderDrawColor(state->renderer, 220, 220, 220, 0xFF);
	SDL_RenderFillRect(state->renderer, &gridBox);

	// Render Clock Box
	SDL_Rect clockBox;
	clockBox.x = (GRID_PANEL_W + CLOCK_GUTTER_X);
	clockBox.y = CLOCK_GUTTER_Y;
	clockBox.w = CLOCK_W;
	clockBox.h = CLOCK_H;
	SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 0xFF);
	SDL_RenderFillRect(state->renderer, &clockBox);

	// Render Grid
	SDL_Rect tiles[GRID_ROWS][GRID_COLS];

	for (int i = 0; i < GRID_ROWS; ++i)
	{
		for (int j = 0; j < GRID_COLS; ++j)
		{
			// Tile dimensions and position
			tiles[i][j].x = (GRID_GUTTER_X + (j * TILE_W));
			tiles[i][j].y = (GRID_GUTTER_Y + (i * TILE_H));
			tiles[i][j].w = TILE_W;
			tiles[i][j].h = TILE_H;

			if(!(state->revealed[i][j]))	// Tile not revealed
			{
				// Render Tile
				SDL_RenderCopy(state->renderer, state->tile, NULL, &tiles[i][j]);
			}
			else
			{
				if (!(state->mines[i][j]))	// Tile revealed but not a mine
				{
					// If adjacency number isn't zero
					if (state->adjNum[i][j] > 0)
					{
						// Render Adjacency Number
						SDL_Rect adjNumDest;
						adjNumDest.x = (GRID_GUTTER_X + (j * TILE_W));
						adjNumDest.y = (GRID_GUTTER_Y + (i * TILE_H));
						adjNumDest.w = TILE_W;
						adjNumDest.h = TILE_H;

						char strAdj[2];

						sprintf(strAdj, "%d", state->adjNum[i][j]);

						SDL_Color adjNumBlue = {46, 107, 241, 0xFF};

						SDL_Surface *adjNumSurface = TTF_RenderText_Blended(state->tileFont, strAdj, adjNumBlue);

						// FIX: Destroy previous texture before creating new one to prevent memory leak.
						// Previously, a new texture was created every frame without freeing the old one,
						// causing memory to grow unbounded as tiles were revealed.
						// Old code: state->tileText[i][j] = SDL_CreateTextureFromSurface(...);
						if (state->tileText[i][j]) SDL_DestroyTexture(state->tileText[i][j]);
						state->tileText[i][j] = SDL_CreateTextureFromSurface(state->renderer, adjNumSurface);

						SDL_FreeSurface(adjNumSurface);

						SDL_RenderCopy(state->renderer, state->tileText[i][j], NULL, &adjNumDest);
					}
				}
				else	// Tile revealed and a mine
				{
					// Render Mine
					SDL_RenderCopy(state->renderer, state->mine, NULL, &tiles[i][j]);
				}
			}
		}
	}

	// Render Clock
	SDL_Rect clock;
	clock.x = (GRID_PANEL_W + CLOCK_GUTTER_X);
	clock.y = CLOCK_GUTTER_Y - 7;	// Requires slight adjustment to not run into barrier of box
	clock.w = CLOCK_W;
	clock.h = CLOCK_H;

	char strTime[4];  // Support 3-digit times (up to 999)

	if (state->time < 10)
		sprintf(strTime, "0%d", state->time);
	else
		sprintf(strTime, "%d", state->time);

	SDL_Color clockRed = {255, 0, 0, 0xFF};
	SDL_Color clockGreen = {57, 255, 20, 0xFF};

	SDL_Surface *clockSurface;

	if (state->time <= 10)
		clockSurface = TTF_RenderText_Blended(state->clockFont, strTime, clockRed);
	else
		clockSurface = TTF_RenderText_Blended(state->clockFont, strTime, clockGreen);

	// FIX: Destroy previous clock texture before creating new one to prevent memory leak.
	// Previously, a new texture was created every frame (~60fps) without freeing the old one,
	// causing thousands of orphaned textures per minute of gameplay.
	// Old code: state->clockText = SDL_CreateTextureFromSurface(...);
	if (state->clockText) SDL_DestroyTexture(state->clockText);
	state->clockText = SDL_CreateTextureFromSurface(state->renderer, clockSurface);

	SDL_FreeSurface(clockSurface);

	SDL_RenderCopy(state->renderer, state->clockText, NULL, &clock);

	// If Animating Explosions
	if (explosionFlag)
	{
		SDL_Rect explosionDest, explosionSource;

		explosionSource.w = 256;
		explosionSource.h = 256;
		explosionSource.x = 256 * ((SDL_GetTicks() / 100) % 8);
		explosionSource.y = 256 * ((SDL_GetTicks() / 100) % 6);

		explosionDest.w = TILE_W;
		explosionDest.h = TILE_H;
		explosionDest.x = GRID_GUTTER_X + (TILE_W * state->mineX);
		explosionDest.y = GRID_GUTTER_Y + (TILE_H * state->mineY);
		
		SDL_RenderCopyEx(state->renderer, state->explosionAnimation, &explosionSource, &explosionDest, 0, NULL, SDL_FLIP_NONE);
	}

	// Draw side panel buttons (NEW GAME, RESET, EXIT)
	drawSidePanelButtons(state);

	// Draw to the screen
	SDL_RenderPresent(state->renderer);
}

// Helper function to draw a single button
void drawSingleButton(GameState *state, int x, int y, int w, int h, const char *text, bool hovered, TTF_Font *font)
{
	SDL_Rect buttonRect = {x, y, w, h};

	// Draw button with hover effect - grey tones
	if (hovered)
		SDL_SetRenderDrawColor(state->renderer, 160, 160, 160, 0xFF);
	else
		SDL_SetRenderDrawColor(state->renderer, 128, 128, 128, 0xFF);
	SDL_RenderFillRect(state->renderer, &buttonRect);

	// Draw border - darker grey
	SDL_SetRenderDrawColor(state->renderer, 80, 80, 80, 0xFF);
	SDL_RenderDrawRect(state->renderer, &buttonRect);

	// Render button text
	SDL_Color textColor = hovered ? (SDL_Color){0, 0, 0, 0xFF} : (SDL_Color){30, 30, 30, 0xFF};

	SDL_Surface *textSurface = TTF_RenderText_Blended(font, text, textColor);
	if (textSurface)
	{
		SDL_Texture *textTexture = SDL_CreateTextureFromSurface(state->renderer, textSurface);

		// Center text in button
		SDL_Rect textRect;
		textRect.w = textSurface->w;
		textRect.h = textSurface->h;
		textRect.x = x + (w - textRect.w) / 2;
		textRect.y = y + (h - textRect.h) / 2;

		SDL_FreeSurface(textSurface);
		SDL_RenderCopy(state->renderer, textTexture, NULL, &textRect);
		SDL_DestroyTexture(textTexture);
	}
}

void drawSidePanelButtons(GameState *state)
{
	// NEW GAME button - uses smaller font to fit text
	drawSingleButton(state, BUTTON_X, NEWGAME_BTN_Y, BUTTON_W, BUTTON_H,
	                 "NEW GAME", state->sidePanelHover == HOVER_NEWGAME, state->smallButtonFont);

	// RESET button
	drawSingleButton(state, BUTTON_X, RESET_BTN_Y, BUTTON_W, BUTTON_H,
	                 "RESET", state->sidePanelHover == HOVER_RESET, state->buttonFont);

	// EXIT button
	drawSingleButton(state, BUTTON_X, EXIT_BTN_Y, BUTTON_W, BUTTON_H,
	                 "EXIT", state->sidePanelHover == HOVER_EXIT, state->buttonFont);
}

void drawMainMenu(GameState *state)
{
	// Clear Screen
	SDL_SetRenderDrawColor(state->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(state->renderer);

	// Draw grid panel as background
	SDL_Rect gridDest = {0, 0, GRID_PANEL_W, GRID_PANEL_H};
	SDL_RenderCopy(state->renderer, state->gridPanel, NULL, &gridDest);

	// Draw clock panel
	SDL_Rect clockDest = {GRID_PANEL_W, 0, CLOCK_PANEL_W, CLOCK_PANEL_H};
	SDL_RenderCopy(state->renderer, state->clockPanel, NULL, &clockDest);

	// Draw NEW GAME button centered
	int btnY = (GRID_PANEL_H - MENU_BTN_H) / 2;
	drawSingleButton(state, MENU_BTN_X, btnY, MENU_BTN_W, MENU_BTN_H,
	                 "NEW GAME", state->menuHover == MENU_HOVER_NEWGAME, state->menuFont);

	SDL_RenderPresent(state->renderer);
}

void drawDifficultyMenu(GameState *state)
{
	// Clear Screen
	SDL_SetRenderDrawColor(state->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(state->renderer);

	// Draw grid panel as background
	SDL_Rect gridDest = {0, 0, GRID_PANEL_W, GRID_PANEL_H};
	SDL_RenderCopy(state->renderer, state->gridPanel, NULL, &gridDest);

	// Draw clock panel
	SDL_Rect clockDest = {GRID_PANEL_W, 0, CLOCK_PANEL_W, CLOCK_PANEL_H};
	SDL_RenderCopy(state->renderer, state->clockPanel, NULL, &clockDest);

	// Calculate button positions (centered, stacked vertically)
	int startY = (GRID_PANEL_H - (4 * MENU_BTN_H + 3 * MENU_BTN_SPACING)) / 2;

	// EASY button
	drawSingleButton(state, MENU_BTN_X, startY, MENU_BTN_W, MENU_BTN_H,
	                 "EASY", state->menuHover == MENU_HOVER_EASY, state->menuFont);

	// MEDIUM button
	drawSingleButton(state, MENU_BTN_X, startY + MENU_BTN_H + MENU_BTN_SPACING, MENU_BTN_W, MENU_BTN_H,
	                 "MEDIUM", state->menuHover == MENU_HOVER_MEDIUM, state->menuFont);

	// HARD button
	drawSingleButton(state, MENU_BTN_X, startY + 2 * (MENU_BTN_H + MENU_BTN_SPACING), MENU_BTN_W, MENU_BTN_H,
	                 "HARD", state->menuHover == MENU_HOVER_HARD, state->menuFont);

	// EXTREME button
	drawSingleButton(state, MENU_BTN_X, startY + 3 * (MENU_BTN_H + MENU_BTN_SPACING), MENU_BTN_W, MENU_BTN_H,
	                 "EXTREME", state->menuHover == MENU_HOVER_EXTREME, state->menuFont);

	SDL_RenderPresent(state->renderer);
}

void drawExtremePlusPrompt(GameState *state)
{
	// Draw semi-transparent overlay
	SDL_SetRenderDrawBlendMode(state->renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 180);
	SDL_Rect overlay = {0, 0, GRID_PANEL_W, GRID_PANEL_H};
	SDL_RenderFillRect(state->renderer, &overlay);

	// Draw popup box
	SDL_Rect popup = {POPUP_X, POPUP_Y, POPUP_W, POPUP_H};
	SDL_SetRenderDrawColor(state->renderer, 50, 50, 50, 255);
	SDL_RenderFillRect(state->renderer, &popup);
	SDL_SetRenderDrawColor(state->renderer, 150, 150, 150, 255);
	SDL_RenderDrawRect(state->renderer, &popup);

	// Draw title text
	SDL_Color white = {255, 255, 255, 255};
	SDL_Surface *titleSurface = TTF_RenderText_Blended(state->menuFont, "GET MORE EXTREME?", white);
	if (titleSurface)
	{
		SDL_Texture *titleTexture = SDL_CreateTextureFromSurface(state->renderer, titleSurface);
		SDL_Rect titleRect = {POPUP_X + (POPUP_W - titleSurface->w) / 2, POPUP_Y + 40, titleSurface->w, titleSurface->h};
		SDL_FreeSurface(titleSurface);
		SDL_RenderCopy(state->renderer, titleTexture, NULL, &titleRect);
		SDL_DestroyTexture(titleTexture);
	}

	// Draw current time info
	int nextTime = state->extremePlusTime;
	if (nextTime > 20)
		nextTime -= 5;
	else
		nextTime -= 2;

	char timeInfo[64];
	sprintf(timeInfo, "Next challenge: %d seconds", nextTime);
	SDL_Surface *infoSurface = TTF_RenderText_Blended(state->buttonFont, timeInfo, white);
	if (infoSurface)
	{
		SDL_Texture *infoTexture = SDL_CreateTextureFromSurface(state->renderer, infoSurface);
		SDL_Rect infoRect = {POPUP_X + (POPUP_W - infoSurface->w) / 2, POPUP_Y + 100, infoSurface->w, infoSurface->h};
		SDL_FreeSurface(infoSurface);
		SDL_RenderCopy(state->renderer, infoTexture, NULL, &infoRect);
		SDL_DestroyTexture(infoTexture);
	}

	// Draw YES/NO buttons
	int btnW = 120;
	int btnH = 50;
	int spacing = 40;
	int yesX = POPUP_X + (POPUP_W / 2) - btnW - (spacing / 2);
	int noX = POPUP_X + (POPUP_W / 2) + (spacing / 2);
	int btnY = POPUP_Y + POPUP_H - btnH - 30;

	drawSingleButton(state, yesX, btnY, btnW, btnH, "YES", state->menuHover == MENU_HOVER_YES, state->buttonFont);
	drawSingleButton(state, noX, btnY, btnW, btnH, "NO", state->menuHover == MENU_HOVER_NO, state->buttonFont);

	SDL_RenderPresent(state->renderer);
}

void drawCongraturation(GameState *state)
{
	// Draw semi-transparent overlay
	SDL_SetRenderDrawBlendMode(state->renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 200);
	SDL_Rect overlay = {0, 0, GRID_PANEL_W, GRID_PANEL_H};
	SDL_RenderFillRect(state->renderer, &overlay);

	// Draw popup box
	SDL_Rect popup = {POPUP_X, POPUP_Y, POPUP_W, POPUP_H};
	SDL_SetRenderDrawColor(state->renderer, 30, 30, 30, 255);
	SDL_RenderFillRect(state->renderer, &popup);
	SDL_SetRenderDrawColor(state->renderer, 200, 150, 50, 255);  // Gold border
	SDL_RenderDrawRect(state->renderer, &popup);

	SDL_Color gold = {255, 215, 0, 255};

	// "CONGRATURATION." - centered
	SDL_Surface *line1Surface = TTF_RenderText_Blended(state->menuFont, "CONGRATURATION.", gold);
	if (line1Surface)
	{
		SDL_Texture *line1Texture = SDL_CreateTextureFromSurface(state->renderer, line1Surface);
		SDL_Rect line1Rect = {POPUP_X + (POPUP_W - line1Surface->w) / 2, POPUP_Y + 50, line1Surface->w, line1Surface->h};
		SDL_FreeSurface(line1Surface);
		SDL_RenderCopy(state->renderer, line1Texture, NULL, &line1Rect);
		SDL_DestroyTexture(line1Texture);
	}

	// "THIS STORY IS HAPPY END." - centered
	SDL_Surface *line2Surface = TTF_RenderText_Blended(state->buttonFont, "THIS STORY IS HAPPY END.", gold);
	if (line2Surface)
	{
		SDL_Texture *line2Texture = SDL_CreateTextureFromSurface(state->renderer, line2Surface);
		SDL_Rect line2Rect = {POPUP_X + (POPUP_W - line2Surface->w) / 2, POPUP_Y + 110, line2Surface->w, line2Surface->h};
		SDL_FreeSurface(line2Surface);
		SDL_RenderCopy(state->renderer, line2Texture, NULL, &line2Rect);
		SDL_DestroyTexture(line2Texture);
	}

	// "THANK YOU." - centered
	SDL_Surface *line3Surface = TTF_RenderText_Blended(state->buttonFont, "THANK YOU.", gold);
	if (line3Surface)
	{
		SDL_Texture *line3Texture = SDL_CreateTextureFromSurface(state->renderer, line3Surface);
		SDL_Rect line3Rect = {POPUP_X + (POPUP_W - line3Surface->w) / 2, POPUP_Y + 160, line3Surface->w, line3Surface->h};
		SDL_FreeSurface(line3Surface);
		SDL_RenderCopy(state->renderer, line3Texture, NULL, &line3Rect);
		SDL_DestroyTexture(line3Texture);
	}

	// Continue button
	int btnW = 150;
	int btnH = 50;
	int btnX = POPUP_X + (POPUP_W - btnW) / 2;
	int btnY = POPUP_Y + POPUP_H - btnH - 20;

	drawSingleButton(state, btnX, btnY, btnW, btnH, "CONTINUE", state->menuHover == MENU_HOVER_CONTINUE, state->buttonFont);

	SDL_RenderPresent(state->renderer);
}

void drawMenuButton(GameState *state, int x, int y, int w, int h, const char *text, bool hovered)
{
	drawSingleButton(state, x, y, w, h, text, hovered, state->menuFont);
}