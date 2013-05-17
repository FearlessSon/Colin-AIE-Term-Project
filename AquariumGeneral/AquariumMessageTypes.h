#ifndef Aquarium_Message_Types
#define Aquarium_Message_Types

#include "MessageIdentifiers.h"

enum MessageTypes {
	OUTPUT_UPDATE_POSITION,	// object, x, y
	OUTPUT_BUBBLES,			// x, y
	OUTPUT_FEED_FISH,		// x, y
	OUTPUT_TAP_GLASS,		// x, y
	INPUT_FEED_FISH,		// x, y
	INPUT_TAP_GLASS,		// x, y
	ID_USER_MESSAGE			// id, char *msg
};

#endif
