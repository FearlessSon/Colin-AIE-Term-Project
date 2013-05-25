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

// Constant declarations
const int SchoolSize = 10;

// AquariumGameState class
// This class holds the state of the game.  It is meant to be passed by
// reference into any function which requires access to the game state.
class AquariumGameState
{
public:
	AquariumGameState();							// Default constructor
	LittleFish	GetLittleFish(int i);				// Returns i from the fish school
	BigFish		GetBigFish(void);					// Returns the big fish
	void SetLittleFish(int i, LittleFish new_fish);	// Passes in a little fish and assigns it to i
	void SetBigFish(BigFish new_fish);				// Passes in a big fish and sets it
	~AquariumGameState();							// Destructor
//private:
	LittleFish	FishSchool[SchoolSize];				// Array of little fish
	BigFish		LargeFish;							// Large fish
};

#endif