/*
	AquariumVector2.h

	Header file for the Vector2 class, for use in the Aquarium project.

	Colin Ferguson, 2013
*/

// Header protection
#ifndef AQUARIUM_VECTOR2
#define AQUARIUM_VECTOR2

#include <math.h>

class Vector2
{
public:
	Vector2(void);
	Vector2(float X, float Y);
	~Vector2(void);
	float Length();
	Vector2 Normalize();
	float X,Y;
};

#endif