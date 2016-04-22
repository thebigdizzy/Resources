#include "key.h"

Key::Key(SDL_Renderer *renderer, string filePath, float x, float y)
{
	active = true;

	string basePath = filePath + "skeletonKey.png";

	key = IMG_LoadTexture(renderer, basePath.c_str());

	keyRect.x = x;
	keyRect.y = y;

	sRect.x = x;
	sRect.y = y;

	int w, h;
	SDL_QueryTexture(key, NULL, NULL, &w, &h);
	keyRect.w = w / 10;
	keyRect.h = h / 10;

	posJ_X = keyRect.x;
	posJ_Y = keyRect.y;
}

void Key::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, key, NULL, &keyRect);
}

void Key::Update(float deltaTime)
{

}

void Key::Reset()
{
	active = true;
	keyRect.x = posJ_X = sRect.x;
	keyRect.y = posJ_Y = sRect.y;
}

void Key::TankMoveX(float tankSpeed, float deltaTime)
{
	posJ_X += (tankSpeed)* deltaTime;

	keyRect.x = (int)(posJ_X + 0.5f);
}

void Key::TankMoveY(float tankSpeed, float deltaTime)
{
	posJ_Y += (tankSpeed)* deltaTime;

	keyRect.y = (int)(posJ_Y + 0.5f);
}
