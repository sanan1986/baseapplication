package breslin.clientside.game;

/***************************************
*           INCLUDES
***************************************/
//standard library
import java.io.*;
import java.util.ArrayList;
import java.nio.ByteBuffer;

//shape
import breslin.clientside.shape.ShapeDynamic;

//network
import breslin.clientside.network.Network;

public class Game
{


public Game(byte[] serverIP, int serverPort)
{
	//StartLog();

	try
	{
		String serverIPString = new String(serverIP, "UTF8");
		System.out.println("serverIP:" + serverIPString + " serverPort:" + serverPort);

	}
	catch (UnsupportedEncodingException e)
	{
	    e.printStackTrace();
	}

	// network
	mNetwork = new Network(this,serverIP,serverPort);

	//time
	//mTime = new Time();
	mFrameTime		 = 0.0f;
	mOldTime         = 0;
}

/***************************************
*   		MEMBER VARIABLES
***************************************/
//Shapes
public ArrayList<ShapeDynamic> mShapeVector = new ArrayList<ShapeDynamic>(); //all shapes in the client world
public ArrayList<ShapeDynamic> mShapeGhostVector = new ArrayList<ShapeDynamic>(); //all shapes in the client world's ghost

//Network
public Network     mNetwork;

//time
//Time* mTime;
public float mFrameTime;
public float mRunNetworkTime;
public int   mOldTime;


/***************************************
*			          METHODS
*
*
*
*
***************************************/

/***************************************
*   		ADMIN
***************************************/
public void shutdown()
{

}

/***************************************
*   		SHAPES
***************************************/
public void addShape(boolean b, ByteBuffer byteBuffer)
{

}

public void removeShape(ByteBuffer byteBuffer)
{

}

ShapeDynamic getShapeDynamic(int id)
{
/*
	ByteBuffer byteBuffer = ByteBuffer.allocate(1400);



	return new ShapeDynamic(this, byteBuffer);
*/

ShapeDynamic shape = null;

	for (int i = 0; i < mShapeVector.size(); i++)
	{
		ShapeDynamic curShape = mShapeVector.get(i);
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

/***************************************
*   		TICKS
***************************************/
public void readServerTick           (ByteBuffer byteBuffer)
{
	// Skip sequences
	//short sequence = byteBuffer.get();

	//WRITE: sequence
	//byteBuffer.putShort(mOutgoingSequence);  //sequence
	byte one = byteBuffer.get(1);
	byte two = byteBuffer.get(2);
	byteBuffer.put(1,two);
	byteBuffer.put(2,one);
	byteBuffer.position(1);
	short sequence = byteBuffer.getShort();
//	System.out.println("sequence:" + sequence);





	while (byteBuffer.hasRemaining())
	{

		//mDetailsPanel->setParamValue(11, Ogre::StringConverter::toString(byteBuffer->GetSize()));

		int id = byteBuffer.get();
		//System.out.println("id:" + id);

		ShapeDynamic shape = null;
		shape = getShapeDynamic(id);

		if (shape != null)
		{
			//System.out.println("got shape");
			shape.readDeltaMoveCommand(byteBuffer);
		}

	}

}

public void interpolateTick()
{

	for (int i = 0; i < mShapeVector.size(); i++)
	{
		mShapeVector.get(i).interpolateTick(getRenderTime());
	}

}

/***************************************
*   		TIME
***************************************/
public float getRenderTime()
{
	System.out.println("fd");
	return 0;
}

/***************************************
*   		NETWORK
***************************************/
public void runNetwork    (float msec)
{
	mRunNetworkTime += msec;

	mNetwork.readPackets();

	// Framerate is too high
	if(mRunNetworkTime > (1000 / 60))
	{
		mNetwork.sendCommand();
		mFrameTime = mRunNetworkTime / 1000.0f;
		mRunNetworkTime = 0;
	}
}

/***************************************
*   		INPUT
***************************************/
void processInput()
{

}

/***************************************
*   		GRAPHICS
***************************************/
public boolean runGraphics()
{
	return false;
}


}