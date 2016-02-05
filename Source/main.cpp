/*
 * main.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: danieltrayler
 */

#if defined(_WIN32) || (_WIN64)
#include "SDL.h"
#include "SDL_image.h"
#endif

#if defined(__linux__)
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#endif

#if defined(__APPLE__)
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
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
	BG1pos_Y += (bkgdSpeed * 1) * deltaTime;

	// set the new bkgd1 position
	bkgd1Pos.y = (int)(BG1pos_Y + 0.5f);

	// RESET WHEN OFF THE BOTTOM OF THE SCREEN
	if(bkgd1Pos.y >= 768){

		bkgd1Pos.y = -768;

		BG1pos_Y = bkgd1Pos.y;
	}

	// Update background 2
	BG2pos_Y += (bkgdSpeed * 1) * deltaTime;

	// set the new bkgd2 position
	bkgd2Pos.y = (int)(BG2pos_Y + 0.5f);

	// RESET WHEN OFF THE BOTTOM OF THE SCREEN
	if(bkgd2Pos.y >= 768){

		bkgd2Pos.y = -768;

		BG2pos_Y = bkgd2Pos.y;
	}
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
	if(event.which == 0) {
		if(event.axis == 0) {

			if(event.value < -JOYSTICK_DEAD_ZONE) {
				xDir = -1.0f;
			} else if (event.value > JOYSTICK_DEAD_ZONE) {
				xDir = 1.0f;
			} else {
				xDir = 0.0f;
			}
		}

	if(event.axis == 1) {

			if(event.value < -JOYSTICK_DEAD_ZONE) {
				yDir = -1.0f;
			} else if (event.value > JOYSTICK_DEAD_ZONE) {
				yDir = 1.0f;
			} else {
				yDir = 0.0f;
			}
		}
	}
}

void UpdateCursor(float deltaTime) {
	// Update cursor
	pos_X += (cursorSpeed * xDir) * deltaTime;
	pos_Y += (cursorSpeed * yDir) * deltaTime;

	cursorPos.x = (int) (pos_X + .5f);
	cursorPos.y = (int) (pos_Y + .5f);

	activePos.x = cursorPos.x;
	activePos.y = cursorPos.y;

	if(cursorPos.x < 0) {
		cursorPos.x = 0;
		pos_X = cursorPos.x;
	}

	if(cursorPos.x > 1024 - cursorPos.w) {
		cursorPos.x = 1024 - cursorPos.w;
		pos_Y = cursorPos.x;
	}

	if(cursorPos.y < 0) {
		cursorPos.y = 0;
		pos_Y = cursorPos.y;
	}

	if(cursorPos.y > 768 - cursorPos.h) {
		cursorPos.y = 768 - cursorPos.h;
		pos_Y = cursorPos.y;
	}
}

// variables for all Menus button over
bool players1Over = false, players2Over = false, instructionsOver = false,
		quitOver = false, menuOver = false, playOver = false;

// class header includes
#include "player.h"


int main(int argc, char* argv[]){

#if defined(_WIN32) || (_WIN64)
	cout << "Running on Windows" << endl;
	cout << "Added on Windows" << endl;

	// Get the current working directory
	string s_cwd(getcwd(NULL, 0));

	// create a string linking to the mac's images folder
	string s_cwd_images = s_cwd + "\\Resources\\Images\\";
#endif

#if defined(__linux__)
	cout << "Running on Linux" << endl;
	cout << "Added on Linux" << endl;

	// Get the current working directory
	string s_cwd(getcwd(NULL, 0));

	// create a string linking to the mac's images folder
	string s_cwd_images = s_cwd + "/Resources/Images";
#endif

#if defined(__APPLE__)
	cout << "Running on Apple" << endl;
	cout << "Added on Apple" << endl;

	// Get the current working directory
	string s_cwd(getcwd(NULL, 0));

	// create a string linking to the mac's images folder
	string s_cwd_images = s_cwd + "/Resources/Images";

	// test
	cout << s_cwd_images << endl;
#endif


	SDL_Window *window;                    // Declare a pointer

	SDL_Init (SDL_INIT_EVERYTHING);              // Initialize SDL2

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
	renderer =  SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// ******************* CREATE PLAYERS - START **********************
	Player player1 = Player(renderer, 0, s_cwd_images.c_str(), 250.0, 500.0);
	Player player2 = Player(renderer, 1, s_cwd_images.c_str(), 750.0, 500.0);

	//********** Create Background **********
	string BKGDpath = s_cwd_images + "/Background.png";

	// create a SD surface to hold the background image
	SDL_Surface *surface = IMG_Load(BKGDpath.c_str());

	// create a SDL texture
	SDL_Texture *bkgd1;

	// place surface info the texture bkgd1
	bkgd1 = SDL_CreateTextureFromSurface(renderer, surface);

	// create a SDL texture
	SDL_Texture *bkgd2;

	// place surface info the texture bkgd2
	bkgd2 = SDL_CreateTextureFromSurface(renderer, surface);

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

	//********** Create Main Menu **********

	//********** Create Title **********

	string TITLEpath = s_cwd_images + "/Title.png";

	// create a SD surface to hold the background image
	surface = IMG_Load(TITLEpath.c_str());

	// create a SDL texture
	SDL_Texture *title;

	// place surface info the texture bkgd1
	title = SDL_CreateTextureFromSurface(renderer, surface);

	// create SDL Rectangle for the title graphic
	SDL_Rect titlePos;

	// set the X, Y, W, and H for the Rectangle
	titlePos.x = 298;
	titlePos.y = 118;
	titlePos.w = 459;
	titlePos.h = 73;

	// free the SDL surface
	//SDL_FreeSurface (surface);

	//********** End Title **********

	//********** Create 1 Player Normal **********

		string P1_Npath = s_cwd_images + "/Player1_n.png";

		// create a SD surface to hold the Player1 image
		surface = IMG_Load(P1_Npath.c_str());

		// create a SDL texture
		SDL_Texture *Player1_n;

		// place surface info the texture Player1
		Player1_n = SDL_CreateTextureFromSurface(renderer, surface);

		// create SDL Rectangle for the Player1 graphic
		SDL_Rect Player1nPos;

		// set the X, Y, W, and H for the Rectangle
		Player1nPos.x = 446;
		Player1nPos.y = 517;
		Player1nPos.w = 284;
		Player1nPos.h = 27;

		//********** End 1 Player Normal **********

		//********** Create 1 Player Over **********

		string P1_Opath = s_cwd_images + "/Player1_o.png";

		// create a SD surface to hold the Player1 image
		surface = IMG_Load(P1_Opath.c_str());

		// create a SDL texture
		SDL_Texture *Player1_o;

		// place surface info the texture Player1
		Player1_o = SDL_CreateTextureFromSurface(renderer, surface);

		// create SDL Rectangle for the Player1 graphic
		SDL_Rect Player1oPos;

		// set the X, Y, W, and H for the Rectangle
		Player1oPos.x = 444;
		Player1oPos.y = 512;
		Player1oPos.w = 296;
		Player1oPos.h = 41;

		//********** End 1 Player Over**********

		//********** Create 2 Player Normal **********

		string P2_Npath = s_cwd_images + "/Player2_n.png";

		// create a SD surface to hold the Player2 image
		surface = IMG_Load(P2_Npath.c_str());

		// create a SDL texture
		SDL_Texture *Player2_n;

		// place surface info the texture Player2
		Player2_n = SDL_CreateTextureFromSurface(renderer, surface);

		// create SDL Rectangle for the player2 graphic
		SDL_Rect Player2nPos;

		// set the X, Y, W, and H for the Rectangle
		Player2nPos.x = 446;
		Player2nPos.y = 583;
		Player2nPos.w = 295;
		Player2nPos.h = 27;

		//********** End 2 Player Normal **********

		//********** Create 2 Player Over **********

		string P2_Opath = s_cwd_images + "/Player2_o.png";

		// create a SD surface to hold the player2 image
		surface = IMG_Load(P2_Opath.c_str());

		// create a SDL texture
		SDL_Texture *Player2_o;

		// place surface info the texture player2
		Player2_o = SDL_CreateTextureFromSurface(renderer, surface);

		// create SDL Rectangle for the player2 graphic
		SDL_Rect Player2oPos;

		// set the X, Y, W, and H for the Rectangle
		Player2oPos.x = 444;
		Player2oPos.y = 579;
		Player2oPos.w = 307;
		Player2oPos.h = 41;

		//********** End 2 Player Over**********

		//********** Create Instructions Normal **********

		string INST_Npath = s_cwd_images + "/Instructions_n.png";

		// create a SD surface to hold the instructions image
		surface = IMG_Load(INST_Npath.c_str());

		// create a SDL texture
		SDL_Texture *Instructions_n;

		// place surface info the texture instructions
		Instructions_n = SDL_CreateTextureFromSurface(renderer, surface);

		// create SDL Rectangle for the Instructions graphic
		SDL_Rect Inst_nPos;

		// set the X, Y, W, and H for the Rectangle
		Inst_nPos.x = 446;
		Inst_nPos.y = 444;
		Inst_nPos.w = 441;
		Inst_nPos.h = 40;

		//********** End Instructions Normal **********

		//********** Create Instructions Over **********

		string INST_Opath = s_cwd_images + "/Instructions_o.png";

		// create a SD surface to hold the Instructions image
		surface = IMG_Load(INST_Opath.c_str());

		// create a SDL texture
		SDL_Texture *Instructions_o;

		// place surface info the texture Instructions
		Instructions_o = SDL_CreateTextureFromSurface(renderer, surface);

		// create SDL Rectangle for the Instructions graphic
		SDL_Rect Inst_oPos;

		// set the X, Y, W, and H for the Rectangle
		Inst_oPos.x = 444;
		Inst_oPos.y = 444;
		Inst_oPos.w = 453;
		Inst_oPos.h = 45;

	//********** End Instructions Over**********

		//********** Create Quit Normal **********

		string QUIT_Npath = s_cwd_images + "/Quit_n.png";

		// create a SD surface to hold the Quit image
		surface = IMG_Load(QUIT_Npath.c_str());

		// create a SDL texture
		SDL_Texture *Quit_n;

		// place surface info the texture Quit
		Quit_n = SDL_CreateTextureFromSurface(renderer, surface);

		// create SDL Rectangle for the Quit graphic
		SDL_Rect Quit_nPos;

		// set the X, Y, W, and H for the Rectangle
		Quit_nPos.x = 446;
		Quit_nPos.y = 652;
		Quit_nPos.w = 156;
		Quit_nPos.h = 31;

		//********** End Quit Normal **********

		//********** Create Quit Over **********

		string QUIT_Opath = s_cwd_images + "/Quit_o.png";

		// create a SD surface to hold the Quit image
		surface = IMG_Load(QUIT_Opath.c_str());

		// create a SDL texture
		SDL_Texture *Quit_o;

		// place surface info the texture Instructions
		Quit_o = SDL_CreateTextureFromSurface(renderer, surface);

		// create SDL Rectangle for the Quit graphic
		SDL_Rect Quit_oPos;

		// set the X, Y, W, and H for the Rectangle
		Quit_oPos.x = 444;
		Quit_oPos.y = 647;
		Quit_oPos.w = 168;
		Quit_oPos.h = 45;

	//********** End Quit Over**********


	// ************** END MAIN MENU ****************

		//********** Create Instructions Menu - Start **********

		//********** INSTRUCTIONS GRAPHICS - START **********

			string instructionsTextPath = s_cwd_images + "/InstructionsText.png";

			// create a SD surface to hold the Instructions Title image
			surface = IMG_Load(instructionsTextPath.c_str());

			// create a SDL texture
			SDL_Texture *instructionText;

			// place surface info the texture bkgd1
			instructionText = SDL_CreateTextureFromSurface(renderer, surface);

			// create SDL Rectangle for the Instructions graphic
			SDL_Rect instructionTextPos;

			// set the X, Y, W, and H for the Rectangle
			instructionTextPos.x = 26;
			instructionTextPos.y = 336;
			instructionTextPos.w = 980;
			instructionTextPos.h = 144;

			// free the SDL surface
			//SDL_FreeSurface (surface);

			//********** INSTRUCTIONS GRAPHICS - END **********

			//********** Create Main Menu Normal **********

			string MENU_Npath = s_cwd_images + "/MainMenu_n.png";

			// create a SD surface to hold the menu image
			surface = IMG_Load(MENU_Npath.c_str());

			// create a SDL texture
			SDL_Texture *MainMenu_n;

			// place surface info the texture menu
			MainMenu_n = SDL_CreateTextureFromSurface(renderer, surface);

			// create SDL Rectangle for the menu graphic
			SDL_Rect Menu_nPos;

			// set the X, Y, W, and H for the Rectangle
			Menu_nPos.x = 30;
			Menu_nPos.y = 716;
			Menu_nPos.w = 400;
			Menu_nPos.h = 22;

			//********** End Main Menu Normal **********

			//********** Create Main Menu Over **********

			string MENU_Opath = s_cwd_images + "/MainMenu_o.png";

			// create a SD surface to hold the menu image
			surface = IMG_Load(MENU_Opath.c_str());

			// create a SDL texture
			SDL_Texture *Menu_o;

			// place surface info the texture menu
			Menu_o = SDL_CreateTextureFromSurface(renderer, surface);

			// create SDL Rectangle for the menu graphic
			SDL_Rect Menu_oPos;

			// set the X, Y, W, and H for the Rectangle
			Menu_oPos.x = 26;
			Menu_oPos.y = 710;
			Menu_oPos.w = 412;
			Menu_oPos.h = 36;

		//********** End Main Menu Over**********

		//********** Create WIN - Start **********

		//********** WIN GRAPHICS - START **********

			string WINPath = s_cwd_images + "/winTitle.png";

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

			string PLAY_Npath = s_cwd_images + "/Play_n.png";

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

			//********** End PLAY AGAIN Normal **********

			//********** Create PLAY AGAIN Over **********

			string PLAY_Opath = s_cwd_images + "/Play_o.png";

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

		//********** End PLAY AGAIN Over**********

			//********** Create LOSE - Start **********

		//********** LOSE GRAPHICS - START **********

			string LOSEPath = s_cwd_images + "/loseTitle.png";

			// create a SD surface to hold the Instructions Title image
			surface = IMG_Load(LOSEPath.c_str());

			// create a SDL texture
			SDL_Texture *loseScene;

			// place surface info the texture bkgd1
			loseScene = SDL_CreateTextureFromSurface(renderer, surface);

			// create SDL Rectangle for the Instructions graphic
			SDL_Rect losePos;

			// set the X, Y, W, and H for the Rectangle
			losePos.x = 330;
			losePos.y = 96;
			losePos.w = 350;
			losePos.h = 73;

			// free the SDL surface
			//SDL_FreeSurface (surface);

	//********** LOSE GRAPHICS - END **********


	//********** Create Cursor **********

	// create cursor
	string CURSORpath = s_cwd_images + "/Cursor.png";

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
	SDL_FreeSurface (surface);

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

	//***** Set up a Game Controller variable *****
	SDL_GameController* gGameController0 = NULL;

	//***** Open Game Controller*****
	gGameController0 = SDL_GameControllerOpen(0);

	// ***** SDL Event to handle input
	SDL_Event event;

	//***** set up variables for the game states
	enum GameState{MENU, INSTRUCTIONS, PLAYERS1, PLAYERS2, WIN, LOSE};

	// ***** set up the initial state
	GameState gameState = MENU;

	// bool value to control movement through the states
	bool menu = false, instructions = false, players1 = false, players2 = false, win = false, lose = false, quit = false;

	// The window is open: could enter program loop here (see SDL_PollEvent())
	while (!quit)
	{
		switch (gameState)
		{
		case MENU:
			menu = true;

			cout << endl;

			while(menu)
			{

				// set up frame rate for the section, or CASE
				thisTime = SDL_GetTicks();
				deltaTime =  (float)(thisTime - lastTime)/1000;
				lastTime = thisTime;

				// check for input events
				if(SDL_PollEvent(&event)){
					// check to see if the SDL Window is closed - player clicks X in the Window
					if(event.type == SDL_QUIT){
						quit = true;
						menu = false;
						break;
					}

					switch(event.type){
					case SDL_CONTROLLERBUTTONDOWN:
						if(event.cdevice.which == 0)
						{
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								if(players1Over) {
									menu = false;
									gameState = PLAYERS1;
									players1Over = false;
								}

								if(players2Over) {
									menu = false;
									gameState = PLAYERS2;
									players2Over = false;
								}

								if(instructionsOver) {
									menu = false;
									gameState = INSTRUCTIONS;
									instructionsOver = false;
								}

								if(quitOver) {
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
				UpdateBackground(deltaTime);

				// update cursor
				UpdateCursor(deltaTime);

				// check for collision between cursor active state and buttons
				players1Over = SDL_HasIntersection(&activePos, &Player1nPos);
				players2Over = SDL_HasIntersection(&activePos, &Player2nPos);
				instructionsOver = SDL_HasIntersection(&activePos, &Inst_nPos);
				quitOver = SDL_HasIntersection(&activePos, &Quit_nPos);

				// Start Drawing

				// Clear SDL renderer
				SDL_RenderClear(renderer);

				// Draw the bkgd1 image
				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				// Draw the bkgd2 image
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				// Draw the title image
				SDL_RenderCopy(renderer, title, NULL, &titlePos);
				///////////////////////// NEW //////////////////////////////////
				if(players1Over){
					// Draw the Player1_n image
					SDL_RenderCopy(renderer, Player1_n, NULL, &Player1nPos);
				} else {
					// Draw the Player1_o image
					SDL_RenderCopy(renderer, Player1_o, NULL, &Player1oPos);
				}

				if(players2Over) {
					// Draw the Player2_n image
					SDL_RenderCopy(renderer, Player2_n, NULL, &Player2nPos);
				} else {
					// Draw the Player2_o image
					SDL_RenderCopy(renderer, Player2_o, NULL, &Player2oPos);
				}

				if(instructionsOver) {
					// Draw the Instructions_n image
					SDL_RenderCopy(renderer, Instructions_n, NULL, &Inst_nPos);
				} else {
					// Draw the Instructions_o image
					SDL_RenderCopy(renderer, Instructions_o, NULL, &Inst_nPos);
				}

				if(quitOver) {
				// Draw the Quit_n image
				SDL_RenderCopy(renderer, Quit_n, NULL, &Quit_nPos);
				} else {
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

		while(instructions)
		{

			// set up frame rate for the section, or CASE
			thisTime = SDL_GetTicks();
			deltaTime =  (float)(thisTime - lastTime)/1000;
			lastTime = thisTime;

			// check for input events
			if(SDL_PollEvent(&event)){
				// check to see if the SDL Window is closed - player clicks X in the Window
				if(event.type == SDL_QUIT){
					quit = true;
					instructions = false;
					break;
				}

				switch(event.type){
				case SDL_CONTROLLERBUTTONDOWN:
					if(event.cdevice.which == 0)
					{
						if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
						{
							if(menuOver){
								instructions = false;
								gameState = MENU;
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
			UpdateBackground(deltaTime);

			// cursor update
			UpdateCursor(deltaTime);

			// check for cursor intersection with menu button
			menuOver = SDL_HasIntersection(&activePos, &Menu_nPos);

			// Start Drawing

			// Clear SDL renderer
			SDL_RenderClear(renderer);

			// Draw the bkgd1 image
			SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

			// Draw the bkgd2 image
			SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

			// Draw the title image
			SDL_RenderCopy(renderer, title, NULL, &titlePos);

			// Draw the instructions graphic image
			SDL_RenderCopy(renderer, instructionText, NULL, &instructionTextPos);

			if(menuOver) {
			// Draw the menu_n image
			SDL_RenderCopy(renderer, MainMenu_n, NULL, &Menu_nPos);
			} else {
			// Draw the menu_o image
			SDL_RenderCopy(renderer, Menu_o, NULL, &Menu_oPos);
			}
			// Draw the cursor image
			SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

			// SDL Render present
			SDL_RenderPresent(renderer);
		}
		break; // end instructions case

		case PLAYERS1:
				players1 = true;

				while(players1)
				{

					// set up frame rate for the section, or CASE
					thisTime = SDL_GetTicks();
					deltaTime =  (float)(thisTime - lastTime)/1000;
					lastTime = thisTime;

					// check for input events
					if(SDL_PollEvent(&event)){
						// check to see if the SDL Window is closed - player clicks X in the Window
						if(event.type == SDL_QUIT){
							quit = true;
							players1 = false;
							break;
						}

						switch(event.type)
						{
						case SDL_CONTROLLERBUTTONDOWN:
							if(event.cdevice.which == 0)
							{
								if(event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
								{
									players1 = false;
									gameState = WIN;
								}

								if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
								{
									players1 = false;
									gameState = LOSE;
								}

								// send button press info to player1
								player1.OnControllerButton(event.cbutton);
							}
							break;

						case SDL_CONTROLLERAXISMOTION:

							player1.OnControllerAxis(event.caxis);
							break;
						default:break;
						}
					}

					// update
					UpdateBackground(deltaTime);

					// update player1
					player1.Update(deltaTime);

					// check for cursor button collision
					// menuOver = SDL_HasIntersection(&activePos, &Menu_nPos);
					// playOver = SDL_HasIntersection(&activePos, &PLAY_nPos);

					// Start Drawing

					// Clear SDL renderer
					SDL_RenderClear(renderer);

					// Draw the bkgd1 image
					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

					// Draw the bkgd2 image
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

					// draw player
					player1.Draw(renderer);

					// SDL Render present
					SDL_RenderPresent(renderer);
				}
				break; // end main menu case

		case PLAYERS2:
				players2 = true;

				while(players2)
				{
					// set up frame rate for the section, or CASE
					thisTime = SDL_GetTicks();
					deltaTime =  (float)(thisTime - lastTime)/1000;
					lastTime = thisTime;

					// check for input events
					if(SDL_PollEvent(&event)){
						// check to see if the SDL Window is closed - player clicks X in the Window
						if(event.type == SDL_QUIT){
							quit = true;
							players2 = false;
							break;
						}

						switch(event.type){
						case SDL_CONTROLLERBUTTONDOWN:
							if(event.cdevice.which == 0)
							{
								if(event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
								{
									players2 = false;
									gameState = WIN;
								}
								if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
								{
									players2 = false;
									gameState = LOSE;
								}
							}

							// send button press info to player 1
							player1.OnControllerButton(event.cbutton);

							// send button press info to player 2
							player2.OnControllerButton(event.cbutton);

							break;
						case SDL_CONTROLLERAXISMOTION:
							// send axis info to player 1
							player1.OnControllerAxis(event.caxis);

							// send button press info to plaeyr 2
							player2.OnControllerAxis(event.caxis);
							break;
						default:break;
						}
					}

					// update
					UpdateBackground(deltaTime);

					// update player 1
					player1.Update(deltaTime);

					// Update Player 2
					player2.Update(deltaTime);

					// Start Drawing

					// Clear SDL renderer
					SDL_RenderClear(renderer);

					// Draw the bkgd1 image
					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

					// Draw the bkgd2 image
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

					// Draw the Player1_n image
					// SDL_RenderCopy(renderer, Player2_n, NULL, &Player2nPos);

					// draw player 1
					player1.Draw(renderer);

					// draw player 2
					player2.Draw(renderer);

					// SDL Render present
					SDL_RenderPresent(renderer);
				}
				break; // end main menu case

		case WIN:
				win = true;

				while(win)
				{

					// set up frame rate for the section, or CASE
					thisTime = SDL_GetTicks();
					deltaTime =  (float)(thisTime - lastTime)/1000;
					lastTime = thisTime;

					// check for input events
					if(SDL_PollEvent(&event)){
						// check to see if the SDL Window is closed - player clicks X in the Window
						if(event.type == SDL_QUIT){
							quit = true;
							win = false;
							break;
						}

						switch(event.type){
						case SDL_CONTROLLERBUTTONDOWN:
							if(event.cdevice.which == 0)
							{
								if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
								{
									if(menuOver) {
										win = false;
										gameState = MENU;
									}

									if(playOver) {
										win = false;
										gameState = PLAYERS1;
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
					UpdateBackground(deltaTime);

					// update cursor
					UpdateCursor(deltaTime);

					// check for cursor intersection with menu button
					menuOver = SDL_HasIntersection(&activePos, &Menu_nPos);
					playOver = SDL_HasIntersection(&activePos, &PLAY_nPos);

					// Start Drawing

					// Clear SDL renderer
					SDL_RenderClear(renderer);

					// Draw the bkgd1 image
					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

					// Draw the bkgd2 image
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

					// Draw the Win Scene image
					SDL_RenderCopy(renderer, winScene, NULL, &winPos);

					if(playOver){
						// Draw the Play Again N image
						SDL_RenderCopy(renderer, PLAY_n, NULL, &PLAY_nPos);
					} else {
						// Draw the Play Again O image
						SDL_RenderCopy(renderer, PLAY_o, NULL, &PLAY_oPos);
					}

					if(menuOver){
						// Draw the Menu N image
						SDL_RenderCopy(renderer, MainMenu_n, NULL, &Menu_nPos);
					} else {
						// Draw the Menu O image
						SDL_RenderCopy(renderer, Menu_o, NULL, &Menu_oPos);
					}

					// Draw the cursor image
					SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

					// SDL Render present
					SDL_RenderPresent(renderer);

				}
				break; // end main menu case

		case LOSE:
				lose = true;

				while(lose)
				{
					// set up frame rate for the section, or CASE
					thisTime = SDL_GetTicks();
					deltaTime =  (float)(thisTime - lastTime)/1000;
					lastTime = thisTime;

					// check for input events
					if(SDL_PollEvent(&event)){
						// check to see if the SDL Window is closed - player clicks X in the Window
						if(event.type == SDL_QUIT){
							quit = true;
							lose = false;
							break;
						}

						switch(event.type){
						case SDL_CONTROLLERBUTTONDOWN:
							if(event.cdevice.which == 0)
							{
								if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
								{
									if(menuOver){
									lose = false;
									gameState = MENU;
									}

									if(playOver){
										lose = false;
										gameState = PLAYERS1;
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
					UpdateBackground(deltaTime);

					// update cursor
					UpdateCursor(deltaTime);

					// check for cursor intersection with menu button
					menuOver = SDL_HasIntersection(&activePos, &Menu_nPos);
					playOver = SDL_HasIntersection(&activePos, &PLAY_nPos);

					// Start Drawing

					// Clear SDL renderer
					SDL_RenderClear(renderer);

					// Draw the bkgd1 image
					SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

					// Draw the bkgd2 image
					SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

					// Draw the Lose Scene image
					SDL_RenderCopy(renderer, loseScene, NULL, &losePos);

					if(playOver) {
						// Draw the Play Again N image
						SDL_RenderCopy(renderer, PLAY_n, NULL, &PLAY_nPos);
					} else {
						// Draw the Play Again O image
						SDL_RenderCopy(renderer, PLAY_o, NULL, &PLAY_oPos);
					}

					if(menuOver) {
						// Draw the Menu N image
						SDL_RenderCopy(renderer, MainMenu_n, NULL, &Menu_nPos);
					} else {
						// Draw the Menu O image
						SDL_RenderCopy(renderer, Menu_o, NULL, &Menu_oPos);
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


