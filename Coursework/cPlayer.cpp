/*
=================
cRocket.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cPlayer.h"


/*
=================================================================
Defualt Constructor
=================================================================
*/
cPlayer::cPlayer() : cSprite(), rocketVelocity(0) 
{
}

/*
=================================================================
Update the sprite position
=================================================================
*/

void cPlayer::update(double deltaTime)
{
	auto rads = PI / 180.0f * (this->getSpriteRotAngle() - 90.0f);

	FPoint direction = { 0.0f, 0.0f };
	direction.X = (float)(cos(rads));
	direction.Y = (float)(sin(rads));

	SDL_Rect currentSpritePos = this->getSpritePos();

	currentSpritePos.x += (int)(this->rocketVelocity * this->moveH * deltaTime);
	currentSpritePos.y += (int)(this->rocketVelocity * this->moveV * deltaTime);

	this->setSpritePos({ currentSpritePos.x , currentSpritePos.y  });
	this->setBoundingRect(this->getSpritePos());
}
/*
=================================================================
Sets the velocity for the rocket
=================================================================
*/
void cPlayer::setRocketVelocity(int rocketVel)
{
	rocketVelocity = rocketVel;
}
/*
=================================================================
Gets the rocket velocity
=================================================================
*/
int cPlayer::getRocketVelocity()
{
	return rocketVelocity;
}
/*
=================================================================
Sets the move value for the rocket
=================================================================
*/
void cPlayer::setRocketMoveV(int rocketMoveV)
{
	moveV = rocketMoveV;
}

void cPlayer::setRocketMoveH(int rocketMoveH)
{
	moveH = rocketMoveH;
}
/*
=================================================================
Gets the rocket move value
=================================================================
*/
int cPlayer::getRocketMoveV()
{
	return moveV;
}

int cPlayer::getRocketMoveH()
{
	return moveH;
}
