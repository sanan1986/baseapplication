#ifndef GAME_H
#define GAME_H

/***************************************
*   		INCLUDES
***************************************/
//standard library
#include <vector>

/***************************************
*   		FORWARD DECLARATIONS
***************************************/
class Application;
class ByteBuffer;
class ShapeDynamic;

class Game 
{
public:
	
	Game(Application* application, const char* serverIP, int serverPort);
	~Game();

/***************************************
*   		MEMBER VARIABLES
***************************************/
//application
Application* mApplication;

//Shapes
std::vector<ShapeDynamic*> mShapeVector;	 //all shapes in the client world
std::vector<ShapeDynamic*> mShapeGhostVector;	 //all shapes in the client world's ghost 

	
/***************************************
*			          METHODS
***************************************/
//shape
virtual void          addShape       (bool b, ByteBuffer* byteBuffer) { }
        void          removeShape    (ByteBuffer* byteBuffer);
        ShapeDynamic* getShapeDynamic(int id);

void interpolateTick();

};

#endif
