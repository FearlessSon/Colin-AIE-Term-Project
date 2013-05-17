// Preprocessor declarations
#include "AquariumMessageTypes.h"
#include "AquariumGameState.h"
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

// Function prototypes
bool InitializeSDL(SDL_Surface* screen);							// Sets up SDL
bool InitializeOpenGL();											// Sets up OpenGL
void GetVertex(float *x, float *y, AquariumCreature *Fish, int i);	// Gets a vertex of a given Creature
inline void LoopVertexDraw(int bottom, int top, float x, float y, AquariumCreature *Fish);	// Loops around getting vertexes and doing GL calls
void DrawFish(AquariumCreature *Fish);								// Draws a fish passed into it as an argument
void DrawBigFish(AquariumCreature *Fish);							// Draws a big fish passed into it as an argument
void DrawLittleFish(AquariumCreature *Fish);						// Draws a small fish passed into it as an argument
void NetworkLoop(AquariumGameState *game_state);					// Loop for processing network packets
DWORD WINAPI GameLoop(AquariumGameState *game_state);				// Main game loop

int main( int argc, char* args[] )
{    
	// Create screen surface
	SDL_Surface* screen = NULL;
	SDL_Surface* hello = NULL;

	InitializeSDL(screen);			// Set up SDL
	SDL_Flip(screen);				// Update screen
	InitializeOpenGL();				// Set up OpenGL

	AquariumGameState *game_state = static_cast<AquariumGameState *>(data);

    SDL_Delay( 2000 );				// Pause

	SDL_FreeSurface(hello);			// Free the loaded image
    SDL_Quit();						// Quit SDL
    
    return 0;
}

// Sets up SDL
bool InitializeSDL(SDL_Surface* screen)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}
 
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	screen = SDL_SetVideoMode(640, 480, 16, SDL_OPENGL);

	return true;
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
DWORD WINAPI GameLoop(AquariumGameState *game_state)
{
	bool Game_Running = true;		// Stays true as long as the game is running
	while (Game_Running == true)	// Begin the loop
	{

	}
	return;
}

// Loop for processing network packets
void NetworkLoop(AquariumGameState *game_state)
{
	RakNet::SocketDescriptor sd;
	peer->Startup(1, &sd, 1);
	printf("enter server IP or <CR> for localHost\n");
	gets(str);
	if (!str[0])
		strcpy(str, "127.0.0.1"); // localHost
	printf("attempting TicTacToe connection\n");
	RakNet::ConnectionAttemptResult result = peer->Connect(str, SERVER_PORT, 0, 0);
	char *reasons[] = {"attempting", "invalid parameter", "bad domain name",
		               "already connected", "already attempting", "security failure"};
	if (result != 0) {
		printf("CONNECTION FAILED (%s)\n", reasons[result]);
		return;
	}
    CreateThread(NULL, 0, GameLoop, NULL, 0, NULL);
}