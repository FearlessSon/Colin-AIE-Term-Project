/*
	AquariumVector2.cpp

	Source file for the Vector2 class, for use in the Aquarium project.

	Colin Ferguson, 2013
*/

#include "AquariumVector2.h"

// Empty class constructor
Vector2::Vector2(void)
{
	// Set default values to zero
	Set(0.0, 0.0);
}

// Assignment class constructor
Vector2::Vector2(float X, float Y)
{
	// Set the arguments of this vector
    Set(X, Y);
}

// Sets values to X and Y
void Vector2::Set(float X, float Y)
{
	this->X = X;
	this->Y = Y;
}

// Returns the length of the vector
float Vector2::Length()
{
    return sqrt(X * X + Y * Y);
}
 
// Normalizes the vector
Vector2 Vector2::Normalize()
{
    Vector2 vector;					// Holds the value to return
    float length = this->Length();	// Holds the length of the vector
 
    if(length != 0)					// Assuming that the vector is not of zero length
	{
        vector.X = X/length;		// Divide the X value by the total length
        vector.Y = Y/length;		// Divide the Y value by the total length
    }
 
    return vector;					// Return the normalized vector
}

// Class destructor
Vector2::~Vector2(void)
{

}