//  TikRakToe.cpp: tic-tac-toe using RakNet and GLUT
//  see www.jenkinssoftware.com/raknet/manual/tutorial.html
//  Jules Bloomenthal, 2013

#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include <math.h>
#include <stdio.h>
#include <GL/glut.h>
#include <Winuser.h>

// server and clients

RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();

#define MAX_PLAYERS 2
#define SERVER_PORT 60000

RakNet::RakNetGUID playerIDs[MAX_PLAYERS];
int nPlayers = 0, playerToMove = 0;

enum MessageTypes {
	ID_GRID_DATA = ID_USER_PACKET_ENUM,     // id and 9 bytes of tictactoe grid
	ID_MOVE_DATA,                           // id, x, y
	ID_USER_MESSAGE                         // id, char *msg
};

void PrintSystemMessage(RakNet::Packet *packet) {
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
}

void SendPlayerMessage(char *msg, RakNet::AddressOrGUID guid) {
	char buf[100];
	buf[0] = ID_USER_MESSAGE;
	strcpy(buf+1, msg);
	peer->Send(buf, strlen(buf)+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, guid, false);
	    // +1 for null character
}

// the game

int     gameWidth = 400, gameHeight = 450;  // size in pixels
double  wht[] = {1,1,1}, red[] = {1,0,0};   // colors

#define RES 36                              // # points in circle for 'O'
#define RAD 40                              // radius of circle in pixels

enum State {S_X, S_O, S_Unset};

class Game {
public:
	State grid[3][3];                       // the tictactoe board
	bool done;                              // game over?
	char message[1000];                     // message to user from server
	int x1, x4, x2, x3;                     // location (in pixels) of grid cells
    double xCirc[RES], yCirc[RES];          // to draw 'O'
    Game( ) {                               // board of given size, margin from app edge
        int margin = 25;                    // distance from board edge
		int size = gameWidth-2*margin;      // size of grid
		x1 = margin;                        // leftmost or lowermost grid edge
        x2 = x1+size/3;                     // left or bottom of middle cell
        x3 = x2+size/3;                     // left or bottom of third cell
        x4 = x1+size;                       // rightmost or uppermost grid edge
        for (int k = 0; k < RES; k++) {     // initialize points for 'O'
            double a = (2.*3.141592*k)/RES;
            xCirc[k] = RAD*cos(a);
            yCirc[k] = RAD*sin(a);
        }
        for (int k = 0; k < 3*3; k++)       // initialize grid
            grid[k/3][k%3] = S_Unset;
        done = false;
		message[0] = 0;
    }
	void ColWidth(double *color, double width) {
		glColor3dv(color);
		glPointSize(width);
		glLineWidth(width);
	}
	void Line(int x1, int y1, int x2, int y2) {
		// draw line from (x1,y1) to (x2,y2) with round endpoints
		glBegin(GL_LINES);
		glVertex2d(x1, y1);
		glVertex2d(x2, y2);
		glEnd();
		glBegin(GL_POINTS);
		glVertex2d(x1, y1);
		glVertex2d(x2, y2);
		glEnd();
	}
	int XFromInt(int i) {
        // what is leftmost edge of cell(i)?
        return (i==0? x1+x2 : i==1? x2+x3 : x3+x4)/2;
    }
    int IntFromX(int x) {
        // to which cell does x belong?
        int i = floor((double)(x-x1)/(x2-x1));
        return i < 0? 0 : i > 2? 2 : i;
    }
	bool TestForWin() {
		for (int k = 0; k < 3; k++)
			if (TestForWin(0, k, 2, k) || TestForWin(k, 0, k, 2))
				return true;
		return TestForWin(0, 0, 2, 2) || TestForWin(0, 2, 2, 0);
	}
    bool TestForWin(int i1, int j1, int i2, int j2) {
        // is row, column or diagonal from (i1, j1) to (i2, j2) same state?
        State s = grid[(i1+i2)/2][(j1+j2)/2];
        return s != S_Unset && grid[i1][j1] == s && grid[i2][j2] == s;
    }
    void DrawIfWin(int i1, int j1, int i2, int j2) {
        // draw slash if winning row, column or diagonal
        if (TestForWin(i1, j1, i2, j2))
			Line(XFromInt(i1), XFromInt(j1), XFromInt(i2), XFromInt(j2));
    }
	void DrawMark(State s, int x, int y) {
		// mark X or O centered at pixel(x,y)
		if (s == S_O) {
			glBegin(GL_POINTS);
			for (int k = 0; k < RES; k++)
				glVertex2d(x+xCirc[k], y+yCirc[k]);
			glEnd();
			glBegin(GL_LINE_LOOP);
			for (int k = 0; k < RES; k++)
				glVertex2d(x+xCirc[k], y+yCirc[k]);
			glEnd();
		}
		if (s == S_X) {
			Line(x-RAD, y-RAD, x+RAD, y+RAD);
			Line(x-RAD, y+RAD, x+RAD, y-RAD);
		}
	}
	void Draw() {
		// clear display, disable depth buffer
		glClearColor(0, 0, 0, 1.);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		// anti-aliasing
		glEnable(GL_BLEND);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POINT_SMOOTH);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// default view
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		// tictactoe hash mark
		ColWidth(wht, 6);
		Line(x2, x1, x2, x4);
		Line(x3, x1, x3, x4);
		Line(x1, x2, x4, x2);
		Line(x1, x3, x4, x3);
		// Xs and Os
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				DrawMark(grid[i][j], XFromInt(i), XFromInt(j));
		// winner slash?
		ColWidth(red, 12);
		for (int k = 0; k < 3; k++) {
			DrawIfWin(0, k, 2, k);
			DrawIfWin(k, 0, k, 2);
		}
		DrawIfWin(0, 0, 2, 2);
		DrawIfWin(0, 2, 2, 0);
		// position and display message
		glScaled(.3, .3, 1.);
		glTranslated(80, 1350, 0);
		ColWidth(wht, 3);
		for (const char *c = message; *c; c++)
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
	}
} game;

// GLUT callbacks and startup

void Draw(void) {
	game.Draw();
    glFlush();
	glutSwapBuffers();
}

void MouseButton(int butn, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		// use upward facing y
		y = gameHeight-y;
		// send message of 3 bytes (ID, x, y) to server
		char message[] = {ID_MOVE_DATA, (unsigned char) game.IntFromX(x), (unsigned char) game.IntFromX(y)};
		peer->Send(message, 3, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		// clear and redisplay game message
		game.message[0] = 0;
		glutPostRedisplay();
	}
}

DWORD WINAPI StartGame(void *data) {
	// Game *game = static_cast<Game *>(data);
	// init game window
	char *name = "TicRakToe";
	int ac = 1, xPosition = FindWindow(NULL, "TicRakToe") == NULL? 950 : 1400;
    glutInit(&ac, &name);
	glutInitWindowSize(gameWidth, gameHeight);
    glutInitWindowPosition(xPosition, 550);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow(name);
    // set default viewport and projection
    glViewport(0, 0, gameWidth, gameHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, gameWidth, 0, gameHeight, 0, -1);
    // set callbacks
    glutDisplayFunc(Draw);
	glutMouseFunc(MouseButton);
    // begin event handling, display, and updates
    glutMainLoop();
	return 0;
}

// packet processing

void ProcessPacketsReceivedByServer() {
	for (RakNet::Packet *packet = peer->Receive(); packet; packet = peer->Receive()) {
		unsigned char c = packet->data[0];
		if (c < ID_USER_PACKET_ENUM)
			PrintSystemMessage(packet);
		if (c == ID_NEW_INCOMING_CONNECTION || c == ID_REMOTE_NEW_INCOMING_CONNECTION) {
			playerIDs[nPlayers++] = packet->guid;
			if (nPlayers == 2)
				SendPlayerMessage("your move", playerIDs[0]);
		}
		if (c == ID_USER_MESSAGE) {
			// send message to other user
			char *s = (char *) packet->data;
			peer->Send(s, strlen(s)+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, true);
		}
		if (c == ID_MOVE_DATA && !game.done) {
			if (playerIDs[playerToMove] != packet->guid)
				SendPlayerMessage("not your turn", packet->guid);
			else {
				State s = playerIDs[0] == packet->guid? S_X : S_O;
				int i = (unsigned char) packet->data[1];
				int j = (unsigned char) packet->data[2];
				if (game.grid[i][j] == S_Unset) {
					// upgrade game grid
    				game.grid[i][j] = s;
					// broadcast new grid
					char message[100];
					message[0] = ID_GRID_DATA;
					memcpy(message+1, game.grid, sizeof(game.grid));
					peer->Send(message, 1+sizeof(game.grid), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
					// test game over
					game.done = game.TestForWin();
					if (game.done)
						for (int i = 0; i < 2; i++)
							SendPlayerMessage("game over", playerIDs[i]);
					else {
						// change turns
						playerToMove = (playerToMove+1)%2;
						SendPlayerMessage(game.done? "" : "your move", playerIDs[playerToMove]);
					}
				}
				else
					// notify illegal move
					SendPlayerMessage("bad move", packet->guid);
			}
		}
		peer->DeallocatePacket(packet);
	}
}

void ProcessPacketsReceivedByClient() {
	for (RakNet::Packet *packet = peer->Receive(); packet; packet = peer->Receive()) {
		unsigned char *s = packet->data;
		if (*s < ID_USER_PACKET_ENUM)
			PrintSystemMessage(packet);
		if (*s == ID_USER_MESSAGE) {
			strcpy(game.message, (const char *) s+1);
			glutPostRedisplay();
		}
		if (*s == ID_GRID_DATA) {
			memcpy(game.grid, s+1, sizeof(game.grid));
			glutPostRedisplay();
		}
		peer->DeallocatePacket(packet);
	}
}

// the application

void main(int ac, char **av) {
	// query user
	printf("(c)lient or (s)erver?\n");
	char str[512];
	gets(str);
	bool server = tolower(str[0]) == 's';
	// start server or client
	if (server) {
		RakNet::SocketDescriptor sd(SERVER_PORT, 0);
		peer->Startup(MAX_PLAYERS, &sd, 1);
		printf("starting TicTacToe server\n");
		peer->SetMaximumIncomingConnections(MAX_PLAYERS);
	}
	else {
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
    	CreateThread(NULL, 0, StartGame, NULL, 0, NULL);
	}
	// process packets
	while (1)
		if (server)
    		ProcessPacketsReceivedByServer();
		else
			ProcessPacketsReceivedByClient();
	// cleanup (move to destructor?)
	RakNet::RakPeerInterface::DestroyInstance(peer);
}
