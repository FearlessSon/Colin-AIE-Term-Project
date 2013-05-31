// Preprocessor declarations
#include "AquariumMessageTypes.h"
#include "AquariumGameState.h"
#include "AquariumCreature.h"
#include "AquariumRakNet.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include <math.h>
#include <stdio.h>
#include <Winuser.h>

// Constant declarations
const int screenH = 640;
const int screenV = 480;

// Global declarations
static RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
RakNet::RakNetGUID ServerID;
char NetworkMessage[1000];

// Function prototypes
SDL_Surface *InitializeSDL();										// Sets up SDL
bool InitializeOpenGL();											// Sets up OpenGL
void GetVertex(float *x, float *y, AquariumCreature *Fish, int i);	// Gets a vertex of a given Creature
inline void LoopVertexDraw(int bottom, int top, float x, float y, AquariumCreature *Fish);	// Loops around getting vertexes and doing GL calls
void DrawFish(AquariumCreature *Fish);								// Draws a fish passed into it as an argument
void DrawBigFish(AquariumCreature *Fish);							// Draws a big fish passed into it as an argument
void DrawLittleFish(AquariumCreature *Fish);						// Draws a small fish passed into it as an argument
void StartNetwork(AquariumGameState *game_state);					// Begins the network connection
DWORD WINAPI NetworkLoop(void *data);								// Passes information to server, listens for information from server
void GameLoop(AquariumGameState *game_state);						// Main game loop

int main( int argc, char* args[] )
{    
	AquariumGameState game_state;			// Create game state

	SDL_Surface* screen = InitializeSDL();	// Create screen surface
	InitializeOpenGL();						// Set up OpenGL

	StartNetwork(&game_state);									// Activate network
	CreateThread(NULL, 0, NetworkLoop, &game_state, 0, NULL);	// Send and recieve information from server in new thread
	GameLoop(&game_state);										// Main program loop

    SDL_Quit();						// Quit SDL
    return 0;						// Quit program
}

// Sets up SDL
SDL_Surface *InitializeSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return NULL;
	}
 
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	return SDL_SetVideoMode(640, 480, 16, SDL_OPENGL);
}

// Sets up OpenGL
bool InitializeOpenGL()
{
	glEnable( GL_TEXTURE_2D );
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glViewport( 0, 0, 640, 480 );
	glClear( GL_COLOR_BUFFER_BIT );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho(0.0f, 640, 480, 0.0f, -1.0f, 1.0f);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	return true;
}

// Gets a vertex of a given Creature
void GetVertex(float *x, float *y, AquariumCreature *Fish, int i)
{
	*x = Fish->GetVertex(i, 0);
	*y = Fish->GetVertex(i, 1);
}

// Loops around getting vertexes and doing GL calls
inline void LoopVertexDraw(int bottom, int top, float x, float y, AquariumCreature *Fish)
{
	// Loop through the range specified
	for (bottom; bottom < top; bottom++)
	{
		GetVertex(&x, &y, Fish, bottom);
		glVertex3f(x, y, 0);
	}
	// Reset x and y to zero
	x = 0;
	y = 0;
}

// Draws a fish passed into it as an argument
void DrawFish(AquariumCreature *Fish)
{
	if (Fish->GetDrawType() == Big_Fish)
		DrawBigFish(Fish);
	if (Fish->GetDrawType() == Little_Fish)
		DrawLittleFish(Fish);
}

// Draws a big fish passed into it as an argument
void DrawBigFish(AquariumCreature *Fish)
{
	// Initialize local variables
	float x = 0;
	float y = 0;

	// Set color
	glColor3i(204, 102, 0);	// Brownish-yellow

	// Drawing fish tail
	glBegin(GL_POLYGON);
	LoopVertexDraw(3, 7, x, y, Fish);
	glEnd();

	// Drawing fish body
	glBegin(GL_POLYGON);
	LoopVertexDraw(0, 3,  x, y, Fish);
	LoopVertexDraw(3, 10, x, y, Fish);
	glEnd();
}

// Draws a small fish passed into it as an argument
void DrawLittleFish(AquariumCreature *Fish)
{
	// Initialize local variables
	float x = 0;
	float y = 0;

	// Set color
	glColor3i(0, 204, 204);	// Teal

	// Drawing fish tail
	glBegin(GL_POLYGON);
	LoopVertexDraw(2, 6, x, y, Fish);
	glEnd();

	// Drawing fish body
	glBegin(GL_POLYGON);
	LoopVertexDraw(0, 2,  x, y, Fish);
	LoopVertexDraw(6, 8, x, y, Fish);
	glEnd();
}

// Main game loop
void GameLoop(AquariumGameState *game_state)
{
	bool Game_Running = true;		// Stays true as long as the game is running
	while (Game_Running == true)	// Begin the loop
	{
		for (int i = 0; i < SchoolSize; i++)			// Iterate through the fish school
			DrawFish(&game_state->GetLittleFish(i));	// Draw each little fish
		DrawFish(&game_state->GetBigFish());			// Draw the big fish

		SDL_Delay(10);									// Brief delay to give the OS a little breathing room
		SDL_GL_SwapBuffers();							// Redraw the screen
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
			strcpy(NetworkMessage, (const char *) s+1);
			if ((int) packet->data[1] != (SchoolSize+1))	// Set position of one of the little fish
				game_state->FishSchool[packet->data[1]].SetPos((float) packet->data[2], (float) packet->data[3]);
			if ((int) packet->data[1] == (SchoolSize+1))	// Set the position of the large fish
				game_state->LargeFish.SetPos((float) packet->data[2], (float) packet->data[3]);
		}
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