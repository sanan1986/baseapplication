#include "rotation.h"
#include "../../tdreamsock/dreamSockLog.h"

#include "../../shape/shape.h"

#include <string>

//Ogre headers
#include "Ogre.h"
using namespace Ogre;

//rotation states
#include "rotationStateMachine.h"
#include "rotationStates.h"

Rotation::Rotation(Shape* shape) : Ability(shape)
{
	mRotation = new Vector3D();
	mRotationLast = new Vector3D();

	mShape = shape;

	mRotationSpeed  = 0.0;

	mDegrees = 0.0f;

    	//run acceleration
    	mRotationAccel    = .04166f * 100.0f;
    	mRotationDecel    = .04166f * 100.0f;

	//rotation states
	mRotationStateMachine = new RotationStateMachine(this);    //setup the state machine
	mRotationStateMachine->setCurrentState      (Normal_Rotation::Instance());
	mRotationStateMachine->setPreviousState     (Normal_Rotation::Instance());
	mRotationStateMachine->setGlobalState       (NULL);
}

Rotation::~Rotation()
{
}

void Rotation::processTick()
{
 	Ogre::Quaternion orientation = mShape->mSceneNode->getOrientation();
        Ogre::Vector3 vector = orientation * -Vector3::UNIT_Z;

        mRotation->x = mShape->mSceneNode->_getDerivedOrientation().zAxis().x;

        mRotation->z = mShape->mSceneNode->_getDerivedOrientation().zAxis().z;

	mRotationStateMachine->update();
}


