/*
	AquariumVector2.cpp

	Source file for the Vector2 class, for use in the Aquarium project.

	Colin Ferguson, 2013
*/

#include "AquariumVector2.h"
 
Vector2::Vector2(void)
{

}
 
Vector2::Vector2(float X, float Y)
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
    Vector2 vector;
    float length = this->Length();
 
    if(length != 0)
	{
        vector.X = X/length;
        vector.Y = Y/length;
    }
 
    return vector;
}
 
Vector2::~Vector2(void)
{

}