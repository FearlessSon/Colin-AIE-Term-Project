// Preprocessor declarations
#include "AquariumMessageTypes.h"
#include "AquariumGameState.h"
#include "AquariumRakNet.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include <math.h>
#include <stdio.h>
#include <Winuser.h>
//#include <tchar.h>

// Constant declarations

// Global declarations
static RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
char NetworkMessage[1000];
RakNet::RakNetGUID PlayerID;

// Function prototypes
void StartNetwork(AquariumGameState *game_state);					// Begins the network connection
DWORD WINAPI NetworkLoop(void *data);								// Passes information to server, listens for information from server
void GameLoop(AquariumGameState *game_state);						// Main game loop

int main(int argc, char *argv[])
{
	AquariumGameState game_state;	// Create game state

	StartNetwork(&game_state);									// Activate network
	CreateThread(NULL, 0, NetworkLoop, &game_state, 0, NULL);	// Send and recieve information from server in new thread
	GameLoop(&game_state);										// Main program loop

	return 0;    
}

// Main game loop
void GameLoop(AquariumGameState *game_state)
{
	bool Game_Running = true;		// Stays true as long as the game is running
	while (Game_Running == true)	// Begin the loop
	{
		
	}
	return;
}

// Loop for processing network packets
void StartNetwork(AquariumGameState *game_state)
{
	char str[512];												// Holds console input
	RakNet::SocketDescriptor sd;								// Creates a RakNet socket
	peer->Startup(1, &sd, 1);									// Start up the network
	printf("enter server IP or <CR> for localHost\n");			// Prompt for server address
	gets(str);													// Take in service address
	if (!str[0])												// If no address is entered...
		strcpy(str, "127.0.0.1");								// ... then default to the local host
	printf("attempting connection\n");
	RakNet::ConnectionAttemptResult result = peer->Connect(str, SERVER_PORT, 0, 0);		// Attempt to connect
	char *reasons[] = {"attempting", "invalid parameter", "bad domain name",			// Get the result of the attempt
		               "already connected", "already attempting", "security failure"};
	if (result != 0)											// Was the attempt a failure?
	{
		printf("CONNECTION FAILED (%s)\n", reasons[result]);	// ... then announce it so... 
		return;													// ... and quit in a huff
	}
}

// Passes information to server, listens for information from server
DWORD WINAPI NetworkLoop(void *data)
{
	AquariumGameState *game_state = static_cast<AquariumGameState *>(data);	// Cast the game state object so we can use it
	for (RakNet::Packet *packet = peer->Receive(); packet; packet = peer->Receive())	// Keeps looping and listening for packets
	{
		unsigned char *s = packet->data;					// Pointer to the RakNet packet data, used for accessing network messages
		if (*s < ID_USER_PACKET_ENUM)						// If just a plain text message...
			PrintSystemMessage(packet);						// ... then output it to the console
		if (*s == ID_USER_MESSAGE)							// If... some other kind of message...
			strcpy(NetworkMessage, (const char *) s+1);		// ... then send it?
		if (*s == OUTPUT_UPDATE_POSITION)					// If message is to update a creature's position
		{
		/*	strcpy(NetworkMessage, (const char *) s+1);
			if ((int) packet->data[1] != (SchoolSize+1))	// Set position of one of the little fish
				game_state->FishSchool[packet->data[1]].SetPos((float) packet->data[2], (float) packet->data[3]);
			if ((int) packet->data[1] == (SchoolSize+1))	// Set the position of the large fish
				game_state->LargeFish.SetPos((float) packet->data[2], (float) packet->data[3]);
*/		}
		if (*s == OUTPUT_BUBBLES)							// If  message is to create some bubbles
		{
			strcpy(NetworkMessage, (const char *) s+1);
			// To be filled...
		}
		if (*s == OUTPUT_FEED_FISH)							// If message is to feed fish
		{
			strcpy(NetworkMessage, (const char *) s+1);
			// To be filled...
		}
		if (*s == OUTPUT_TAP_GLASS)							// If message is to tap glass
		{
			strcpy(NetworkMessage, (const char *) s+1);
			// To be filled...
		}
		peer->DeallocatePacket(packet);
	}
	return 1;
}