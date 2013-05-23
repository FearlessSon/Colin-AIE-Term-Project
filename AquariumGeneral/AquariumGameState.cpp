/*
	AquariumGameState.cpp

	Code file for the AquariumCreature class, for use in the Aquarium project.

	Colin Ferguson, 2013
*/

// Preprocessor declarations
#include "AquariumGameState.h"

// Class constructor
AquariumGameState::AquariumGameState()
{

}

// Returns i from the fish school
LittleFish AquariumGameState::GetLittleFish(int i)
{
	return FishSchool[i];
}

// Returns the big fish
BigFish AquariumGameState::GetBigFish(void)
{
	return LargeFish;
}

// Passes in a little fish and assigns it to i
void AquariumGameState::SetLittleFish(int i, LittleFish new_fish)
{
	FishSchool[i] = new_fish;		// Copies by implicit assignment operator
}

// Passes in a big fish and sets it
void AquariumGameState::SetBigFish(BigFish new_fish)
{
	LargeFish = new_fish;			// Copies by implicit assaignment operator
}

// Destructor
AquariumGameState::~AquariumGameState()
{

}