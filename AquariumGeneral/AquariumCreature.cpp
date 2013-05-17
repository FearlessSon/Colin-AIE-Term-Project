/*
	AquariumCreature.cpp

	Code file for the AquariumCreature class, for use in the Aquarium project.

	Colin Ferguson, 2013
*/

// Preprocessor declarations
#include "AquariumCreature.h"

// Class constructor
AquariumCreature::AquariumCreature()
{
	// Assign all values to default
	xpos = 0;
	ypos = 0;
	prevxpos = 0;
	prevypos = 0;
	rotation = 0;
}

// Assigns current position to x and y
void AquariumCreature::SetPos(int x, int y)
{
	// Set previous position
	prevxpos = xpos;
	prevypos = ypos;

	// Set x and y position to arguments
	xpos = x;
	ypos = y;
}

// Assigns x and y arguments to current position
void AquariumCreature::GetPos(int *x, int *y)
{
	// Set x and y to x and y positions
	*x = xpos;
	*y = ypos;
}

// Returns x position
int AquariumCreature::GetXPos(void)
{
	// Return x position
	return xpos;
}

// Returns y position
int AquariumCreature::GetYPos(void)
{
	// Return y position
	return ypos;
}

// Assigns x and y arguments to previous position
void AquariumCreature::GetPrevPos(int *x, int *y)
{
	// Assign x and y arguments
	*x = prevxpos;
	*y = prevypos;
}

// Returns previous x position
int AquariumCreature::GetPrevXPos(void)
{
	// Return previous x position
	return prevxpos;
}

// Returns previous y position
int AquariumCreature::GetPrevYPos(void)
{
	// Return previous y position
	return prevypos;
}

// Returns total number of verticies in the model
int AquariumCreature::GetTotalVertices(void)
{
	// Return total number of verticies
	return total_verticies;
}

// Assigns r argument to the rotation
void AquariumCreature::SetRotation(float r)
{
	// Assign r to rotation
	rotation = r;
}

// Returns the rotation of the creature
float AquariumCreature::GetRotation(void)
{
	// Return the value of rotation
	return rotation;
}

// Returns the type of the creature for drawing
CreatureType AquariumCreature::GetDrawType(void)
{
	// Return the creature's type
	return draw_type;
}

// Class destructor
AquariumCreature::~AquariumCreature()
{

}

// Class constructor
LittleFish::LittleFish()
{
	// Assign all values to default
	xpos = 0;
	ypos = 0;
	prevxpos = 0;
	prevypos = 0;
	rotation = 0;
	total_verticies = LittleFishTotalVerticies;

	// Define verticies
	Verticies[0][0] = 1.0;	Verticies[0][1] = 0.0;
	Verticies[1][0] = 0.0;	Verticies[1][1] = -1.0;
	Verticies[2][0] = -0.5;	Verticies[2][1] = 0.0;
	Verticies[3][0] = -1.0;	Verticies[3][1] = -1.0;
	Verticies[4][0] = -1.0;	Verticies[4][1] = 1.0;
	Verticies[5][0] = -0.5;	Verticies[5][1] = 0.0;
	Verticies[6][0] = 0.0;	Verticies[6][1] = 1.0;
	Verticies[7][0] = 1.0;	Verticies[7][1] = 0.0;
}

// Returns the x or y value of the specified vertex
float LittleFish::GetVertex(int x, int y)
{
	// Return the indicated vertex
	return Verticies[x][y];
}

// Class destructor
LittleFish::~LittleFish()
{

}

// Class constructor
BigFish::BigFish()
{
	// Assign all values to default
	xpos = 0;
	ypos = 0;
	prevxpos = 0;
	prevypos = 0;
	rotation = 0;
	total_verticies = BigFishTotalVerticies;

	// Define verticies
	Verticies[0][0] = 2.0;	Verticies[0][1] = 0.0;
	Verticies[1][0] = 1.0;	Verticies[1][1] = -2.0;
	Verticies[2][0] = -1.0;	Verticies[2][1] = -2.0;
	Verticies[3][0] = -1.5;	Verticies[3][1] = 0.0;
	Verticies[4][0] = -2.0;	Verticies[4][1] = -2.0;
	Verticies[5][0] = -2.0;	Verticies[5][1] = 2.0;
	Verticies[6][0] = -1.5;	Verticies[6][1] = 0.0;
	Verticies[7][0] = -1.0;	Verticies[7][1] = 2.0;
	Verticies[8][0] = 1.0;	Verticies[8][1] = 2.0;
	Verticies[9][0] = 2.0;	Verticies[9][1] = 0.0;
}

// Returns the x or y value of the specified vertex
float BigFish::GetVertex(int x, int y)
{
	// Return the indicated vertex
	return Verticies[x][y];
}

// Class destructor
BigFish::~BigFish()
{

}