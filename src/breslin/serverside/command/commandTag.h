#ifndef COMMANDTAG_H
#define COMMANDTAG_H

/******************************************************
*				INCLUDES
********************************************************/
//parent
#include "command.h"

//math
#include "../../math/vector3D.h"

/******************************************************
*				FORWARD DECLARATIONS
********************************************************/

/******************************************************
*				CLASS
********************************************************/
class CommandTag : public Command
{
public:
CommandTag();
~CommandTag();

/******************************************************
*				VARIABLES
********************************************************/
int mTextCode;

/******************************************************
*				METHODS
********************************************************/
};
#endif
