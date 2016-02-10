#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#endif

#if defined(__linux__)
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"
#endif

#if defined(__APPLE__)
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "SDL2_mixer/SDL_mixer.h"
#include "SDL2_ttf/SDL_ttf.h"
#endif

// needed includes
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

// bullet includes
#include <vector>
#include "bullet.h"

class Player {

public:

	// player score and lives vars
	int playerScore, oldScore, playerLives, oldLives;

	// variable for what font to use
	TTF_Font *font;

	// font color var
	SDL_Color colorP1 = {0, 255, 0, 255};

	// font color var
	SDL_Color colorP2 = {0, 0, 255, 255};

	// surface for the player score and player lives
	SDL_Surface *scoreSurface, *livesSurface;

	// textures for the player score and lives
	SDL_Texture *scoreTexture, *livesTexture;

	// SDL_Rects for the player score and lives textures
	SDL_Rect scorePos, livesPos;

	// strings to hold the temp values of player lives and player score
	string tempScore, tempLives;

	// audio sound effect = CHUNK
	Mix_Chunk *laser;

	// variable to hold the list of bullets
	vector<Bullet> bulletList;

	// string to hold the path to the player's texture
	string playerPath;

	// Integer for the player's number
	int playerNum;

	// player's surface to load png  into
	SDL_Surface *surface;

	// player's texture to hold img
	SDL_Texture *texture;

	// player's SDL_Rect for the X, Y, W, & H of texture
	SDL_Rect posRect;

	// floats to hold the joystick axis (-1 r 0 or 1)
	float xDir;
	float yDir;

	// float for player speed
	float speed;

	// float for the player's position to avoid precision loss
	float pos_X, pos_Y;

	// Player's creation method using passed in values for renderer, player number, path to the texture
	// starting position X, starting position Y
	Player(SDL_Renderer *renderer, int pNum, string filePath, string audioPath, float x, float y);

	// Method to allow the player the move via joystick axis
	void OnControllerAxis(const SDL_ControllerAxisEvent event);

	// Method to allow the player to interact with the joystick buttons
	void OnControllerButton(const SDL_ControllerButtonEvent event);

	// update the player using the passed in deltaTime
	void Update(float deltaTime, SDL_Renderer *renderer);

	// draw the player main's passed in renderer
	void Draw(SDL_Renderer *renderer);

	// Player's destruction method
	~Player();

	// Update Score
	void UpdateScore(SDL_Renderer *renderer);

private:

	// create a bullet function
	void CreateBullet();
};
