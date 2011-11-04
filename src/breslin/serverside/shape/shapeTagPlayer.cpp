//headers
#include "shapeTagPlayer.h"

//log
#include "../tdreamsock/dreamSockLog.h"

ShapeTagPlayer::ShapeTagPlayer(unsigned int index, Game* game, Client* client, Vector3D* position, Vector3D* velocity, Vector3D* rotation, Ogre::Root* root,
			 bool animated ,bool collidable, float collisionRadius, int meshCode, bool ai)
	:
		ShapeTag(index, game, client,position,velocity,rotation,root,animated,collidable,collisionRadius,meshCode,ai)

{
	LogString("ShapeTagPlayer Constructor");
}
	
ShapeTagPlayer::~ShapeTagPlayer()
{

}
