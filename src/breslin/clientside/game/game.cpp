//header
#include "game.h"

//log
#include "../tdreamsock/dreamSockLog.h"

//applicationBreslin
#include "../application/applicationBreslin.h"

//network
#include "../network/network.h"

//shape
#include "../shape/shape.h"

//bytebuffer
#include "../bytebuffer/byteBuffer.h"

//ability
#include "../ability/rotation/abilityRotation.h"
#include "../ability/move/abilityMove.h"

//state machine
#include "../../statemachine/stateMachine.h"
#include "states/gameGlobal.h"
#include "states/gameInitialize.h"
#include "states/gamePlay.h"
#include "states/gamePause.h"


/***************************************
*			          CONSTRUCTORS
***************************************/
Game::Game(ApplicationBreslin* applicationBreslin)
{
	mApplicationBreslin = applicationBreslin;

	mShapeVector = new std::vector<Shape*>();
	mShapeGhostVector = new std::vector<Shape*>();

	//keys
	mKeyUp = 1;
	mKeyDown = 2;
	mKeyLeft = 4;
	mKeyRight = 8;
	mKeyCounterClockwise = 16;
	mKeyClockwise = 32;

	//input
	mKeyCurrent = 0;
	mKeyLast = 0;

	//time
    	mRunNetworkTime = 0.0f;
	mFrameTimeServer = 0.0f;

	mStateMachine = new StateMachine();
	mGameGlobal = new GameGlobal(this);
	mGameInitialize = new GameInitialize(this);
	mGamePlay = new GamePlay(this);
	mGamePause = new GamePause(this);

	mStateMachine->setGlobalState(mGameGlobal);
	mStateMachine->changeState(mGamePlay);

	//set Camera
	// Position it at 500 in Z direction
    	mApplicationBreslin->getCamera()->setPosition(Ogre::Vector3(0,20,20));
    	// Look back along -Z
    	mApplicationBreslin->getCamera()->lookAt(Ogre::Vector3(0,0,0));
    	mApplicationBreslin->getCamera()->setNearClipDistance(5);
}

Game::~Game()
{
	LogString("Destructor for Game");
	if (mShapeVector)
	{
		for (unsigned int i = 0; i < mShapeVector->size(); i++)
		{
			delete mShapeVector->at(i);
			//delete mShapeGhostVector->at(i);
		}
	}

//ShapeVector::~mShapeVector();
//	ShapeGhostVector::VectormShapeGhostVector();
}
/*********************************
		Update
**********************************/
void Game::processUpdate()
{
	mStateMachine->update();

	for (unsigned int i = 0; i < mShapeVector->size(); i++)
	{
		mShapeVector->at(i)->interpolateTick(mApplicationBreslin->getRenderTime());
	}
}

/*********************************
*		SHAPE
**********************************/
void Game::addShape(ByteBuffer* byteBuffer)
{
	Shape* shape = new Shape(mApplicationBreslin,byteBuffer,false);  //you should just need to call this...

	//ability
	shape->addAbility(new AbilityRotation(shape));
	shape->addAbility(new AbilityMove(shape));

	//put shape and ghost in game vectors so they can be looped and game now knows of them.
	mShapeVector->push_back(shape);
	mShapeGhostVector->push_back(shape->mGhost);
}

void Game::removeShape(ByteBuffer* byteBuffer)
{
	int index = byteBuffer->ReadByte();

	Shape* shape = getShape(index);

	for (unsigned int i = 0; i < mShapeVector->size(); i++)
	{
		if (mShapeVector->at(i) == shape)
		{
			delete mShapeVector->at(i);
			mShapeVector->erase (mShapeVector->begin()+i);
		}
	}
}

Shape* Game::getShape(int id)
{
	Shape* shape = NULL;

	for (unsigned int i = 0; i < mShapeVector->size(); i++)
	{
		Shape* curShape = mShapeVector->at(i);
		if (curShape->mIndex == id)
		{
			shape = curShape;
		}
	}

	if(!shape)
	{
		return NULL;
	}
	else
	{
		return shape;
	}
}

/*********************************
		Network
**********************************/

void Game::checkForByteBuffer()
{
        int type = 0;

        ByteBuffer* byteBuffer = new ByteBuffer();

        while(mApplicationBreslin->mNetwork->checkForByteBuffer(byteBuffer))
        {
                byteBuffer->BeginReading();

                type = byteBuffer->ReadByte();

                switch(type)
                {
                        case mMessageAddShape:
                        	addShape(byteBuffer);
                        break;

                        case mMessageRemoveShape:
                            removeShape(byteBuffer);
                        break;

                        case mMessageFrame:
				if (!mApplicationBreslin->mNetwork->mIgnorePacket)
				{
					readServerTick(byteBuffer);
				}
                        break;
                }
        }
}

void Game::readServerTick(ByteBuffer* byteBuffer)
{
        // Skip sequences
        short sequence = byteBuffer->ReadShort();
        mFrameTimeServer = byteBuffer->ReadByte();
        //LogString("mFrameTimeServer:%f",mFrameTimeServer);

        while (byteBuffer->getReadCount() <= byteBuffer->GetSize())
        {
                mApplicationBreslin->mDetailsPanel->setParamValue(11, Ogre::StringConverter::toString(byteBuffer->GetSize()));

                int id = byteBuffer->ReadByte();

                Shape* shape = NULL;
                shape = getShape(id);

		if (shape)
                {
                        shape->processDeltaByteBuffer(byteBuffer);
                }
                else
                {
                        //LogString("INVALID SHAPE ID");
                }
        }
}

void Game::sendByteBuffer()
{
	mRunNetworkTime += mApplicationBreslin->getRenderTime() * 1000.0f;

    	// Framerate is too high
    	if(mRunNetworkTime > (1000 / 60))
    	{
        	// Build delta-compressed move command
        	int flags = 0;

        	//if key has not been changed return having done nothing 
        	if(mKeyLast != mKeyCurrent)
        	{
                	flags |= mCommandKey;
        	}
		else
		{
			return;
		}

		//create byteBuffer
		ByteBuffer* byteBuffer = new ByteBuffer();

		//WRITE: type
		byteBuffer->WriteByte(mMessageFrame);

                //WRITE: key
               	byteBuffer->WriteByte(mKeyCurrent);

        	//set 'last' commands for diff
        	mKeyLast = mKeyCurrent;

        	// Send the packet
        	mApplicationBreslin->mNetwork->send(byteBuffer);

		mRunNetworkTime = 0.0f;
    	}
}




/*************************************************
*
*   OGRE_SPECIFIC
*
**************************************************



/***************************************
*			INPUT
******************************************/


void Game::processInput()
{
	mKeyCurrent = 0;

	if (mApplicationBreslin->getKeyboard()->isKeyDown(OIS::KC_I)) // Forward
   {
		mKeyCurrent |= mKeyUp;
   }

   if (mApplicationBreslin->getKeyboard()->isKeyDown(OIS::KC_K)) // Backward
   {
		mKeyCurrent |= mKeyDown;
   }

	if (mApplicationBreslin->getKeyboard()->isKeyDown(OIS::KC_J)) // Left
   {
		mKeyCurrent |= mKeyLeft;
   }

   if (mApplicationBreslin->getKeyboard()->isKeyDown(OIS::KC_L)) // Right
   {
		mKeyCurrent |= mKeyRight;
   }

	if (mApplicationBreslin->getKeyboard()->isKeyDown(OIS::KC_Z)) // Rotate -Yaw(counter-clockwise)
   {
		mKeyCurrent |= mKeyCounterClockwise;
   }

   if (mApplicationBreslin->getKeyboard()->isKeyDown(OIS::KC_X)) // Right + Yaw(clockwise)
   {
		mKeyCurrent |= mKeyClockwise;
   }

}
