/*
	AquariumCreature.h

	Header file for the AquariumCreature class, for use in the Aquarium project.

	Colin Ferguson, 2013
*/

// Header protection
#ifndef AQUARIUM_CREATURE
#define AQUARIUM_CREATURE

// Constant declarations
const int LittleFishTotalVerticies = 8;
const int BigFishTotalVerticies = 10;

// Enumeration declaration
enum CreatureType
{
	Little_Fish,
	Big_Fish
};

// AquariumCreatue class
// This class holds information for various creatures in the simulation
class AquariumCreature
{
public:
	AquariumCreature();						// Class constructor
	void	SetPos(int x, int y);			// Assigns current position to x and y
	void	GetPos(int *x, int *y);			// Assigns x and y arguments to current position
	int		GetXPos(void);					// Returns x position
	int		GetYPos(void);					// Returns y position
	void	GetPrevPos(int *x, int *y);		// Assigns x and y arguments to previous position
	int		GetPrevXPos(void);				// Returns previous x position
	int		GetPrevYPos(void);				// Returns previous y position
	int		GetTotalVertices(void);			// Returns total number of verticies in the model
	void	SetRotation(float r);			// Assigns r argument to the rotation
	float	GetRotation(void);				// Returns the rotation of the creature
	CreatureType GetDrawType(void);			// Returns the type of creature for drawing
	virtual float GetVertex(int x, int y)=0;// Returns the x or y value of the specified vertex
	~AquariumCreature();					// Class destructor
protected:
	int		xpos, ypos;						// The x and y position of the creature
	int		prevxpos, prevypos;				// The previous x and y position of the creature
	int		total_verticies;				// The total number of verticies in the creature
	float	rotation;						// Rotation of the creature
	CreatureType draw_type;					// The type of creature for drawing functions
};

// LittleFish class
// This class holds information for little fish (swim in schools) in the simulation
// Derived from the AquariumCreature class
class LittleFish : public AquariumCreature
{
public:
	LittleFish();								// Class constructor
	float GetVertex(int x, int y);				// Returns the x or y value of the specified vertex
	~LittleFish();								// Class destructor
private:
	float Verticies[LittleFishTotalVerticies][2];	// Contains drawing verticies for the creature
};

// BigFish class
// This class holds information for big fish (swim alone) in the simulation
// Derived from the AquariumCreature class
class BigFish : public AquariumCreature
{
public:
	BigFish();									// Class constructor
	float GetVertex(int x, int y);				// Returns the x or y value of the specified vertex
	~BigFish();									// Class destructor
private:
	float Verticies[BigFishTotalVerticies][2];	// Contains drawing verticies for the creature
};

#endif