#ifndef MAINSCREEN_H
#define MAINSCREEN_H

/***************************************
*   		INCLUDES
***************************************/

#include "../../../statemachine/state.h"

/***************************************
*   		FORWARD DECLARATIONS
***************************************/

class ApplicationBreslin;

class ApplicationMain : public State
{
public:

/***************************************
*   		MEMBER VARIABLES
***************************************/

ApplicationBreslin* mApplication;

ApplicationMain(ApplicationBreslin* application);
~ApplicationMain();

virtual void enter  ();
virtual void execute();
virtual void exit   ();

};

#endif
