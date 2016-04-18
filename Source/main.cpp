
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

#if defined(__linux__)
#include <unistd.h>
#endif

#if defined(_WIN32) || (_WIN64)
#include <direct.h>
#define getcwd _getcwd
#endif

#include <stdio.h>
#include <iostream>
#include "tank.h"
#include "Building.h"
#include "copper.h"
using namespace std;

// CODE FOR FRAME RATE INDEPENDENCE
float deltaTime = 0.0f;
int thisTime = 0;
int lastTime = 0;

// set speed for background
int bkgdSpeed = 100;

// create the SDL_Rectangle for the texture's position and size - x,y,w,h
SDL_Rect bkgd1Pos;

// create the SDL_Rectangle for the texture's position and size - x,y,w,h
SDL_Rect bkgd2Pos;

// set temp variable to hold movement - background 1
float BG1pos_X = 0, BG1pos_Y = 0;

// set temp variable to hold movement - background 2
float BG2pos_X = 0, BG2pos_Y = -768;

// move the background
void UpdateBackground(float deltaTime)
{
	// Update

	// Update background 1
	//	BG1pos_Y += (bkgdSpeed * 1) * deltaTime;
	//
	//// set the new bkgd1 position
	//bkgd1Pos.y = (int)(BG1pos_Y + 0.5f);

	// RESET WHEN OFF THE BOTTOM OF THE SCREEN
	//	if (bkgd1Pos.y >= 768) {
	//
	//		bkgd1Pos.y = -768;

	//		BG1pos_Y = bkgd1Pos.y;
	//	}

	// Update background 2
	//	BG2pos_Y += (bkgdSpeed * 1) * deltaTime;

	// set the new bkgd2 position
	//	bkgd2Pos.y = (int)(BG2pos_Y + 0.5f);

	// RESET WHEN OFF THE BOTTOM OF THE SCREEN
	//	if (bkgd2Pos.y >= 768) {

	//		bkgd2Pos.y = -768;

	//		BG2pos_Y = bkgd2Pos.y;
	//	}
}

const int JOYSTICK_DEAD_ZONE = 8000;

// joystick direction vars
float xDir, yDir;

// cursor float vars for movement
float pos_X, pos_Y;

// create SDL Rectangle for the 2 player graphic
SDL_Rect cursorPos, activePos;

// var from cursor speed
int cursorSpeed = 300;

void moveCursor(const SDL_ControllerAxisEvent event)
{
	if (event.which == 0) {
		if (event.axis == 0) {

			if (event.value < -JOYSTICK_DEAD_ZONE) {
				xDir = -1.0f;
			}
			else if (event.value > JOYSTICK_DEAD_ZONE) {
				xDir = 1.0f;
			}
			else {
				xDir = 0.0f;
			}
		}

		if (event.axis == 1) {

			if (event.value < -JOYSTICK_DEAD_ZONE) {
				yDir = -1.0f;
			}
			else if (event.value > JOYSTICK_DEAD_ZONE) {
				yDir = 1.0f;
			}
			else {
				yDir = 0.0f;
			}
		}
	}
}

void UpdateCursor(float deltaTime) {
	// Update cursor
	pos_X += (cursorSpeed * xDir) * deltaTime;
	pos_Y += (cursorSpeed * yDir) * deltaTime;

	cursorPos.x = (int)(pos_X + .5f);
	cursorPos.y = (int)(pos_Y + .5f);

	activePos.x = cursorPos.x;
	activePos.y = cursorPos.y;

	if (cursorPos.x < 0) {
		cursorPos.x = 0;
		pos_X = cursorPos.x;
	}

	if (cursorPos.x > 1024 - cursorPos.w) {
		cursorPos.x = 1024 - cursorPos.w;
		pos_Y = cursorPos.x;
	}

	if (cursorPos.y < 0) {
		cursorPos.y = 0;
		pos_Y = cursorPos.y;
	}

	if (cursorPos.y > 768 - cursorPos.h) {
		cursorPos.y = 768 - cursorPos.h;
		pos_Y = cursorPos.y;
	}
}

// variables for all Menus button over
bool players1Over = false, players2Over = false, instructionsOver = false,
quitOver = false, menuOver = false, playOver = false;

// class header includes
//#include "player.h"
//#include "enemy.h"
#include <vector>
#include <stdlib.h>	// srand, rand
#include <time.h>	// time
#include "explode.h"

// variable to hold the list of enemies: for 1 player game - 6 total, for 2 player game - 12 total
//vector<Enemy> enemyList;
vector<Explode> explodeList;

void MakeExplosion(int x, int y)
{
	// see if there is an explosion not active to use
	for (int i = 0; i < explodeList.size(); i++)
	{
		// see if the explosion is not active
		if (explodeList[i].active == false)
		{
			// set explosion to active
			explodeList[i].active = true;

			// use some math in the x position to get the bullet close to
			// the center of the player using player width
			explodeList[i].posRect.x = x;
			explodeList[i].posRect.y = y;

			// once explosion is found, break out of loop
			break;
		}
	}
}

int main(int argc, char* argv[]) {

	// initialize random seed:
	srand(time(NULL));

#if defined(_WIN32) || (_WIN64)
	cout << "Running on Windows" << endl;
	cout << "Added on Windows" << endl;

	// Get the current working directory
	string s_cwd(getcwd(NULL, 0));

	// create a string linking to the mac's images folder
	string s_cwd_images = s_cwd + "\\Resources\\Images\\";

	// create a string linking to the window's audio folder
	string audio_dir = s_cwd + "\\Resources\\Audio\\";
#endif

#if defined(__linux__)
	cout << "Running on Linux" << endl;
	cout << "Added on Linux" << endl;

	// Get the current working directory
	string s_cwd(getcwd(NULL, 0));

	// create a string linking to the mac's images folder
	string s_cwd_images = s_cwd + "/Resources/Images/";

	// create a string linking to the linux's audio folder
	string audio_dir = s_cwd + "/Resources/Audio/";
#endif

#if defined(__APPLE__)
	cout << "Running on Apple" << endl;
	cout << "Added on Apple" << endl;

	// Get the current working directory
	string s_cwd(getcwd(NULL, 0));

	// create a string linking to the mac's images folder
	string s_cwd_images = s_cwd + "/Resources/Images/";

	// create a string to link to the audio folder on __APPLE__
	string audio_dir = s_cwd + "/Resources/Audio/";

	// test
	//cout << audio_dir << endl;
#endif


	SDL_Window *window;                    // Declare a pointer

	SDL_Init(SDL_INIT_EVERYTHING);              // Initialize SDL2

	// Create an application window with the following settings:
	window = SDL_CreateWindow("An SDL2 window",		// window title
		SDL_WINDOWPOS_UNDEFINED,           		// initial x position
		SDL_WINDOWPOS_UNDEFINED,           		// initial y position
		1024,                               	// width, in pixels
		768,                               		// height, in pixels
		SDL_WINDOW_OPENGL                  		// flags - see below
		);

	// Check that the window was successfully created
	if (window == NULL) {
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Renderer* renderer = NULL;

	// create the renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//********** Create Background **********
	string BKGD1path = s_cwd_images + "MainMenu1.png";

	string BKGD2path = s_cwd_images + "MainMenu2.png";

	string BKGD3path = s_cwd_images + "MainMenu3.png";

	string BKGD4path = s_cwd_images + "MainMenu4.png";

	string BKGD5path = s_cwd_images + "BackStory1.png";

	string BKGD6path = s_cwd_images + "BackStory2.png";

	string BKGD7path = s_cwd_images + "BackStory3.png";

	string BKGD8path = s_cwd_images + "BackStory4.png";

	// win scene background
	string BKGD9path = s_cwd_images + "WinSceneBack.png";

	// win scene background
	string BKGD10path = s_cwd_images + "LoseSceneBack.png";

	//instructions scene background
	string BKGDInstpath = s_cwd_images + "InstructionsSceneBack.png";

	string level1BKGDpath = s_cwd_images + "level1.png";

	// create a SDL texture
	SDL_Texture *bkgd1;

	// create a SD surface to hold the background image
	SDL_Surface *surface = IMG_Load(BKGD1path.c_str());

	// place surface info the texture bkgd1
	bkgd1 = SDL_CreateTextureFromSurface(renderer, surface);

	// create a SDL texture
	SDL_Texture *bkgd2;

	// create a SD surface to hold the background image
	surface = IMG_Load(BKGD2path.c_str());

	// place surface info the texture bkgd2
	bkgd2 = SDL_CreateTextureFromSurface(renderer, surface);

	// create a SDL texture
	SDL_Texture *bkgd3;

	// create a SD surface to hold the background image
	surface = IMG_Load(BKGD3path.c_str());

	// place surface info the texture bkgd2
	bkgd3 = SDL_CreateTextureFromSurface(renderer, surface);

	// create a SDL texture
	SDL_Texture *bkgd4;

	// create a SD surface to hold the background image
	surface = IMG_Load(BKGD4path.c_str());

	// place surface info the texture bkgd2
	bkgd4 = SDL_CreateTextureFromSurface(renderer, surface);

	// create a SDL texture
	SDL_Texture *bkgd5;

	// create a SD surface to hold the background image
	surface = IMG_Load(BKGD5path.c_str());

	// place surface info the texture bkgd1
	bkgd5 = SDL_CreateTextureFromSurface(renderer, surface);

	// create a SDL texture
	SDL_Texture *bkgd6;

	// create a SD surface to hold the background image
	surface = IMG_Load(BKGD6path.c_str());

	// place surface info the texture bkgd2
	bkgd6 = SDL_CreateTextureFromSurface(renderer, surface);

	// create a SDL texture
	SDL_Texture *bkgd7;

	// create a SD surface to hold the background image
	surface = IMG_Load(BKGD7path.c_str());

	// place surface info the texture bkgd2
	bkgd7 = SDL_CreateTextureFromSurface(renderer, surface);

	// create a SDL texture
	SDL_Texture *bkgd8;

	// create a SD surface to hold the background image
	surface = IMG_Load(BKGD8path.c_str());

	// place surface info the texture bkgd2
	bkgd8 = SDL_CreateTextureFromSurface(renderer, surface);

	// create a SDL texture
	SDL_Texture *bkgd9;

	// create a SD surface to hold the background image
	surface = IMG_Load(BKGD9path.c_str());

	// place surface info the texture bkgd2
	bkgd9 = SDL_CreateTextureFromSurface(renderer, surface);

	// create a SDL texture
	SDL_Texture *bkgd10;

	// create a SD surface to hold the background image
	surface = IMG_Load(BKGD10path.c_str());

	// place surface info the texture bkgd2
	bkgd10 = SDL_CreateTextureFromSurface(renderer, surface);

	// create a SDL texture
	SDL_Texture *bkgdInst;

	// create a SD surface to hold the background image
	surface = IMG_Load(BKGDInstpath.c_str());

	// place surface info the texture bkgd2
	bkgdInst = SDL_CreateTextureFromSurface(renderer, surface);

	// create a SDL texture
	SDL_Texture *level1bkgd;

	// create a SD surface to hold the background image
	surface = IMG_Load(level1BKGDpath.c_str());

	// place surface info the texture bkgd2
	level1bkgd = SDL_CreateTextureFromSurface(renderer, surface);


	// free the SDL surface
	//SDL_FreeSurface (surface);

	// set the X, Y, W, and H for the Rectangle
	bkgd1Pos.x = 0;
	bkgd1Pos.y = 0;
	bkgd1Pos.w = 1024;
	bkgd1Pos.h = 768;

	// set the X, Y, W, and H for the Rectangle
	bkgd2Pos.x = 0;
	bkgd2Pos.y = -768;
	bkgd2Pos.w = 1024;
	bkgd2Pos.h = 768;

	SDL_Rect bkgdRect;

	bkgdRect.x = 0;

	bkgdRect.y = 0;

	bkgdRect.w = 2048;

	bkgdRect.h = 1536;


	//********** Create Main Menu **********

	//********** Create Title **********

	string TITLEpath = s_cwd_images + "Title.png";

	// create a SD surface to hold the background image
	surface = IMG_Load(TITLEpath.c_str());

	// create a SDL texture
	SDL_Texture *title;

	// place surface info the texture bkgd1
	title = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the title graphic
	SDL_Rect titlePos;

	// set the X, Y, W, and H for the Rectangle
	titlePos.x = 209;
	titlePos.y = 652;
	titlePos.w = 619;
	titlePos.h = 62;

	TITLEpath = s_cwd_images + "WinText.png";

	// create a SDL texture
	SDL_Texture *winTitle;

	// create a SD surface to hold the background image
	surface = IMG_Load(TITLEpath.c_str());

	// place surface info the texture bkgd1
	winTitle = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the title graphic
	SDL_Rect winTitlePos;

	// set the X, Y, W, and H for the Rectangle
	winTitlePos.x = 73;
	winTitlePos.y = 29;
	winTitlePos.w = 880;
	winTitlePos.h = 240;

	// free the SDL surface
	//SDL_FreeSurface (surface);

	//********** End Title **********

	//********** Create 1 Player Normal **********

	string P1_Npath = s_cwd_images + "playGameO.png";

	// create a SD surface to hold the Player1 image
	surface = IMG_Load(P1_Npath.c_str());

	// create a SDL texture
	SDL_Texture *Player1_n;

	// place surface info the texture Player1
	Player1_n = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the Player1 graphic
	SDL_Rect Player1nPos;

	// set the X, Y, W, and H for the Rectangle
	Player1nPos.x = 541;
	Player1nPos.y = 305;
	Player1nPos.w = 361;
	Player1nPos.h = 25;

	//********** End 1 Player Normal **********

	//********** Create 1 Player Over **********

	string P1_Opath = s_cwd_images + "playGameN.png";

	// create a SD surface to hold the Player1 image
	surface = IMG_Load(P1_Opath.c_str());

	// create a SDL texture
	SDL_Texture *Player1_o;

	// place surface info the texture Player1
	Player1_o = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the Player1 graphic
	SDL_Rect Player1oPos;

	// set the X, Y, W, and H for the Rectangle
	Player1oPos.x = 541;
	Player1oPos.y = 305;
	Player1oPos.w = 361;
	Player1oPos.h = 25;

	//********** End 1 Player Over**********

	//********** Create Backstory Normal **********

	string P2_Npath = s_cwd_images + "BackStoryO.png";

	// create a SD surface to hold the Player2 image
	surface = IMG_Load(P2_Npath.c_str());

	// create a SDL texture
	SDL_Texture *backStoryN;

	// place surface info the texture Player2
	backStoryN = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the player2 graphic
	SDL_Rect backStorynPos;

	// set the X, Y, W, and H for the Rectangle
	backStorynPos.x = 541;
	backStorynPos.y = 416;
	backStorynPos.w = 345;
	backStorynPos.h = 25;

	//********** End Backstory Normal **********

	//********** Create BackStory Over **********

	string P2_Opath = s_cwd_images + "backStoryN.png";

	// create a SD surface to hold the player2 image
	surface = IMG_Load(P2_Opath.c_str());

	// create a SDL texture
	SDL_Texture *backStoryO;

	// place surface info the texture player2
	backStoryO = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the player2 graphic
	SDL_Rect backStoryoPos;

	// set the X, Y, W, and H for the Rectangle
	backStoryoPos.x = 541;
	backStoryoPos.y = 416;
	backStoryoPos.w = 345;
	backStoryoPos.h = 25;

	//********** End 2 Player Over**********

	//********** Create Instructions Normal **********

	string INST_Npath = s_cwd_images + "InstructionsO.png";

	// create a SD surface to hold the instructions image
	surface = IMG_Load(INST_Npath.c_str());

	// create a SDL texture
	SDL_Texture *Instructions_n;

	// place surface info the texture instructions
	Instructions_n = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the Instructions graphic
	SDL_Rect Inst_nPos;

	// set the X, Y, W, and H for the Rectangle
	Inst_nPos.x = 541;
	Inst_nPos.y = 361;
	Inst_nPos.w = 409;
	Inst_nPos.h = 21;

	//********** End Instructions Normal **********

	//********** Create Instructions Over **********

	string INST_Opath = s_cwd_images + "InstructionsN.png";

	// create a SD surface to hold the Instructions image
	surface = IMG_Load(INST_Opath.c_str());

	// create a SDL texture
	SDL_Texture *Instructions_o;

	// place surface info the texture Instructions
	Instructions_o = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the Instructions graphic
	SDL_Rect Inst_oPos;

	// set the X, Y, W, and H for the Rectangle
	Inst_oPos.x = 541;
	Inst_oPos.y = 361;
	Inst_oPos.w = 409;
	Inst_oPos.h = 21;

	//********** End Instructions Over**********

	//********** Create Quit Normal **********

	string QUIT_Npath = s_cwd_images + "quitO.png";

	// create a SD surface to hold the Quit image
	surface = IMG_Load(QUIT_Npath.c_str());

	// create a SDL texture
	SDL_Texture *Quit_n;

	// place surface info the texture Quit
	Quit_n = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the Quit graphic
	SDL_Rect Quit_nPos;

	// set the X, Y, W, and H for the Rectangle
	Quit_nPos.x = 541;
	Quit_nPos.y = 470;
	Quit_nPos.w = 145;
	Quit_nPos.h = 26;

	//********** End Quit Normal **********

	//********** Create Quit Over **********

	string QUIT_Opath = s_cwd_images + "quitN.png";

	// create a SD surface to hold the Quit image
	surface = IMG_Load(QUIT_Opath.c_str());

	// create a SDL texture
	SDL_Texture *Quit_o;

	// place surface info the texture Instructions
	Quit_o = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the Quit graphic
	SDL_Rect Quit_oPos;

	// set the X, Y, W, and H for the Rectangle
	Quit_oPos.x = 541;
	Quit_oPos.y = 470;
	Quit_oPos.w = 145;
	Quit_oPos.h = 26;

	//********** End Quit Over**********


	// ************** END MAIN MENU ****************

	//********** Create Instructions Menu - Start **********

	//********** INSTRUCTIONS GRAPHICS - START **********

	string instructionsTextPath = s_cwd_images + "InstructionsText.png";

	// create a SD surface to hold the Instructions Title image
	surface = IMG_Load(instructionsTextPath.c_str());

	// create a SDL texture
	SDL_Texture *instructionText;

	// place surface info the texture bkgd1
	instructionText = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the Instructions graphic
	SDL_Rect instructionTextPos;

	// set the X, Y, W, and H for the Rectangle
	instructionTextPos.x = 156;
	instructionTextPos.y = 132;
	instructionTextPos.w = 722;
	instructionTextPos.h = 506;

	// free the SDL surface
	//SDL_FreeSurface (surface);

	//********** INSTRUCTIONS GRAPHICS - END **********

	//********** BACKSTORY GRAPHICS - START **********

	string backStoryTextPath = s_cwd_images + "backStoryText.png";

	// create a SD surface to hold the Instructions Title image
	surface = IMG_Load(backStoryTextPath.c_str());

	// create a SDL texture
	SDL_Texture *backStoryText;

	// place surface info the texture bkgd1
	backStoryText = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the Instructions graphic
	SDL_Rect backStoryTextPos;

	// set the X, Y, W, and H for the Rectangle
	backStoryTextPos.x = 539;
	backStoryTextPos.y = 76;
	backStoryTextPos.w = 481;
	backStoryTextPos.h = 496;

	// free the SDL surface
	//SDL_FreeSurface (surface);

	//********** BACKSTORY GRAPHICS - END **********

	//********** Create Main Menu Normal **********

	string MENU_Npath = s_cwd_images + "mainMenuO.png";

	// create a SD surface to hold the menu image
	surface = IMG_Load(MENU_Npath.c_str());

	// create a SDL texture
	SDL_Texture *MainMenu_n;

	// place surface info the texture menu
	MainMenu_n = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the menu graphic
	SDL_Rect Menu_nPos;

	// set the X, Y, W, and H for the Rectangle
	Menu_nPos.x = 10;
	Menu_nPos.y = 103;
	Menu_nPos.w = 370;
	Menu_nPos.h = 21;

	MENU_Npath = s_cwd_images + "WinMainMenuO.png";

	// create a SD surface to hold the menu image
	surface = IMG_Load(MENU_Npath.c_str());

	// create a SDL texture
	SDL_Texture *WinMainMenu_n;

	// place surface info the texture menu
	WinMainMenu_n = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the menu graphic
	SDL_Rect WinMenu_nPos;

	// set the X, Y, W, and H for the Rectangle
	WinMenu_nPos.x = 30;
	WinMenu_nPos.y = 715;
	WinMenu_nPos.w = 452;
	WinMenu_nPos.h = 33;

	MENU_Npath = s_cwd_images + "LoseMainMenuO.png";

	// create a SD surface to hold the menu image
	surface = IMG_Load(MENU_Npath.c_str());

	// create a SDL texture
	SDL_Texture *LoseMainMenu_n;

	// place surface info the texture menu
	LoseMainMenu_n = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the menu graphic
	SDL_Rect LoseMenu_nPos;

	// set the X, Y, W, and H for the Rectangle
	LoseMenu_nPos.x = 19;
	LoseMenu_nPos.y = 706;
	LoseMenu_nPos.w = 474;
	LoseMenu_nPos.h = 59;

	//********** End Main Menu Normal **********

	//********** Create Main Menu Over **********

	string MENU_Opath = s_cwd_images + "mainMenuN.png";

	// create a SD surface to hold the menu image
	surface = IMG_Load(MENU_Opath.c_str());

	// create a SDL texture
	SDL_Texture *Menu_o;

	// place surface info the texture menu
	Menu_o = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the menu graphic
	SDL_Rect Menu_oPos;

	// set the X, Y, W, and H for the Rectangle
	Menu_oPos.x = 10;
	Menu_oPos.y = 103;
	Menu_oPos.w = 370;
	Menu_oPos.h = 21;

	MENU_Opath = s_cwd_images + "WinMainMenuN.png";

	// create a SD surface to hold the menu image
	surface = IMG_Load(MENU_Opath.c_str());

	// create a SDL texture
	SDL_Texture *winMenu_o;

	// place surface info the texture menu
	winMenu_o = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the menu graphic
	SDL_Rect winMenu_oPos;

	// set the X, Y, W, and H for the Rectangle
	winMenu_oPos.x = 30;
	winMenu_oPos.y = 715;
	winMenu_oPos.w = 452;
	winMenu_oPos.h = 33;

	MENU_Opath = s_cwd_images + "LoseMainMenuN.png";

	// create a SD surface to hold the menu image
	surface = IMG_Load(MENU_Opath.c_str());

	// create a SDL texture
	SDL_Texture *loseMenu_o;

	// place surface info the texture menu
	loseMenu_o = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the menu graphic
	SDL_Rect loseMenu_oPos;

	// set the X, Y, W, and H for the Rectangle
	loseMenu_oPos.x = 19;
	loseMenu_oPos.y = 706;
	loseMenu_oPos.w = 474;
	loseMenu_oPos.h = 59;

	//********** End Main Menu Over**********

	//********** Create WIN - Start **********

	//********** WIN GRAPHICS - START **********

	string WINPath = s_cwd_images + "winTitle.png";

	// create a SD surface to hold the Instructions Title image
	surface = IMG_Load(WINPath.c_str());

	// create a SDL texture
	SDL_Texture *winScene;

	// place surface info the texture bkgd1
	winScene = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the Instructions graphic
	SDL_Rect winPos;

	// set the X, Y, W, and H for the Rectangle
	winPos.x = 330;
	winPos.y = 96;
	winPos.w = 350;
	winPos.h = 73;

	// free the SDL surface
	//SDL_FreeSurface (surface);

	//********** WIN GRAPHICS - END **********

	//********** Create PLAY AGAIN Normal **********

	string PLAY_Npath = s_cwd_images + "Play_n.png";

	// create a SD surface to hold the menu image
	surface = IMG_Load(PLAY_Npath.c_str());

	// create a SDL texture
	SDL_Texture *PLAY_n;

	// place surface info the texture menu
	PLAY_n = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the menu graphic
	SDL_Rect PLAY_nPos;

	// set the X, Y, W, and H for the Rectangle
	PLAY_nPos.x = 620;
	PLAY_nPos.y = 716;
	PLAY_nPos.w = 400;
	PLAY_nPos.h = 22;

	PLAY_Npath = s_cwd_images + "WinPlayAgainN.png";

	// create a SD surface to hold the menu image
	surface = IMG_Load(PLAY_Npath.c_str());

	// create a SDL texture
	SDL_Texture *winPlayN;

	// place surface info the texture menu
	winPlayN = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the menu graphic
	SDL_Rect winPlayNPos;

	// set the X, Y, W, and H for the Rectangle
	winPlayNPos.x = 536;
	winPlayNPos.y = 714;
	winPlayNPos.w = 448;
	winPlayNPos.h = 38;

	PLAY_Npath = s_cwd_images + "LosePlayAgainN.png";

	// create a SD surface to hold the menu image
	surface = IMG_Load(PLAY_Npath.c_str());

	// create a SDL texture
	SDL_Texture *losePlayN;

	// place surface info the texture menu
	losePlayN = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the menu graphic
	SDL_Rect losePlayNPos;

	// set the X, Y, W, and H for the Rectangle
	losePlayNPos.x = 536;
	losePlayNPos.y = 706;
	losePlayNPos.w = 469;
	losePlayNPos.h = 64;

	//********** End PLAY AGAIN Normal **********

	//********** Create PLAY AGAIN Over **********

	string PLAY_Opath = s_cwd_images + "Play_o.png";

	// create a SD surface to hold the menu image
	surface = IMG_Load(PLAY_Opath.c_str());

	// create a SDL texture
	SDL_Texture *PLAY_o;

	// place surface info the texture menu
	PLAY_o = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the menu graphic
	SDL_Rect PLAY_oPos;

	// set the X, Y, W, and H for the Rectangle
	PLAY_oPos.x = 614;
	PLAY_oPos.y = 710;
	PLAY_oPos.w = 412;
	PLAY_oPos.h = 36;

	PLAY_Opath = s_cwd_images + "WinPlayAgainO.png";

	// create a SD surface to hold the menu image
	surface = IMG_Load(PLAY_Opath.c_str());

	// create a SDL texture
	SDL_Texture *winPlayO;

	// place surface info the texture menu
	winPlayO = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the menu graphic
	SDL_Rect winPlayOPos;

	// set the X, Y, W, and H for the Rectangle
	winPlayOPos.x = 536;
	winPlayOPos.y = 714;
	winPlayOPos.w = 448;
	winPlayOPos.h = 38;

	PLAY_Opath = s_cwd_images + "LosePlayAgainO.png";

	// create a SD surface to hold the menu image
	surface = IMG_Load(PLAY_Opath.c_str());

	// create a SDL texture
	SDL_Texture *losePlayO;

	// place surface info the texture menu
	losePlayO = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the menu graphic
	SDL_Rect losePlayOPos;

	// set the X, Y, W, and H for the Rectangle
	losePlayOPos.x = 536;
	losePlayOPos.y = 706;
	losePlayOPos.w = 469;
	losePlayOPos.h = 64;

	//********** End PLAY AGAIN Over**********

	//********** Create LOSE - Start **********

	//********** LOSE TEXT GRAPHICS - START **********

	string LOSEPath = s_cwd_images + "LoseText.png";

	// create a SD surface to hold the Instructions Title image
	surface = IMG_Load(LOSEPath.c_str());

	// create a SDL texture
	SDL_Texture *loseText;

	// place surface info the texture bkgd1
	loseText = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the Instructions graphic
	SDL_Rect loseTextPos;

	// set the X, Y, W, and H for the Rectangle
	loseTextPos.x = 303;
	loseTextPos.y = 79;
	loseTextPos.w = 435;
	loseTextPos.h = 69;

	// free the SDL surface
	//SDL_FreeSurface (surface);

	//********** LOSE TEXT GRAPHICS - END **********


	//********** Create Cursor **********

	// create cursor
	string CURSORpath = s_cwd_images + "Cursor.png";

	// create a SD surface to hold the background image
	surface = IMG_Load(CURSORpath.c_str());

	// create a SDL texture
	SDL_Texture *cursor;

	// place surface info the texture bkgd1
	cursor = SDL_CreateTextureFromSurface(renderer, surface);

	// create the SDL_Rectangle for the texture's position and size - x,y,w,h
	//SDL_Rect cursorPos, activePos;

	// set the X, Y, W, and H for the Rectangle
	cursorPos.x = 0;
	cursorPos.y = 0;
	cursorPos.w = 32;
	cursorPos.h = 32;

	// set the X, Y, W, and H for the Rectangle
	activePos.x = 10;
	activePos.y = 10;
	activePos.w = 10;
	activePos.h = 10;


	// free the SDL surface
	SDL_FreeSurface(surface);

	/*
	//The surface contained by the window


	SDL_Surface* screenSurface = NULL;

	//Get window surface
	screenSurface = SDL_GetWindowSurface(window);

	//Fill the surface white
	SDL_FillRect(screenSurface, NULL,
			SDL_MapRGB(screenSurface->format, 0, 42, 254));

	//Update the surface
	SDL_UpdateWindowSurface(window);
	 */

	 // ***** Turn on Game Controller Events
	SDL_GameControllerEventState(SDL_ENABLE);

	//***** Set up Game Controller 1 variable *****
	SDL_GameController* gGameController0 = NULL;

	//***** Open Game Controller 1*****
	gGameController0 = SDL_GameControllerOpen(0);

	//***** Set up Game Controller 2 variable *****
	//SDL_GameController* gGameController1 = NULL;

	//***** Open Game Controller*****
	//gGameController1 = SDL_GameControllerOpen(1);

	// create Bibble
	Tank bibble = Tank(renderer, 0, s_cwd_images.c_str(), audio_dir.c_str(), 50.0f, 50.0f);
	Copper cop1 = Copper(renderer, s_cwd_images.c_str(), audio_dir.c_str(), 370.0f, 350.0f);

	// middle buildings
	Building building1 = Building(renderer, s_cwd_images.c_str(), audio_dir.c_str(), 375, 425, 0);
	Building building2 = Building(renderer, s_cwd_images.c_str(), audio_dir.c_str(), 1120, 425, 0);
	Building building3 = Building(renderer, s_cwd_images.c_str(), audio_dir.c_str(), 375, 870, 0);
	Building building4 = Building(renderer, s_cwd_images.c_str(), audio_dir.c_str(), 1120, 870, 0);

	// bottom buildings
	Building building5 = Building(renderer, s_cwd_images.c_str(), audio_dir.c_str(), 375, 1315, 1);
	Building building6 = Building(renderer, s_cwd_images.c_str(), audio_dir.c_str(), 1120, 1315, 1);

	// top buildings
	Building building7 = Building(renderer, s_cwd_images.c_str(), audio_dir.c_str(), 375, 100, 2);
	Building building8 = Building(renderer, s_cwd_images.c_str(), audio_dir.c_str(), 1120, 100, 2);

	float X_pos = 0.0f;

	float Y_pos = 0.0f;

	// ***** SDL Event to handle input
	SDL_Event event;

	//***** set up variables for the game states
	enum GameState { MENU, INSTRUCTIONS, LEVEL1, LEVEL2, BACKSTORY, WIN, LOSE };

	// ***** set up the initial state
	GameState gameState = LEVEL1;

	// bool value to control movement through the states
	bool menu = false, instructions = false, level1 = false, level2 = false, backStory = false, win = false, lose = false, quit = false;

	// Open Audio Channel
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	// load a MUSIC file
	//Mix_Music *bgm = Mix_LoadMUS((audio_dir + "background.wav").c_str());
	// load a WAVE file
	Mix_Music *bgm = Mix_LoadMUS((audio_dir + "spaceShip.flac").c_str());

	// if the MUSIC file is not playing - Play it
	if (!Mix_PlayingMusic())
	{
		Mix_PlayMusic(bgm, -1);
	}

	Mix_Chunk *pressSound = Mix_LoadWAV((audio_dir + "press.wav").c_str());

	Mix_Chunk *overSound = Mix_LoadWAV((audio_dir + "over.wav").c_str());

	//Mix_Chunk *explosionSound = Mix_LoadWAV((audio_dir + "explosion.wav").c_str());

	// bool value to control the over sound effect and the buttons
	bool alreadyOver = false;

	// create a pool of explosions - 20
	for (int i = 0; i < 10; i++) {
		// create the enemy
		Explode tmpExplode(renderer, s_cwd_images, -1000, -1000, 0);

		// add to the enemyList
		explodeList.push_back(tmpExplode);
	}

	// The window is open: could enter program loop here (see SDL_PollEvent())
	while (!quit)
	{
		Inst_nPos.x = 541;
		Inst_nPos.y = 361;
		Inst_nPos.w = 409;
		Inst_nPos.h = 21;

		Inst_oPos.x = 541;
		Inst_oPos.y = 361;
		Inst_oPos.w = 409;
		Inst_oPos.h = 21;

		switch (gameState)
		{
		case MENU:
			menu = true;
			alreadyOver = false;

			Player1nPos.x = 541;
			Player1nPos.y = 305;
			Player1nPos.w = 361;
			Player1nPos.h = 25;

			Player1oPos.x = 541;
			Player1oPos.y = 305;
			Player1oPos.w = 361;
			Player1oPos.h = 25;


			while (menu)
			{

				// set up frame rate for the section, or CASE
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// check for input events
				if (SDL_PollEvent(&event)) {
					// check to see if the SDL Window is closed - player clicks X in the Window
					if (event.type == SDL_QUIT) {
						quit = true;
						menu = false;
						break;
					}

					switch (event.type) {
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0)
						{
							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								if (players1Over) {
									// Play the Over Sound - plays fine through levels, must pause/delat for QUIT
									Mix_PlayChannel(-1, pressSound, 0);
									menu = false;
									gameState = LEVEL1;
									players1Over = false;
								}

								if (players2Over) {
									// Play the Over Sound - plays fine through levels, must pause/delat for QUIT
									Mix_PlayChannel(-1, pressSound, 0);
									menu = false;
									gameState = BACKSTORY;
									players2Over = false;
								}

								if (instructionsOver) {
									// Play the Over Sound - plays fine through levels, must pause/delat for QUIT
									Mix_PlayChannel(-1, pressSound, 0);
									menu = false;
									gameState = INSTRUCTIONS;
									instructionsOver = false;
								}

								if (quitOver) {
									// Play the Over Sound - plays fine through levels, must pause/delat for QUIT
									Mix_PlayChannel(-1, pressSound, 0);
									// add a slight delay
									SDL_Delay(500);
									menu = false;
									quit = true;
									quitOver = false;
								}
							}

						}
						break;

					case SDL_CONTROLLERAXISMOTION:

						moveCursor(event.caxis);
						break;
					}
				}

				// update bkgd
				//UpdateBackground(deltaTime);

				// update cursor
				UpdateCursor(deltaTime);

				// check for collision between cursor active state and buttons
				players1Over = SDL_HasIntersection(&activePos, &Player1nPos);
				players2Over = SDL_HasIntersection(&activePos, &backStorynPos);
				instructionsOver = SDL_HasIntersection(&activePos, &Inst_nPos);
				quitOver = SDL_HasIntersection(&activePos, &Quit_nPos);

				// if the cursor is over a button, play the over sound
				if (players1Over || players2Over || instructionsOver || quitOver)
				{
					if (alreadyOver == false)
					{
						Mix_PlayChannel(-1, overSound, 0);
						alreadyOver = true;
					}
				}

				// if the cursor is not over ANY button, reset the alreadyOver var
				if (!players1Over && !players2Over && !instructionsOver && !quitOver)
				{
					alreadyOver = false;
				}

				// Start Drawing

				// Clear SDL renderer
				SDL_RenderClear(renderer);

				static float timer = 0;

				timer++;

				if (timer < 200) {
					// Draw the bkgd1 image
					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);
				}
				else if (timer >= 200 && timer < 400) {
					// Draw the bkgd2 image
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd1Pos);
				}
				else if (timer >= 400 && timer < 600) {
					// Draw the bkgd2 image
					SDL_RenderCopy(renderer, bkgd3, NULL, &bkgd1Pos);
				}
				else if (timer >= 600 && timer < 800) {
					// Draw the bkgd2 image
					SDL_RenderCopy(renderer, bkgd4, NULL, &bkgd1Pos);
				}
				else if (timer >= 800 && timer < 1000) {
					// Draw the bkgd2 image
					SDL_RenderCopy(renderer, bkgd3, NULL, &bkgd1Pos);
				}
				else if (timer >= 1000 && timer < 1200) {
					// Draw the bkgd2 image
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd1Pos);
				}
				else {
					timer = 0;
				}

				// Draw the title image
				SDL_RenderCopy(renderer, title, NULL, &titlePos);


				///////////////////////// NEW //////////////////////////////////
				if (players1Over) {
					// Draw the Player1_n image
					SDL_RenderCopy(renderer, Player1_n, NULL, &Player1nPos);
				}
				else {
					// Draw the Player1_o image
					SDL_RenderCopy(renderer, Player1_o, NULL, &Player1oPos);
				}

				if (players2Over) {
					// Draw the Player2_n image
					SDL_RenderCopy(renderer, backStoryN, NULL, &backStorynPos);
				}
				else {
					// Draw the Player2_o image
					SDL_RenderCopy(renderer, backStoryO, NULL, &backStoryoPos);
				}

				if (instructionsOver) {
					// Draw the Instructions_n image
					SDL_RenderCopy(renderer, Instructions_n, NULL, &Inst_nPos);
				}
				else {
					// Draw the Instructions_o image
					SDL_RenderCopy(renderer, Instructions_o, NULL, &Inst_nPos);
				}

				if (quitOver) {
					// Draw the Quit_n image
					SDL_RenderCopy(renderer, Quit_n, NULL, &Quit_nPos);
				}
				else {
					// Draw the Quit_o image
					SDL_RenderCopy(renderer, Quit_o, NULL, &Quit_nPos);
				}
				///////////////////////// NEW //////////////////////////////////

				// Draw the cursor image
				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

				// SDL Render present
				SDL_RenderPresent(renderer);
			}
			break; // end main menu case

		case INSTRUCTIONS:
			instructions = true;
			alreadyOver = false;

			Player1nPos.x = 626;
			Player1nPos.y = 723;
			Player1nPos.w = 361;
			Player1nPos.h = 25;

			Player1oPos.x = 626;
			Player1oPos.y = 723;
			Player1oPos.w = 361;
			Player1oPos.h = 25;

			Menu_nPos.x = 53;
			Menu_nPos.y = 723;
			Menu_nPos.w = 370;
			Menu_nPos.h = 21;

			Menu_oPos.x = 53;
			Menu_oPos.y = 723;
			Menu_oPos.w = 370;
			Menu_oPos.h = 21;

			while (instructions)
			{

				// set up frame rate for the section, or CASE
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// check for input events
				if (SDL_PollEvent(&event)) {
					// check to see if the SDL Window is closed - player clicks X in the Window
					if (event.type == SDL_QUIT) {
						quit = true;
						instructions = false;
						break;
					}

					switch (event.type) {
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0)
						{
							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								if (menuOver) {
									// Play the Over Sound - plays fine through levels, must pause/delay for QUIT
									Mix_PlayChannel(-1, pressSound, 0);
									instructions = false;
									gameState = MENU;
								}

								if (players1Over) {
									// Play the Over Sound - plays fine through levels, must pause/delay for QUIT
									Mix_PlayChannel(-1, pressSound, 0);
									instructions = false;
									gameState = LEVEL1;
								}
							}
						}
						break;
					case SDL_CONTROLLERAXISMOTION:
						moveCursor(event.caxis);
						break;
					}
				}

				MakeExplosion(475, 450);

				// update
				//UpdateBackground(deltaTime);

				// cursor update
				UpdateCursor(deltaTime);

				// check to see if active
				if (explodeList[0].active == true)
				{
					// draw explode
					explodeList[0].Update(deltaTime);
				}

				// check for cursor intersection with menu button
				menuOver = SDL_HasIntersection(&activePos, &Menu_nPos);
				players1Over = SDL_HasIntersection(&activePos, &Player1nPos);

				// if the cursor is over a button, play the over sound
				if (menuOver || players1Over)
				{
					if (alreadyOver == false)
					{
						Mix_PlayChannel(-1, overSound, 0);
						alreadyOver = true;
					}
				}

				// if the cursor is not over ANY button, reset the alreadyOver var
				if (!menuOver && !players1Over)
				{
					alreadyOver = false;
				}



				// Start Drawing

				// Clear SDL renderer
				SDL_RenderClear(renderer);

				// Draw the bkgd1 image
				//SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);
				// Draw the bkgd1 image
				SDL_RenderCopy(renderer, bkgdInst, NULL, &bkgd1Pos);

				// check to see if active
				if (explodeList[0].active == true)
				{
					// draw explode
					explodeList[0].Draw(renderer);
				}

				// Draw the bkgd2 image
				//SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				if (players1Over) {
					// Draw the Instructions_n image
					SDL_RenderCopy(renderer, Player1_n, NULL, &Player1nPos);
				}
				else {
					// Draw the Instructions_o image
					SDL_RenderCopy(renderer, Player1_o, NULL, &Player1oPos);
				}

				// Draw the title image
				//SDL_RenderCopy(renderer, title, NULL, &titlePos);

				// Draw the instructions graphic image
				SDL_RenderCopy(renderer, instructionText, NULL, &instructionTextPos);

				if (menuOver) {
					// Draw the menu_n image
					SDL_RenderCopy(renderer, MainMenu_n, NULL, &Menu_nPos);
				}
				else {
					// Draw the menu_o image
					SDL_RenderCopy(renderer, Menu_o, NULL, &Menu_oPos);
				}

				// Draw the cursor image
				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

				// SDL Render present
				SDL_RenderPresent(renderer);
			}
			break; // end instructions case

		case LEVEL1:
			level1 = true;
			alreadyOver = false;

			while (level1)
			{
				// set up frame rate for the section, or CASE
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// check for input events
				if (SDL_PollEvent(&event)) {
					// check to see if the SDL Window is closed - player clicks X in the Window
					if (event.type == SDL_QUIT) {
						quit = true;
						level1 = false;
						break;
					}

					switch (event.type) {
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0)
						{
							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								bibble.OnControllerButton(event.cbutton);
							}

							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
							{
								SDL_Delay(500);
								level1 = false;
								quit = true;
								break;
							}
						}
						break;
					case SDL_CONTROLLERAXISMOTION:
						//moveCursor(event.caxis);
						break;
					default:break;
					}
				}

				// get values for both x and y of the controller
				const Sint16 Xvalue = SDL_GameControllerGetAxis(gGameController0, SDL_CONTROLLER_AXIS_LEFTX);
				const Sint16 Yvalue = SDL_GameControllerGetAxis(gGameController0, SDL_CONTROLLER_AXIS_LEFTY);

				// update
				//UpdateBackground(deltaTime);

				// update cursor
				//UpdateCursor(deltaTime);

				// update the buildings
				building1.Update(deltaTime, bibble.posRect);
				building2.Update(deltaTime, bibble.posRect);
				building3.Update(deltaTime, bibble.posRect);
				building4.Update(deltaTime, bibble.posRect);
				building5.Update(deltaTime, bibble.posRect);
				building6.Update(deltaTime, bibble.posRect);

				// pass to player 1
				bibble.OnControllerAxis(Xvalue, Yvalue);

				// update the player 1 tank
				bibble.Update(deltaTime);

				// update the coppers
				cop1.Update(deltaTime, bibble.posRect);

				if (SDL_HasIntersection(&bibble.posRect, &building1.baseRect) || SDL_HasIntersection(&bibble.posRect, &building2.baseRect) || SDL_HasIntersection(&bibble.posRect, &building3.baseRect) ||
					SDL_HasIntersection(&bibble.posRect, &building4.baseRect) || SDL_HasIntersection(&bibble.posRect, &building5.baseRect) || SDL_HasIntersection(&bibble.posRect, &building6.baseRect) ||
					SDL_HasIntersection(&bibble.posRect, &building7.baseRect) || SDL_HasIntersection(&bibble.posRect, &building8.baseRect))
				{
					bibble.speed = -2000;
				}
				else {
					bibble.speed = 100;
				}

				// move background long the x axis
				// right
				if ((bibble.posRect.x >= 512) && (bibble.Xvalue > 8000))
				{
					X_pos -= (bibble.speed) * deltaTime;

					if ((bkgdRect.x > -1374))
					{
						bkgdRect.x = (int)(X_pos + .5f);

						// move the buildings
						building1.TankMoveX(-bibble.speed, deltaTime);
						building2.TankMoveX(-bibble.speed, deltaTime);
						building3.TankMoveX(-bibble.speed, deltaTime);
						building4.TankMoveX(-bibble.speed, deltaTime);
						building5.TankMoveX(-bibble.speed, deltaTime);
						building6.TankMoveX(-bibble.speed, deltaTime);
						building7.TankMoveX(-bibble.speed, deltaTime);
						building8.TankMoveX(-bibble.speed, deltaTime);

						cop1.eCopperMoveX(-bibble.speed, deltaTime);

						for (int i = 0; i < bibble.bulletList.size(); i++)
						{
							if (bibble.bulletList[i].active)
								if (bibble.bulletList[i].stop)
								{
									bibble.bulletList[i].TankMoveX(-bibble.speed, deltaTime);
								}
						}

					}
					else {
						X_pos = bkgdRect.x;
					}
				}
				// left
				if ((bibble.posRect.x <= 512) && (bibble.Xvalue < 8000))
				{
					X_pos += (bibble.speed) * deltaTime;

					if ((bkgdRect.x < 370))
					{
						bkgdRect.x = (int)(X_pos + 0.5f);

						// move the buildings
						building1.TankMoveX(bibble.speed, deltaTime);
						building2.TankMoveX(bibble.speed, deltaTime);
						building3.TankMoveX(bibble.speed, deltaTime);
						building4.TankMoveX(bibble.speed, deltaTime);
						building5.TankMoveX(bibble.speed, deltaTime);
						building6.TankMoveX(bibble.speed, deltaTime);
						building7.TankMoveX(bibble.speed, deltaTime);
						building8.TankMoveX(bibble.speed, deltaTime);

						cop1.eCopperMoveX(bibble.speed, deltaTime);

						for (int i = 0; i < bibble.bulletList.size(); i++)
						{
							if (bibble.bulletList[i].active)
								if (bibble.bulletList[i].stop)
								{
									bibble.bulletList[i].TankMoveX(bibble.speed, deltaTime);
								}
						}
					}
					else {
						X_pos = bkgdRect.x;
					}
				}

				// move background along the y axis
				// down
				if ((bibble.posRect.y >= 384) && (bibble.Yvalue > 8000))
				{
					Y_pos -= (bibble.speed) * deltaTime;

					if ((bkgdRect.y > -968))
					{
						bkgdRect.y = (int)(Y_pos + .5f);

						// move the buildings
						building1.TankMoveY(-bibble.speed, deltaTime);
						building2.TankMoveY(-bibble.speed, deltaTime);
						building3.TankMoveY(-bibble.speed, deltaTime);
						building4.TankMoveY(-bibble.speed, deltaTime);
						building5.TankMoveY(-bibble.speed, deltaTime);
						building6.TankMoveY(-bibble.speed, deltaTime);
						building7.TankMoveY(-bibble.speed, deltaTime);
						building8.TankMoveY(-bibble.speed, deltaTime);

						cop1.eCopperMoveY(-bibble.speed, deltaTime);

						for (int i = 0; i < bibble.bulletList.size(); i++)
						{
							if (bibble.bulletList[i].active)
								if (bibble.bulletList[i].stop)
								{
									bibble.bulletList[i].TankMoveY(-bibble.speed, deltaTime);
								}
						}
					}
					else {
						Y_pos = bkgdRect.y;
					}
				}
				// up
				if ((bibble.posRect.y <= 384) && (bibble.Yvalue < 8000))
				{
					Y_pos += (bibble.speed) * deltaTime;

					if ((bkgdRect.y < 250))
					{
						bkgdRect.y = (int)(Y_pos + 0.5f);

						// move the buildings
						building1.TankMoveY(bibble.speed, deltaTime);
						building2.TankMoveY(bibble.speed, deltaTime);
						building3.TankMoveY(bibble.speed, deltaTime);
						building4.TankMoveY(bibble.speed, deltaTime);
						building5.TankMoveY(bibble.speed, deltaTime);
						building6.TankMoveY(bibble.speed, deltaTime);
						building7.TankMoveY(bibble.speed, deltaTime);
						building8.TankMoveY(bibble.speed, deltaTime);

						cop1.eCopperMoveY(bibble.speed, deltaTime);

						for (int i = 0; i < bibble.bulletList.size(); i++)
						{
							if (bibble.bulletList[i].active)
								if (bibble.bulletList[i].stop)
								{
									bibble.bulletList[i].TankMoveY(bibble.speed, deltaTime);
								}
						}
					}
					else {
						Y_pos = bkgdRect.y;
					}
				}

				// Start Drawing

				// Clear SDL renderer
				SDL_RenderClear(renderer);

				// Draw the bkgd1 image
				SDL_RenderCopy(renderer, level1bkgd, NULL, &bkgdRect);

				// draw bibble
				bibble.Draw(renderer);

				// draw the coppers
				cop1.Draw(renderer);

				building1.Draw(renderer);
				building2.Draw(renderer);
				building3.Draw(renderer);
				building4.Draw(renderer);
				building5.Draw(renderer);
				building6.Draw(renderer);
				building7.Draw(renderer);
				building8.Draw(renderer);

				// SDL Render present
				SDL_RenderPresent(renderer);
			}
			break;
		case LEVEL2:
			level2 = true;
			alreadyOver = false;

			while (level2)
			{
				// set up frame rate for the section, or CASE
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// check for input events
				if (SDL_PollEvent(&event)) {
					// check to see if the SDL Window is closed - player clicks X in the Window
					if (event.type == SDL_QUIT) {
						quit = true;
						level2 = false;
						break;
					}

					switch (event.type) {
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0)
						{
							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								if (menuOver) {
									// Play the Over Sound - plays fine through levels, must pause/delat for QUIT
									Mix_PlayChannel(-1, pressSound, 0);
									level2 = false;
									gameState = MENU;
								}

								if (playOver) {
									// Play the Over Sound - plays fine through levels, must pause/delat for QUIT
									Mix_PlayChannel(-1, pressSound, 0);
									level1 = false;
									gameState = LEVEL2;
								}
							}
						}
						break;
					case SDL_CONTROLLERAXISMOTION:
						//moveCursor(event.caxis);
						break;
					default:break;
					}
				}

				// get values for both x and y of the controller
				const Sint16 Xvalue = SDL_GameControllerGetAxis(gGameController0, SDL_CONTROLLER_AXIS_LEFTX);
				const Sint16 Yvalue = SDL_GameControllerGetAxis(gGameController0, SDL_CONTROLLER_AXIS_LEFTY);

				// update
				//UpdateBackground(deltaTime);

				// update cursor
				//UpdateCursor(deltaTime);

				// pass to player 1
				bibble.OnControllerAxis(Xvalue, Yvalue);

				// update the player 1 tank
				bibble.Update(deltaTime);

				// move background long the x axis
				if ((bibble.posRect.x > 512) && (bibble.Xvalue > 8000))
				{
					X_pos -= (bibble.speed) * deltaTime;

					if ((bkgdRect.x > -1374))
					{
						bkgdRect.x = (int)(X_pos + .5f);

					}
					else {
						X_pos = bkgdRect.x;
					}
				}

				if ((bibble.posRect.x <= 512) && (bibble.Xvalue < 8000))
				{
					X_pos += (bibble.speed) * deltaTime;

					if ((bkgdRect.x < 370))
					{
						bkgdRect.x = (int)(X_pos + 0.5f);

					}
					else {
						X_pos = bkgdRect.x;
					}
				}

				// move background along the y axis
				if ((bibble.posRect.y > 384) && (bibble.Yvalue > 8000))
				{
					Y_pos -= (bibble.speed) * deltaTime;

					if ((bkgdRect.y > -968))
					{
						bkgdRect.y = (int)(Y_pos + .5f);

					}
					else {
						Y_pos = bkgdRect.y;
					}
				}

				if ((bibble.posRect.y <= 384) && (bibble.Yvalue < 8000))
				{
					Y_pos += (bibble.speed) * deltaTime;

					if ((bkgdRect.y < 250))
					{
						bkgdRect.y = (int)(Y_pos + 0.5f);

					}
					else {
						Y_pos = bkgdRect.y;
					}
				}

				// if the cursor is not over ANY button, reset the alreadyOver var
				if (!menuOver && !playOver)
				{
					alreadyOver = false;
				}

				// Start Drawing

				// Clear SDL renderer
				SDL_RenderClear(renderer);

				// Draw the bkgd1 image
				SDL_RenderCopy(renderer, level1bkgd, NULL, &bkgdRect);

				// draw bibble
				bibble.Draw(renderer);

				// SDL Render present
				SDL_RenderPresent(renderer);
			}
			break;
		case BACKSTORY:
			backStory = true;
			alreadyOver = false;

			Inst_nPos.x = 10;
			Inst_nPos.y = 58;
			Inst_nPos.w = 409;
			Inst_nPos.h = 21;

			Inst_oPos.x = 10;
			Inst_oPos.y = 58;
			Inst_oPos.w = 409;
			Inst_oPos.h = 21;

			Player1nPos.x = 10;
			Player1nPos.y = 8;
			Player1nPos.w = 361;
			Player1nPos.h = 25;

			Player1oPos.x = 10;
			Player1oPos.y = 8;
			Player1oPos.w = 361;
			Player1oPos.h = 25;

			Menu_nPos.x = 10;
			Menu_nPos.y = 103;
			Menu_nPos.w = 370;
			Menu_nPos.h = 21;

			Menu_oPos.x = 10;
			Menu_oPos.y = 103;
			Menu_oPos.w = 370;
			Menu_oPos.h = 21;

			while (backStory)
			{

				// set up frame rate for the section, or CASE
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// check for input events
				if (SDL_PollEvent(&event)) {
					// check to see if the SDL Window is closed - player clicks X in the Window
					if (event.type == SDL_QUIT) {
						quit = true;
						backStory = false;
						break;
					}

					switch (event.type) {
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0)
						{
							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								if (menuOver) {
									// Play the Over Sound - plays fine through levels, must pause/delat for QUIT
									Mix_PlayChannel(-1, pressSound, 0);
									backStory = false;
									gameState = MENU;
								}

								if (instructionsOver) {
									// Play the Over Sound - plays fine through levels, must pause/delat for QUIT
									Mix_PlayChannel(-1, pressSound, 0);
									backStory = false;
									gameState = INSTRUCTIONS;
									instructionsOver = false;
								}

								if (players1Over) {
									// Play the Over Sound - plays fine through levels, must pause/delat for QUIT
									Mix_PlayChannel(-1, pressSound, 0);
									backStory = false;
									gameState = LEVEL1;
									players1Over = false;
								}
							}
						}
						break;

					case SDL_CONTROLLERAXISMOTION:

						moveCursor(event.caxis);
						break;
					}
				}

				// update
				//UpdateBackground(deltaTime);

				// cursor update
				UpdateCursor(deltaTime);

				// check for cursor intersection with menu button
				menuOver = SDL_HasIntersection(&activePos, &Menu_nPos);
				instructionsOver = SDL_HasIntersection(&activePos, &Inst_nPos);
				players1Over = SDL_HasIntersection(&activePos, &Player1nPos);

				// if the cursor is over a button, play the over sound
				if (menuOver || players1Over || instructionsOver)
				{
					if (alreadyOver == false)
					{
						Mix_PlayChannel(-1, overSound, 0);
						alreadyOver = true;
					}
				}

				// if the cursor is not over ANY button, reset the alreadyOver var
				if (!menuOver && !instructionsOver && !players1Over)
				{
					alreadyOver = false;
				}

				// Start Drawing

				// Clear SDL renderer
				SDL_RenderClear(renderer);

				static float timer = 0;

				timer++;

				if (timer < 200) {
					// Draw the bkgd1 image
					SDL_RenderCopy(renderer, bkgd5, NULL, &bkgd1Pos);
				}
				else if (timer >= 200 && timer < 400) {
					// Draw the bkgd2 image
					SDL_RenderCopy(renderer, bkgd6, NULL, &bkgd1Pos);
				}
				else if (timer >= 400 && timer < 600) {
					// Draw the bkgd2 image
					SDL_RenderCopy(renderer, bkgd7, NULL, &bkgd1Pos);
				}
				else if (timer >= 600 && timer < 800) {
					// Draw the bkgd2 image
					SDL_RenderCopy(renderer, bkgd8, NULL, &bkgd1Pos);
				}
				else if (timer >= 800 && timer < 1000) {
					// Draw the bkgd2 image
					SDL_RenderCopy(renderer, bkgd7, NULL, &bkgd1Pos);
				}
				else if (timer >= 1000 && timer < 1200) {
					// Draw the bkgd2 image
					SDL_RenderCopy(renderer, bkgd6, NULL, &bkgd1Pos);
				}
				else {
					// Draw the bkgd1 image
					SDL_RenderCopy(renderer, bkgd5, NULL, &bkgd1Pos);
					timer = 0;
				}

				// Draw the title image
				SDL_RenderCopy(renderer, title, NULL, &titlePos);

				// Draw the instructions graphic image
				SDL_RenderCopy(renderer, backStoryText, NULL, &backStoryTextPos);

				if (menuOver) {
					// Draw the menu_n image
					SDL_RenderCopy(renderer, MainMenu_n, NULL, &Menu_nPos);
				}
				else {
					// Draw the menu_o image
					SDL_RenderCopy(renderer, Menu_o, NULL, &Menu_oPos);
				}

				if (instructionsOver) {
					// Draw the Instructions_n image
					SDL_RenderCopy(renderer, Instructions_n, NULL, &Inst_nPos);
				}
				else {
					// Draw the Instructions_o image
					SDL_RenderCopy(renderer, Instructions_o, NULL, &Inst_nPos);
				}

				if (players1Over) {
					// Draw the Instructions_n image
					SDL_RenderCopy(renderer, Player1_n, NULL, &Player1nPos);
				}
				else {
					// Draw the Instructions_o image
					SDL_RenderCopy(renderer, Player1_o, NULL, &Player1oPos);
				}

				// Draw the cursor image
				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

				// SDL Render present
				SDL_RenderPresent(renderer);
			}

			break; // end backStory case

		case WIN:
			win = true;
			alreadyOver = false;

			while (win)
			{
				// set up frame rate for the section, or CASE
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// check for input events
				if (SDL_PollEvent(&event)) {
					// check to see if the SDL Window is closed - player clicks X in the Window
					if (event.type == SDL_QUIT) {
						quit = true;
						win = false;
						break;
					}

					switch (event.type) {
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0)
						{
							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								if (menuOver) {
									// Play the Over Sound - plays fine through levels, must pause/delat for QUIT
									Mix_PlayChannel(-1, pressSound, 0);
									win = false;
									gameState = MENU;
								}

								if (playOver) {
									// Play the Over Sound - plays fine through levels, must pause/delat for QUIT
									Mix_PlayChannel(-1, pressSound, 0);
									win = false;
									gameState = LEVEL1;
									playOver = false;
								}
							}
						}

						break;
					case SDL_CONTROLLERAXISMOTION:
						moveCursor(event.caxis);
						break;
					default:break;
					}
				}

				// update
				//UpdateBackground(deltaTime);

				// update cursor
				UpdateCursor(deltaTime);


				// check to see if active
				if (explodeList[0].active == true)
				{
					// draw explode
					explodeList[0].Update(deltaTime);
				}

				// check for cursor intersection with menu button
				menuOver = SDL_HasIntersection(&activePos, &WinMenu_nPos);
				playOver = SDL_HasIntersection(&activePos, &winPlayNPos);

				// if the cursor is over a button, play the over sound
				if (menuOver || playOver)
				{
					if (alreadyOver == false)
					{
						Mix_PlayChannel(-1, overSound, 0);
						alreadyOver = true;
					}
				}

				MakeExplosion(730, 390);

				// if the cursor is not over ANY button, reset the alreadyOver var
				if (!menuOver && !playOver)
				{
					alreadyOver = false;
				}

				// Start Drawing

				// Clear SDL renderer
				SDL_RenderClear(renderer);

				// Draw the bkgd1 image
				SDL_RenderCopy(renderer, bkgd9, NULL, &bkgd1Pos);

				// check to see if active
				if (explodeList[0].active == true)
				{
					// draw explode
					explodeList[0].Draw(renderer);
				}

				// Draw the bkgd2 image
				//SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				// Draw the Win Scene image
				SDL_RenderCopy(renderer, winTitle, NULL, &winTitlePos);

				if (playOver) {
					// Draw the Play Again N image
					SDL_RenderCopy(renderer, winPlayO, NULL, &winPlayOPos);
				}
				else {
					// Draw the Play Again O image
					SDL_RenderCopy(renderer, winPlayN, NULL, &winPlayNPos);
				}

				if (menuOver) {
					// Draw the Menu N image
					SDL_RenderCopy(renderer, WinMainMenu_n, NULL, &WinMenu_nPos);
				}
				else {
					// Draw the Menu O image
					SDL_RenderCopy(renderer, winMenu_o, NULL, &winMenu_oPos);
				}

				// Draw the cursor image
				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

				// SDL Render present
				SDL_RenderPresent(renderer);

			}
			break; // end main menu case

		case LOSE:
			lose = true;
			alreadyOver = false;

			while (lose)
			{
				// set up frame rate for the section, or CASE
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// check for input events
				if (SDL_PollEvent(&event)) {
					// check to see if the SDL Window is closed - player clicks X in the Window
					if (event.type == SDL_QUIT) {
						quit = true;
						lose = false;
						break;
					}

					switch (event.type) {
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0)
						{
							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								if (menuOver) {
									// Play the Over Sound - plays fine through levels, must pause/delat for QUIT
									Mix_PlayChannel(-1, pressSound, 0);
									lose = false;
									gameState = MENU;
								}

								if (playOver) {
									// Play the Over Sound - plays fine through levels, must pause/delat for QUIT
									Mix_PlayChannel(-1, pressSound, 0);
									lose = false;
									gameState = LEVEL1;
								}
							}
						}

						break;
					case SDL_CONTROLLERAXISMOTION:
						moveCursor(event.caxis);
						break;
					default:break;
					}
				}

				// update
				//UpdateBackground(deltaTime);

				// update cursor
				UpdateCursor(deltaTime);

				// check for cursor intersection with menu button
				menuOver = SDL_HasIntersection(&activePos, &loseMenu_oPos);
				playOver = SDL_HasIntersection(&activePos, &losePlayNPos);

				// if the cursor is over a button, play the over sound
				if (menuOver || playOver)
				{
					if (alreadyOver == false)
					{
						Mix_PlayChannel(-1, overSound, 0);
						alreadyOver = true;
					}
				}

				// if the cursor is not over ANY button, reset the alreadyOver var
				if (!menuOver && !playOver)
				{
					alreadyOver = false;
				}

				// Start Drawing

				// Clear SDL renderer
				SDL_RenderClear(renderer);

				// Draw the bkgd1 image
				SDL_RenderCopy(renderer, bkgd10, NULL, &bkgd1Pos);

				// Draw the bkgd2 image
				//SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				// Draw the Lose Scene image
				SDL_RenderCopy(renderer, loseText, NULL, &loseTextPos);

				if (playOver) {
					// Draw the Play Again N image
					SDL_RenderCopy(renderer, losePlayO, NULL, &losePlayOPos);
				}
				else {
					// Draw the Play Again O image
					SDL_RenderCopy(renderer, losePlayN, NULL, &losePlayNPos);
				}

				if (menuOver) {
					// Draw the Menu N image
					SDL_RenderCopy(renderer, LoseMainMenu_n, NULL, &LoseMenu_nPos);
				}
				else {
					// Draw the Menu O image
					SDL_RenderCopy(renderer, loseMenu_o, NULL, &loseMenu_oPos);
				}

				// Draw the cursor image
				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

				// SDL Render present
				SDL_RenderPresent(renderer);

			}
			break; // end main menu case
		default:break;
		}
	}

	// SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example

	// Close and destroy the window
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();
	return 0;
}


