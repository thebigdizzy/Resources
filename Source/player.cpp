#include "player.h"

// Analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;

// Player creation Method
Player::Player(SDL_Renderer *renderer, int pNum, string filePath, string audioPath, float x, float y)
{
	// set the player number 0 or 1
	playerNum = pNum;

	// set float for player speed
	speed = 500.0f;

	laser = Mix_LoadWAV((audioPath + "laser.wav").c_str());

	// init score and lives vars
	oldScore = 0;
	playerScore = 0;
	oldLives = 0;
	playerLives = 3;

	// init the font system
	TTF_Init();

	// load the font
	font = TTF_OpenFont((audioPath + "Airacobra Extra Bold.ttf").c_str(), 40);

	// see if this is player 1, or player 2, and create the correctX and Y locations
	if(playerNum == 0)
	{
		// create the score texture X and Y position
		scorePos.x = scorePos.y = 10;
		livesPos.x = 10;
		livesPos.y = 40;
	} else {
		// create the score texture X and Y position
		scorePos.x = 650;
		scorePos.y = 10;
		livesPos.x = 650;
		livesPos.y = 40;
	}

	// update score
	UpdateScore(renderer);

	// update lives
	UpdateLives(renderer);

	// see if this is player 1, or player 2, and create the correct file path
	if (playerNum == 0)
	{
		// Create the player 1 texture
		playerPath = filePath + "Player1.png";
	}
	else {
		// Create the player 2 texture
		playerPath = filePath + "Player2.png";
	}

	// load the surface
	surface = IMG_Load(playerPath.c_str());

	// load the surface into the texture
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	// free the surface for later use
	SDL_FreeSurface(surface);

	// set the SDL_Rect X and Y for the player
	posRect.x = x;
	posRect.y = y;

	// Use SDL_QueryTexture to get the W and H of the player's texture
	int w, h;

	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	posRect.w = w;
	posRect.h = h;

	// Set the movement floats to the players original X and Y
	pos_X = x;
	pos_Y = y;

	// set the xDir and yDir for the joysticks
	xDir = 0;
	yDir = 0;

	// string to create the path to the player's bullet image
	string bulletPath;

	// see if this is red player, or blue player, and create the correct file path 
	if (playerNum == 0) {
		// create the red bullet texture
		bulletPath = filePath + "bullet2.png";
	}
	else {
		// create the blue bullet texture
		bulletPath = filePath + "bullet.png";
	}

	// create the player's bullet pos
	for (int i = 0; i < 10; i++)
	{
		// create the bullet and move offscreen, out of the game play area
		Bullet tmpBullet(renderer, bulletPath, -1000, -1000);

		// add to the bulletList
		bulletList.push_back(tmpBullet);
	}
}

// Update Score
void Player::UpdateLives(SDL_Renderer *renderer)
{
	// fix for to_string problems on linux
	string Result;		// string which will contain the result
	ostringstream convert;	// stream used for the conversion
	convert << playerLives;	// insert the textual representation of 'Number' in the characters in the stream
	Result = convert.str();	// set 'Result' to the contents of the stream

	// create the text for the font texture
	tempLives = "Player Lives: " + Result;

	// check to see what player this is and color the font as needed
	if(playerNum == 0)
	{
		// place the player 1 lives info into a surface
		livesSurface = TTF_RenderText_Solid(font, tempLives.c_str(), colorP1);
	} else {
		// place the player 2 lives info into a surface
		livesSurface = TTF_RenderText_Solid(font, tempLives.c_str(), colorP2);
	}

	// create the player lives texture
	livesTexture = SDL_CreateTextureFromSurface(renderer, livesSurface);

	// get the Width and Height of the texture
	SDL_QueryTexture(livesTexture, NULL, NULL, &livesPos.w, &livesPos.h);

	// free surface
	SDL_FreeSurface(livesSurface);

	// set old score
	oldLives = playerLives;
}

// Update Score
void Player::UpdateScore(SDL_Renderer *renderer)
{
	// fix for to_string problems on linux
	string Result;		// string which will contain the result
	ostringstream convert;	// stream used for the conversion
	convert << playerScore;	// insert the textual representation of 'Number' in the characters in the stream
	Result = convert.str();	// set 'Result' to the contents of the stream

	// create the text for the font texture
	tempScore = "Player Score: " + Result;

	// check to see what player this is and color the font as needed
	if(playerNum == 0)
	{
		// place the player 1 score info into a surface
		scoreSurface = TTF_RenderText_Solid(font, tempScore.c_str(), colorP1);
	} else {
		// place the player 2 score info into a surface
		scoreSurface = TTF_RenderText_Solid(font, tempScore.c_str(), colorP2);
	}

	// create the player score texture
	scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);

	// get the Width and Height of the texture
	SDL_QueryTexture(scoreTexture, NULL, NULL, &scorePos.w, &scorePos.h);

	// free surface
	SDL_FreeSurface(scoreSurface);

	// set old score
	oldScore = playerScore;
}

// create a bullet
void Player::CreateBullet() {
	// see if there is a bullet active to fire
	for (int s = 0; s < bulletList.size(); s++)
	{
		// see if the bullet is not active
		if (bulletList[s].active == false) 
		{

			// Play the Laser sound when firing a bullet
			Mix_PlayChannel(-1, laser, 0);

			// set bullet to active
			bulletList[s].active = true;

			// use some math in the x position to get the bullet close to
			// the center of the player using player width
			bulletList[s].posRect.x = (pos_X + (posRect.w / 2));

			// finishing aligning to the player center using the texture width
			bulletList[s].posRect.x = (bulletList[s].posRect.x - (bulletList[s].posRect.w / 2));
			bulletList[s].posRect.y = posRect.y;

			// Set the x and y positions of the bullet's float positions
			bulletList[s].pos_X = pos_X;
			bulletList[s].pos_Y = pos_Y;

			// once bullet is found, break out of loop
			break;
		}
	}
}

// Player Joystick Button Method
void Player::OnControllerButton(const SDL_ControllerButtonEvent event)
{
	// if the player's number is 0 and the joystick button is from joystick 0
	if (event.which == 0 && playerNum == 0)
	{
		// if A Button
		if (event.button == 0)
		{
			//cout << "Player 1 - Button A" << endl;
			// Test - change players score
			playerScore += 10;

			// Test - change players lives
			playerLives -= 1;

			// create a bullet
			CreateBullet();
		}
	}

	// if the player's number is 1 and the joystick button is from joystick 1
	if (event.which == 1 && playerNum == 1)
	{
		// if A button
		if (event.button == 0)
		{
			// cout << "Player 2 - Button A" << endl;

			// Test - change players score
			playerScore += 10;

			// Test - change players lives
			playerLives -= 1;

			// create a bullet
			CreateBullet();
		}
	}
}

// Player Joystick Axis Method
void Player::OnControllerAxis(const SDL_ControllerAxisEvent event)
{
	// Axis movements and button presses both sent gere as SDL_ControllerAxisEvent structures

	// if the player's number is 0 and the joystick axis is from joystick 0
	if (event.which == 0 && playerNum == 0)
	{
		// X axis
		if (event.axis == 0)
		{
			if (event.value < -JOYSTICK_DEAD_ZONE)
			{
				xDir = -1.0f; // LEFT
			}
			else if (event.value > JOYSTICK_DEAD_ZONE)
			{
				xDir = 1.0f; // RIGHT
			}
			else
			{
				xDir = 0.0f; // NONE
			}
		}

		// Y axis
		if (event.axis == 1)
		{
			if (event.value < -JOYSTICK_DEAD_ZONE)
			{
				yDir = -1.0f; // DOWN
			}
			else if (event.value > JOYSTICK_DEAD_ZONE)
			{
				yDir = 1.0f; // UP
			}
			else
			{
				yDir = 0.0f; // NONE
			}
		}
	}

	// if the player's number is 0 and the joystick axis is from joystick 0
	if (event.which == 1 && playerNum == 1)
	{
		// X axis
		if (event.axis == 0)
		{
			if (event.value < -JOYSTICK_DEAD_ZONE)
			{
				xDir = -1.0f; // LEFT
			}
			else if (event.value > JOYSTICK_DEAD_ZONE)
			{
				xDir = 1.0f; // RIGHT
			}
			else
			{
				xDir = 0.0f; // NONE
			}
		}

		// Y axis
		if (event.axis == 1)
		{
			if (event.value < -JOYSTICK_DEAD_ZONE)
			{
				yDir = -1.0f; // DOWN
			}
			else if (event.value > JOYSTICK_DEAD_ZONE)
			{
				yDir = 1.0f; // UP
			}
			else
			{
				yDir = 0.0f; // NONE
			}
		}
	}
}

void Player::Update(float deltaTime, SDL_Renderer *renderer)
{
	// Adjust position floats based on speed, direction of hoystick axis and deltaTime
	pos_X += (speed * xDir) * deltaTime;
	pos_Y += (speed * yDir) * deltaTime;

	// Update player position with code to account for precition loss
	posRect.x = (int)(pos_X + 0.5f);
	posRect.y = (int)(pos_Y + 0.5f);

	if (posRect.x < 0) {
		posRect.x = 0;
		pos_X = posRect.x;
	}

	if (posRect.x > 1024 - posRect.w) {
		posRect.x = 1024 - posRect.w;
		pos_X = posRect.x;
	}

	if (posRect.y < 0) {
		posRect.y = 0;
		pos_Y = posRect.y;
	}

	if (posRect.y > 768 - posRect.h) {
		posRect.y = 768 - posRect.h;
		pos_Y = posRect.y;
	}

	// Update the player's bullets
	for (int i = 0; i < bulletList.size(); i++)
	{
		// check to see if the bulelt is active
		if (bulletList[i].active) {
			// update bullet
			bulletList[i].Update(deltaTime);
		}
	}

	// should the score be updated?
	if(playerScore != oldScore) {
		UpdateScore(renderer);
	}

	// should the lives be updated?
		if(playerLives != oldLives) {
			UpdateLives(renderer);
		}
}

// Player Draw method
void Player::Draw(SDL_Renderer *renderer)
{
	// Draw the player texture using the vars texture and posRect
	SDL_RenderCopy(renderer, texture, NULL, &posRect);

	// Draw the player's bullets
	for (int i = 0; i < bulletList.size(); i++)
	{
		// check to see if the bulelt is active
		if (bulletList[i].active) {
			// update bullet
			bulletList[i].Draw(renderer);
		}
	}

	// draw the player score
	SDL_RenderCopy(renderer, scoreTexture, NULL, &scorePos);

	// draw the player lives
		SDL_RenderCopy(renderer, livesTexture, NULL, &livesPos);

}

Player::~Player()
{
	SDL_DestroyTexture(texture);
}


