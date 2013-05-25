/*
	AquariumRakNet.h

	Header file for RakNet related functions and globals, for use in the Aquarium project.

	Colin Ferguson, 2013
*/

// Preprocessor declarations
#include "AquariumRakNet.h"
#include "AquariumMessageTypes.h"
#include "RakPeerInterface.h"
#include "AquariumMessageTypes.h"
#include "SDL.h"
#include <stdio.h>

// RakNet peer interface declared and initialized
static RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();

// Outputs network messages to the console
void	PrintSystemMessage(RakNet::Packet *packet)
{
	unsigned char c = packet->data[0];
	char *msg = c == ID_REMOTE_DISCONNECTION_NOTIFICATION?  "client disconnected" :
				c == ID_REMOTE_CONNECTION_LOST?			    "client lost connection" :
				c == ID_REMOTE_NEW_INCOMING_CONNECTION?     "client has connected" :
				c == ID_CONNECTION_REQUEST_ACCEPTED?	    "connection request accepted" :
				c == ID_NEW_INCOMING_CONNECTION?		    "connection is incoming" :
				c == ID_NO_FREE_INCOMING_CONNECTIONS?	    "server is full" :
				c == ID_DISCONNECTION_NOTIFICATION?         "client disconnected" :
				c == ID_CONNECTION_LOST?                    "connection lost" : "";
	// print specific message or RakNet identifier
	*msg? printf("%s\n", msg) : printf("message %i: %s\n", c, packet->data);
	return;
}

// Sends messages from the server to the clients
void	SendPlayerMessage(char *msg, RakNet::AddressOrGUID guid)
{
	char buf[100];
	buf[0] = ID_USER_MESSAGE;
	strcpy(buf+1, msg);
	peer->Send(buf, strlen(buf)+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, guid, false);
	    // +1 for null character
	return;
}

// Processes packets send from a client to the server
void	ProcessPacketsReceivedByServer()
{
	for (RakNet::Packet *packet = peer->Receive(); packet; packet = peer->Receive())
	{
		unsigned char c = packet->data[0];
		if (c < ID_USER_PACKET_ENUM)
			PrintSystemMessage(packet);
/*		if (c == ID_NEW_INCOMING_CONNECTION || c == ID_REMOTE_NEW_INCOMING_CONNECTION) {
			playerIDs[nPlayers++] = packet->guid;
			if (nPlayers == 2)
				SendPlayerMessage("your move", playerIDs[0]);
		}
*//*	if (c == ID_USER_MESSAGE) {
			// send message to other user
			char *s = (char *) packet->data;
			peer->Send(s, strlen(s)+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, true);
		}
*/		if (c == INPUT_FEED_FISH)
		{
			
		}
		if (c == INPUT_TAP_GLASS)
		{

		}
		peer->DeallocatePacket(packet);
	}
	return;
}

// Processes packets sent from the server to the clients
void	ProcessPacketsReceivedByClient(SDL_Surface* screen)
{
	for (RakNet::Packet *packet = peer->Receive(); packet; packet = peer->Receive()) {
		unsigned char *s = packet->data;
		if (*s < ID_USER_PACKET_ENUM)
			PrintSystemMessage(packet);
/*		if (*s == ID_USER_MESSAGE)
		{
			strcpy(game.message, (const char *) s+1);
			SDL_Flip(screen);
		}
*/		if (*s == OUTPUT_UPDATE_POSITION)
		{
			
		}
		if (*s == OUTPUT_BUBBLES)
		{

		}
		if (*s == OUTPUT_FEED_FISH)
		{

		}
		if (*s == OUTPUT_TAP_GLASS)
		{

		}
		peer->DeallocatePacket(packet);
	}
	return;
}