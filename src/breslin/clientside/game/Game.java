package breslin.clientside.game;

/***************************************
*           INCLUDES
***************************************/
//standard library
import java.io.*;
import java.util.ArrayList;
import java.nio.ByteBuffer;

//shape
import breslin.clientside.shape.Shape;

//ability
import breslin.clientside.ability.rotation.AbilityRotation;
import breslin.clientside.ability.move.AbilityMove;

/***************************************
*           CLASS
***************************************/
public class Game
{


public Game(Application application)
{
	mApplication = application;
}

/***************************************
*   		MEMBER VARIABLES
***************************************/
//Shapes
public ArrayList<Shape> mShapeVector = new ArrayList<Shape>(); //all shapes in the client world
public ArrayList<Shape> mShapeGhostVector = new ArrayList<Shape>(); //all shapes in the client world's ghost


/***************************************
*   		METHODS
***************************************/
public void run()
{
	for (int i = 0; i < mShapeVector.size(); i++)
	{
		mShapeVector.get(i).interpolateTick(getRenderTime());
	}
}

public void addShape(boolean b, ByteBuffer byteBuffer)
{
	Shape shapeDynamicMonkey = new ShapeMonkey(this,byteBuffer,false);  //you should just need to call this...

	//ability
	shapeDynamicMonkey.addAbility(new AbilityRotation(shapeDynamicMonkey));
	shapeDynamicMonkey.addAbility(new AbilityMove    (shapeDynamicMonkey));
}

public void removeShape(ByteBuffer byteBuffer)
{

}

Shape getShape(int id)
{
	Shape shape = null;

	for (int i = 0; i < mShapeVector.size(); i++)
	{
		Shape curShape = mShapeVector.get(i);
		if (curShape.mIndex == id)
		{
			shape = curShape;
		}
	}

	if(shape == null)
	{
		return null;
	}
	else
	{
		return shape;
	}
}



}
