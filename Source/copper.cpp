#include "copper.h"

Copper::Copper(SDL_Renderer *renderer, string filePath, string audioPath, float x, float y, int id)
{
	active = true;

	turn = false;

	idNum = id;

	if(id == 0){
		rt = true;
		lt = false;
		up = false;
		dn = false;
	} else if(id == 1){
		rt = false;
		lt = true;
		up = false;
		dn = false;
	}else if(id == 2){
		rt = false;
		lt = false;
		up = false;
		dn = true;
	}else if(id == 3){
		rt = false;
		lt = false;
		up = true;
		dn = false;
	}

	

	//cAnim = Mix_LoadWAV((audioPath + "fire.wav").c_str());

	string basePath = filePath + "flashLightCopper.png";

	// create a pool of explosions - 20
	for (int i = 0; i < 2; i++)
	{
		// create the enemy
		Animate tmpAnim(renderer, filePath, x, y, 1);

		// add to the enemyList
		cAnim.push_back(tmpAnim);
	}

	posX = x;
	posY = y;
	sPosX = x;
	sPosY = y;
	lPosX = x + 50;
	lPosY = y - 25;

	flashLight = IMG_LoadTexture(renderer, basePath.c_str());

	cAnim[0].posRect.x = sRect.x = x;
	cAnim[0].posRect.y = sRect.y = y;

	fLightRect.x = x;
	fLightRect.y = y;

	int w, h;
	SDL_QueryTexture(flashLight, NULL, NULL, &w, &h);
	fLightRect.w = w*4;
	fLightRect.h = h*4;

	//cAnim[0].posRect.w = 100;
	//cAnim[0].posRect.h = 100;

	posT_X = cAnim[0].posRect.x;
	posT_Y = cAnim[0].posRect.y;

	speed = 70;

	health = 10;

	CopperAngle = 0;

	MakeCopper(x, y);

	center.x = -25;
	center.y = fLightRect.h / 2 - 5;
}

void Copper::Update(float deltaTime, SDL_Rect CopperRect)
{
	cAnim[0].Update(deltaTime);

	float move_x = 0;
	float move_y = 0;

	double distancex = (cAnim[0].posRect.x - CopperRect.x) * (cAnim[0].posRect.x - CopperRect.x);
	double distancey = (cAnim[0].posRect.y - CopperRect.y) * (cAnim[0].posRect.y - CopperRect.y);

	double calcdistance = sqrt(distancex + distancey);

	//cout << "angle: " << center.x << endl;

	/*if (calcdistance <= 1000)
	{
		active = true;
	}
	else {
		active = false;
	}*/

	if (active) {
		if(idNum == 0){
			if (posX < sPosX) {
				rt = true;
				lt = false;
				turn = true;
			}
			else if (posX > sPosX + 500) {
				lt = true;
				rt = false;
				turn = true;
			}

			if (rt && turn && !lt) {
				CopperAngle += .1f;
			}
			else if (lt && turn && !rt) {
				CopperAngle -= .1f;
			}

			if (CopperAngle <= -180)
				turn = false;
			else if (CopperAngle >= 0)
				turn = false;

			if (rt && !turn){
				CopperAngle = 0;
				move_x = speed;
			}
			else if(lt && !turn){
				CopperAngle = -180;
				move_x = -speed;
			}
		} else if(idNum == 1){
			if (posX < sPosX - 500) {
				rt = true;
				lt = false;
				turn = true;
			}
			else if (posX > sPosX) {
				lt = true;
				rt = false;
				turn = true;
			}

			if (rt && turn && !lt) {
				CopperAngle -= .1f;
			}
			else if (lt && turn && !rt) {
				CopperAngle += .1f;
			}

			if (CopperAngle <= 0)
				turn = false;
			else if (CopperAngle >= 180)
				turn = false;

			if (rt && !turn){
				CopperAngle = 0;
				move_x = speed;
			}
			else if(lt && !turn){
				CopperAngle = 180;
				move_x = -speed;
			}
		}

		//lPosX = center.x * cos(((CopperAngle)* M_PI) / 180);
		//lPosY = center.y * sin(((CopperAngle)* M_PI) / 180);

		if (!turn) {
			//CopperAngle = atan2(y, x) * 180 / 3.14;

			//float radians = (0 * 3.14) / 180;

			posT_X += (move_x)* deltaTime;
			posT_Y += (move_y)* deltaTime;

			posX += (move_x)* deltaTime;
			posY += (move_y)* deltaTime;

			lPosX += (move_x)* deltaTime;
			lPosY += (move_y)* deltaTime;

			cAnim[0].posRect.x = (int)(posT_X + 0.5f);
			cAnim[0].posRect.y = (int)(posT_Y + 0.5f);

			fLightRect.x = (int)(lPosX + 0.5f);
			fLightRect.y = (int)(lPosY + 0.5f);
		}
	}
}

void Copper::Draw(SDL_Renderer *renderer)
{
	cAnim[0].Draw(renderer, CopperAngle);
	SDL_RenderCopyEx(renderer, flashLight, NULL, &fLightRect, CopperAngle, &center, SDL_FLIP_NONE);
}

void Copper::Reset()
{
	cAnim[0].posRect.x = -1000;

	posT_X = cAnim[0].posRect.x;

	health = 10;

	active = false;
}

void Copper::reset()
{
	cAnim[0].posRect.x = sRect.x;
	cAnim[0].posRect.y = sRect.y;

	posT_X = sPosX;
	posT_Y = sPosY;

	fLightRect.x = sRect.x;
	fLightRect.y = sRect.y;

	posX = sPosX;
	posY = sPosY;

	lPosX = sPosX + 50;
	lPosY = sPosY - 25;
}

void Copper::RemoveHealth()
{
	health -= 1;

	if (health <= 0)
	{
		Reset();
	}
}

int Copper::checkCollision(SDL_Rect target, SDL_Point point)
{
	SDL_Point difference, object;
	difference.x = target.x - fLightRect.x;
	difference.y = target.y - fLightRect.y;
	int r = target.w / 2;

	object.x = (difference.x * cos(-CopperAngle * M_PI / 180)) - (difference.y * sin(-CopperAngle * M_PI / 180));
	object.y = (difference.x * sin(-CopperAngle * M_PI / 180)) + (difference.y * cos(-CopperAngle * M_PI / 180));

	if (object.x - 50 < r + (fLightRect.w) && object.x > 0 && object.y < (fLightRect.h / 2) && object.y >  (-fLightRect.h / 2)) {
		if (object.x  - 50 < r + (fLightRect.w) && lt || object.x < r + (fLightRect.w) && rt) {
			//cout << "yep" << endl;
			return 1;
		}
	}

	return 0;
}

void Copper::eCopperMoveX(float CopperSpeed, float deltaTime)
{
	posT_X += (CopperSpeed)* deltaTime;
	lPosX += (CopperSpeed)* deltaTime;

	cAnim[0].posRect.x = (int)(posT_X + 0.5f);
	fLightRect.x = (int)(lPosX + 0.5f);
}

void Copper::eCopperMoveY(float CopperSpeed, float deltaTime)
{
	posT_Y += (CopperSpeed)* deltaTime;
	lPosY += (CopperSpeed)* deltaTime;

	cAnim[0].posRect.y = (int)(posT_Y + 0.5f);
	fLightRect.y = (int)(lPosY + 0.5f);
}

void Copper::MakeCopper(int x, int y)
{
	// see if there is an explosion not active to use
	for (int i = 0; i < cAnim.size(); i++)
	{
		// see if the explosion is not active
		if (cAnim[i].active == false)
		{
			// set explosion to active
			cAnim[i].active = true;

			// use some math in the x position to get the bullet close to
			// the center of the player using player width
			cAnim[i].posRect.x = x;
			cAnim[i].posRect.y = y;

			// once explosion is found, break out of loop
			break;
		}
	}
}
