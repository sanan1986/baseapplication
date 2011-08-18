#ifndef SHAPEDYNAMICOGRE_H
#define SHAPEDYNAMICOGRE_H

#include "shapeDynamic.h"

//Ogre headers
#include "Ogre.h"
using namespace Ogre;

class ObjectTitle;

//ability
class AbilityRotationOgre;
class AbilityMoveOgre;
class AbilityAnimationOgre;

#include <string>

class ShapeDynamicOgre : public ShapeDynamic //, public OgreAnimation
{

public:
ShapeDynamicOgre(Game* game, Dispatch* dispatch, bool isGhost);
~ShapeDynamicOgre();


//ogre scene stuff
std::string         mMeshName;
std::string         mName;
Entity*             mEntity;

//this is your pointer to move shape, really all you need.
SceneNode*          mSceneNode;

//billboard
ObjectTitle* mObjectTitle;
std::string  mObjectTitleString;

//shape
void createShape();

//ghost
bool mIsGhost;

//debugging
void checkExtents(Vector3D min);

//size
void scale(Vector3D scaleVector);

//movement
void yaw        (float amountToYaw, bool converToDegree   );
void translate  (Vector3D translateVector, int perspective);
void setPosition(Vector3D position                        );
void setPosition(float x, float y, float z                );
void setVisible (bool visible                             );

//check position
float    getDegreesToSomething(Vector3D something                       );
Vector3D getPosition          (                                         );

//title
void drawTitle    ();
void appendToTitle(std::string appendage);
void appendToTitle(int appendage);
void clearTitle   ();
void setupTitle();

//name
std::string getName() { return mName; } 

//ogre scene node
SceneNode*  getSceneNode() { return mSceneNode; }

//utility
Ogre::Vector3 converToVector3(Vector3D vector3d);

//ability
AbilityRotationOgre* mAbilityRotationOgre;
AbilityMoveOgre* mAbilityMoveOgre;
AbilityAnimationOgre* mAbilityAnimationOgre;

};

#endif

