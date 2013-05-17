/*
	AquariumGameState.h

	Header file for the AquariumGameState class, for use in the Aquarium project.

	Colin Ferguson, 2013
*/

// Header protection
#ifndef AQUARIUM_GAME_STATE
#define AQUARIUM_GAME_STATE

// Preprocessor declarations
#include "AquariumCreature.h"

// AquariumGameState class
// This class holds the state of the game.  It is meant to be passed by
// reference into any function which requires access to the game state.
class AquariumGameState
{
public:
	AquariumGameState();		// Default constructor
	~AquariumGameState();		// Destructor
private:
	LittleFish	FishSchool[10];	// Array of little fish
	BigFish		LargeFish;		// Large fish
};

#endif