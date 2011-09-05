package breslin.clientside.shape;

/**********************************
*          INCLUDES
**********************************/
//parent
import breslin.clientside.shape.ShapeDynamic;

//standard library
import java.util.ArrayList;
import java.nio.ByteBuffer;
import java.lang.String;

  // geometry
  import com.jme3.material.Material;
  import com.jme3.math.Vector3f;
  import com.jme3.scene.Geometry;
  import com.jme3.scene.shape.Box;
  import com.jme3.math.ColorRGBA;
  import com.jme3.scene.Node;


//shape
import breslin.clientside.shape.Shape;

//game
import breslin.clientside.game.GameMonkey;

//math
import breslin.math.Vector3D;

//ability
import breslin.clientside.ability.Ability;

//parser
import breslin.clientside.parser.Parser;

//command
import breslin.clientside.command.Command;



/**********************************
*          CLASS
**********************************/
public class ShapeDynamicMonkey extends ShapeDynamic //, public OgreAnimation
{


public ShapeDynamicMonkey(GameMonkey gameMonkey, ByteBuffer byteBuffer, boolean isGhost)
{
	super(gameMonkey,byteBuffer);

	System.out.println("creating a monkey shape in constructor.");
	//we use this to name shape. as ogre is picky about same names. it also serves as a counter of sorts.

	mGameMonkey = gameMonkey;

	mIsGhost = isGhost;

	if (mIsGhost)
	{
		mIndex = mIndex * -1;
	}

	//figure out mesh based on code passed in byteBuffer
	mMeshName = getMeshString(mMeshCode);


	createShape();

	//animation
	//if (mAnimate)
	//{
	//	addAbility(new AbilityAnimationOgre(this));
	//}

	//setupTitle();

	//call create ghost here..
	if (!mIsGhost)
	{
		//create a ghost for this shape
		mGhost = new ShapeDynamicMonkey(mGameMonkey,byteBuffer,true);
		//mGhost->setVisible(false);

		//put shape and ghost in game vectors so they can be looped and game now knows of them.
		mGame.mShapeVector.add(this);
		mGame.mShapeGhostVector.add(mGhost);
	}


}

/**********************************
*          VARIABLES
**********************************/

//game
GameMonkey mGameMonkey;

//ghost
boolean mIsGhost;

//ogre scene stuff
String        mMeshName;
String         mName;
//Entity*             mEntity;

//this is your pointer to move shape, really all you need.
Node          mSceneNode;

//billboard
//ObjectTitle* mObjectTitle;
//std::string  mObjectTitleString;

//ability

/**********************************
*          METHODS
**********************************/
//shape
void createShape()
{
	//actually create the shape and attach to node.
	mSceneNode = (Node) mGameMonkey.mGraphicsMonkey.getAssetManager().loadModel(getMeshString(mMeshCode));
	mGameMonkey.mGraphicsMonkey.getRootNode().attachChild(mSceneNode);

	//scale
	mSceneNode.scale(mScale,mScale,mScale);

	//move
	mPosition.x = 5.0f;
	setPosition((float)mPosition.x,(float)mPosition.y,(float)mPosition.z);
}

//debugging
void checkExtents(Vector3D min)
{

}

//size
void scale(Vector3D scaleVector)
{

}

//movement
public void yaw        (float amountToYaw, boolean converToDegree   )
{

}

void translate  (Vector3D translateVector, int perspective)
{
	if (perspective == 1)
	{
		//getSceneNode()->translate(converToVector3(translateVector), Ogre::Node::TS_WORLD);
		getSceneNode().move((float)translateVector.x,(float)translateVector.y,(float)translateVector.z);
	}
	if (perspective == 2)
	{
		//getSceneNode()->translate(converToVector3(translateVector), Ogre::Node::TS_LOCAL);
		getSceneNode().move((float)translateVector.x,(float)translateVector.y,(float)translateVector.z);
	}
}
void setPosition(Vector3D position                        )
{
	getSceneNode().setLocalTranslation((float)position.x,(float)position.y,(float)position.z);
}
void setPosition(float x, float y, float z                )
{
	//in ogre setPosition is absolute so for jmonkey i could just take the difference?
	//mSceneNode.getPosition();
	getSceneNode().setLocalTranslation(x,y,z);
}
void setVisible (boolean visible                             )
{

}

//check position
float    getDegreesToSomething(Vector3D something                       )
{
	return 0;
}
Vector3D getPosition          (                                         )
{
	Vector3f vector3f = getSceneNode().getWorldTranslation();
	Vector3D vector3D = new Vector3D();
	vector3D.x = vector3f.x;
	vector3D.y = vector3f.y;
	vector3D.z = vector3f.z;

	return vector3D;
}

//title
void drawTitle    ()
{

}

void appendToTitle(String appendage)
{

}
void appendToTitle(int appendage)
{

}

void clearTitle   ()
{

}

void setupTitle()
{

}


//name
String getName()
{
	return mName;
}

//scene node
Node  getSceneNode() { return mSceneNode; }

//utility
//Ogre::Vector3 converToVector3(Vector3D vector3d);



String getMeshString(int meshCode)
{

	if (meshCode == 0)
	{
		mScale = .25f;
		return "cube.mesh";
	}
	if (meshCode == 1)
	{
		mScale = .25f;
		return "assets/Models/Sinbad/Sinbad.mesh.xml";
	}
	return "cube.mesh";
}

}
