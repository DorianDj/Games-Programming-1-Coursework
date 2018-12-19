/*
=================
cRocket.h
- Header file for class definition - SPECIFICATION
- Header file for the Rocket class which is a child of cSprite class
=================
*/
#ifndef _CPLAYER_H
#define _CPLAYER_H
#include "cSprite.h"

class cPlayer : public cSprite
{
private:
	int rocketVelocity;
	int moveV;
	int moveH;

public:
	cPlayer();
	void update(double deltaTime);		// Rocket update method
	void setRocketVelocity(int rocketVel);   // Sets the velocity for the rocket
	int getRocketVelocity();				 // Gets the rocket velocity
	void setRocketMoveV(int rocketMoveV);   // Sets the rocket move Value in the vertical pos
	void setRocketMoveH(int rocketMoveH);   // Sets the rocket move Value in the vertical pos
	int getRocketMoveV();					// Gets the rocket moveV value
	int getRocketMoveH();					// Gets the rocket moveH value
	bool isMovingUp;
	bool isMovingDown;
	bool isMovingLeft;
	bool isMovingRight;
	bool isMovingUL;
	bool isMovingUR;
	bool isMovingDL;
	bool isMovingDR;
	bool isFacingUp;
	bool isFacingDown;
	bool isFacingLeft;
	bool isFacingRight;
	bool canMove;
}; 
#endif