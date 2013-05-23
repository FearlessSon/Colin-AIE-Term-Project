#ifndef Aquarium_RakNet_Header
#define Aquarium_RakNet_Header

#include "RakPeerInterface.h"
#include "SDL.h"

const unsigned int SERVER_PORT = 60000;

void	PrintSystemMessage(RakNet::Packet *packet);
void	SendPlayerMessage(char *msg, RakNet::AddressOrGUID guid);
void	ProcessPacketsReceivedByServer();
void	ProcessPacketsReceivedByClient(SDL_Surface* screen);

#endif