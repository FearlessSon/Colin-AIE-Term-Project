/*
	AquariumRakNet.h

	Header file for RakNet related functions and globals, for use in the Aquarium project.

	Colin Ferguson, 2013
*/

// Header protection
#ifndef Aquarium_RakNet_Header
#define Aquarium_RakNet_Header

// Preprocessor declarations
#include "RakPeerInterface.h"
#include "SDL.h"

//#define SERVER_PORT 60000
const unsigned int SERVER_PORT = 60000;	// Constant value for the server port

void	PrintSystemMessage(RakNet::Packet *packet);					// Outputs network messages to the console
void	SendPlayerMessage(char *msg, RakNet::AddressOrGUID guid);	// Sends messages from the server to the clients
//void	ProcessPacketsReceivedByServer();							// Processes packets sent from the server to the clients
//void	ProcessPacketsReceivedByClient(SDL_Surface* screen);		// Processes packets send from a client to the server

#endif