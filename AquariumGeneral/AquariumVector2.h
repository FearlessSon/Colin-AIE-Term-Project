/*
	AquariumVector2.h

	Header file for the Vector2 class, for use in the Aquarium project.

	Colin Ferguson, 2013
*/

// Header protection
#ifndef AQUARIUM_VECTOR2
#define AQUARIUM_VECTOR2

#include <math.h>

// Vector2 class
// This class holds two-dimentional vectors and processing options
class Vector2
{
public:
	Vector2(void);				// Class constructor, with default values
	Vector2(float X, float Y);	// Class constructor, with assignments
	void Set(float X, float Y);	// Sets values to X and Y
	float Length();				// Returns the length of the vector
	Vector2 Normalize();		// Returns a normalized version of this vector
	~Vector2(void);				// Class destructor
	float X,Y;					// X and Y values
};

#endif