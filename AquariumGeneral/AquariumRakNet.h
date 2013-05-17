#ifndef Aquarium_RakNet_Header
#define Aquarium_RakNet_Header

#include "RakPeerInterface.h"
#include "SDL.h"

RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();

void	PrintSystemMessage(RakNet::Packet *packet);
void	SendPlayerMessage(char *msg, RakNet::AddressOrGUID guid);
void	ProcessPacketsReceivedByServer();
void	ProcessPacketsReceivedByClient(SDL_Surface* screen);

#endif