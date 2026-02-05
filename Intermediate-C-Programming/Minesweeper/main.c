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

	Mix_Music *gameLoopMusic;	// Game Loop Music
	Mix_Chunk *victorySound;	// Victory Sound
	Mix_Chunk *defeatSound;		// Defeat Sound
	Mix_Chunk *explosionSound;	// Explosion Sound

	SDL_Texture *explosionAnimation;	// Explosion Animation

	int adjNum[GRID_ROWS][GRID_COLS];		// Adjacent Mines
	bool mines[GRID_ROWS][GRID_COLS];		// Mines
	bool revealed[GRID_ROWS][GRID_COLS];	// Tile Revealed

	bool gameStarted;		// Has Game Started Yet
	bool gameOver;			// Game Over Flag
	bool victory;			// Victory Flag
	bool defeat;			// Defeat Flag
	bool mineFound;			// Mine Found Flag
	int time;				// Current Game Time
	unsigned int lastTime;	// Time Last Checked
	int numRevealed;		// Number of Tiles Revealed
	int mineX;				// X Coordinate of Mine Found
	int mineY;				// Y Coordinate of Mine Found
} GameState;

void cleanupAndClose(int exitCode, GameState *state);

bool setupSDL(char *title, int width, int height, GameState *state);

bool shouldClose();

void gameLoop(GameState *state);

void initGrid(GameState *state);

void updateRevealed(GameState *state);

bool handleEvents(GameState *state);

void clearLocalZeroes(GameState *state, int i, int j);

void draw(GameState *state, bool explosionFlag);

int main(int argc, char *argv[])
{
	// Initialize GameState
	GameState state = {.window=NULL, .renderer=NULL, .buffer=NULL, .gridPanel=NULL, .clockPanel=NULL, .tile=NULL, .mine=NULL, .clockText=NULL, .clockFont=NULL, .tileFont=NULL, .gameLoopMusic=NULL, .victorySound=NULL, .defeatSound=NULL, .explosionSound=NULL, .explosionAnimation=NULL};
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
	state.lastTime = 0;
	state.numRevealed = 0;
	state.mineX = 0;
	state.mineY = 0;

	if (setupSDL("Extreme Minesweeper - by Will Buscombe", WINDOW_W, WINDOW_H, &state))
	{
		gameLoop(&state);

		while(!shouldClose())
		{
			draw(&state, false);
		}
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
	if (state->gameLoopMusic)	Mix_FreeMusic(state->gameLoopMusic);
	if (state->victorySound)	Mix_FreeChunk(state->victorySound);
	if (state->defeatSound)		Mix_FreeChunk(state->defeatSound);
	if (state->explosionSound)	Mix_FreeChunk(state->explosionSound);
	if (state->explosionAnimation)	SDL_DestroyTexture(state->explosionAnimation);
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

bool shouldClose()
{
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_KEYDOWN || event.type == SDL_QUIT)
            return true;
    }
    return false;
}

void gameLoop(GameState *state)
{
	unsigned int currentTime = 0;
	unsigned int animationStart, animationCurrent;

	initGrid(state);

	// Main Game Loop
	while (handleEvents(state) && !(state->gameOver) && !(shouldClose()))
	{
		// Check and Update Time

		// If Time has Expired
		if (state->time == 0)
			state->gameOver = true;

		if (state->gameStarted)
		{
			if (!(state->gameOver))
			{
				currentTime = SDL_GetTicks();
				if (currentTime >= (state->lastTime + 1000))
				{
					state->time -= 1;
					state->lastTime = currentTime;
				}
			}
		}

		// Draw Current Game State
		draw(state, false);

		// Check Number of Revealed Tiles
		updateRevealed(state);

		// Check if Victorious
		if (((GRID_ROWS * GRID_COLS) - state->numRevealed) == MINES)
		{
			state->gameOver = true;
			state->victory = true;
		}
	}

	// Draw for Revealed Mine
	draw(state, false);

	// Stop Game Loop Music
	Mix_HaltMusic();

	// Pause

	// Victorious
	if (state->victory)
	{
		// Play Victory Sound
		Mix_PlayChannel(-1, state->victorySound, 0);
	}
	// Defeat
	else
	{
		// Play Defeat Sound
		Mix_PlayChannel(-1, state->defeatSound, 0);
		
		// Defeat and Mine Found
		if(state->mineFound)
		{
			SDL_Delay(8000);

			// Hide Mine
			//state->mines[state->mineY][state->mineX] = false;

			// Animate Explosion in Position of Mine and Play Explosion Sound
			Mix_PlayChannel(-1, state->explosionSound, 0);

			animationStart = SDL_GetTicks();

			while (animationCurrent <= (animationStart + 350))
			{
				draw(state, true);

				animationCurrent = SDL_GetTicks();

				SDL_Delay(100);
			}

			SDL_Delay(7000);
		}
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

	// Set Timer
	state->time = MAX_TIME;
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

bool handleEvents(GameState *state)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		// Check if Game is Over
		if (state->gameOver)
			return false;

		switch (event.type)
		{
			case SDL_QUIT:
				return false;
			case SDL_MOUSEBUTTONDOWN:
			{
				// If Click Outside of Tile Grid
				if ((event.button.x < GRID_GUTTER_X) || (event.button.x > (GRID_GUTTER_X + GRID_W)) ||
				 (event.button.y < GRID_GUTTER_Y) || (event.button.y > (GRID_GUTTER_Y + GRID_H)))
					continue;

				// If First Move of Game Start Timer
				if (!(state->gameStarted))
				{
					state->gameStarted = true;
					state->lastTime = SDL_GetTicks();
					// Start Game Loop Audio
					Mix_PlayMusic(state->gameLoopMusic, -1);
				}

				int clickX = (event.button.x - GRID_GUTTER_X) / TILE_W;
				int clickY = (event.button.y - GRID_GUTTER_Y) / TILE_H;

				// Revealed Clicked Tile
				state->revealed[clickY][clickX] = true;

				// If Clicked Tile Reveals a Mine Game is Over and Player has Lost
				if (state->mines[clickY][clickX])
				{
					// Game is Over and Mine is Found
					state->gameOver = true;
					state->defeat = true;
					state->mineFound = true;

					// Coordiantes of Found Mine
					state->mineX = clickX;
					state->mineY = clickY;
				}
				else
					if (state->adjNum[clickY][clickX] == 0)
						clearLocalZeroes(state, clickY, clickX);
			}
		}
	}
	return true;
}

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

	char strTime[3];

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

	// Draw to the screen
	SDL_RenderPresent(state->renderer);
}