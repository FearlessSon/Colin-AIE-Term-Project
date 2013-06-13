/*
	AquariumAStar.h

	Header file for the AStar class, for use in the Aquarium project.

	Colin Ferguson, 2013
*/

// Header protection
#ifndef AQUARIUM_ASTAR
#define AQUARIUM_ASTAR

#include "AquariumVector2.h"

// Node class for forming linked lists
class Node
{
public:
	Node();						// Class constructor
	~Node();					// Class destructor

	Vector2	data;				// Data held in node
	Node*	next;				// Next node in the chain
};

// Linked list class
class LinkedList
{
public:
	LinkedList();				// Class constructor
	~LinkedList();				// Class destrctor

	Node*	start;				// Pointer to the first node in the list

	void add(Vector2 data);		// Adds a node to the list with data
	void erase(Vector2 data);	// Removes a node from the list with matching data
	bool check(Vector2 data);	// Returns true if data is in list
	bool empty(void);			// Returns true if nothing is in the list
};

// AStar class for pathfinding
class AStar
{
	LinkedList	ClosedSet;
	LinkedList	OpenSet;

	ReconstructPath
};

#endif