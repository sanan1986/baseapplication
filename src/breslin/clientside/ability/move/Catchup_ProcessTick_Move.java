package breslin.clientside.ability.move;

/******************************************************
*				INCLUDES
********************************************************/
//parent
import breslin.clientside.ability.move.AbilityMoveState;

//math
import breslin.math.Vector3D;

class Catchup_ProcessTick_Move extends AbilityMoveState
{

	private static Catchup_ProcessTick_Move mAbilityMoveState;

	/** A private Constructor prevents any other class from instantiating. */
	private Catchup_ProcessTick_Move()
	{
		  //	 Optional Code
	}

/******************************************************
*				METHODS
*
*
*
*
********************************************************/

public static synchronized Catchup_ProcessTick_Move getAbilityMoveState()
{
   	if (mAbilityMoveState == null){
    	mAbilityMoveState = new Catchup_ProcessTick_Move();
    }
	    return mAbilityMoveState;
}


public Object clone()throws CloneNotSupportedException
{
    throw new CloneNotSupportedException();
}



public void enter(AbilityMove abilityMove)
{
}

public void execute(AbilityMove abilityMove)
{
	/*
	//if we are back in sync
    if(abilityMove.mDeltaPosition <= abilityMove.mPosInterpLimitHigh || abilityMove.mShapeDynamic.mServerFrame.mMoveVelocity.isZero())
    {
		abilityMove.mProcessTickStateMachine.changeState(Normal_ProcessTick_Move.getAbilityMoveState());
    }
    else
    {
		Vector3D serverDest = new Vector3D(); //vector to future server pos
        Vector3D myDest     = new Vector3D(); //vector from clienr pos to future server pos

        serverDest.x = abilityMove.mShapeDynamic.mServerFrame.mMoveVelocity.x;
        serverDest.y = abilityMove.mShapeDynamic.mServerFrame.mMoveVelocity.y;
        serverDest.z = abilityMove.mShapeDynamic.mServerFrame.mMoveVelocity.z;
        serverDest.normalise();

        float multiplier = abilityMove.mDeltaPosition * abilityMove.mPosInterpFactor;

        serverDest.x = serverDest.x * multiplier;
        serverDest.y = serverDest.y * multiplier;
        serverDest.z = serverDest.z * multiplier;

        serverDest.x = abilityMove.mShapeDynamic.mServerFrame.mPosition.x + serverDest.x;
        serverDest.y = abilityMove.mShapeDynamic.mServerFrame.mPosition.y + serverDest.y;
        serverDest.z = abilityMove.mShapeDynamic.mServerFrame.mPosition.z + serverDest.z;

        myDest.x = serverDest.x - abilityMove.mShapeDynamic.getPosition().x;
        myDest.y = serverDest.y - abilityMove.mShapeDynamic.getPosition().y;
        myDest.z = serverDest.z - abilityMove.mShapeDynamic.getPosition().z;


        //dist from clienr pos to future server pos
        float predictDist = (float)
        (float)java.lang.Math.pow(myDest.x, 2) +
        (float)java.lang.Math.pow(myDest.y, 2) +
        (float)java.lang.Math.pow(myDest.z, 2);

        predictDist = (float)java.lang.Math.sqrt(predictDist);

        //server velocity
		if(abilityMove.mShapeDynamic.mCommandToRunOnShape.mMilliseconds != 0)
        {
           abilityMove.mSpeed = (float)
           		java.lang.Math.sqrt(
			   java.lang.Math.pow(abilityMove.mShapeDynamic.mServerFrame.mMoveVelocity.x, 2) +
               java.lang.Math.pow(abilityMove.mShapeDynamic.mServerFrame.mMoveVelocity.y, 2) +
               java.lang.Math.pow(abilityMove.mShapeDynamic.mServerFrame.mMoveVelocity.z, 2)) /
               (float)abilityMove.mShapeDynamic.mCommandToRunOnShape.mMilliseconds;

		   abilityMove.mShapeDynamic.mSpeed = abilityMove.mSpeed;
		}

		if(abilityMove.mSpeed != 0.0)
		{
           //time needed to get to future server pos
           float time = abilityMove.mDeltaPosition * abilityMove.mPosInterpFactor/abilityMove.mSpeed;

           myDest.normalise();

           //client vel needed to get to future server pos in time
		   float distTime = predictDist/time;

           myDest.x = myDest.x * distTime;
           myDest.y = myDest.y * distTime;
           myDest.z = myDest.z * distTime;

           abilityMove.mShapeDynamic.mCommandToRunOnShape.mMoveVelocity.x = myDest.x;
           abilityMove.mShapeDynamic.mCommandToRunOnShape.mMoveVelocity.y = myDest.y;
           abilityMove.mShapeDynamic.mCommandToRunOnShape.mMoveVelocity.z = myDest.z;

		}
		else
		{
			//why would catchup ever need to set velocity to zero, wouldn't we simply leave catchup state??
           abilityMove.mShapeDynamic.mCommandToRunOnShape.mMoveVelocity.x = 0.0f;
           abilityMove.mShapeDynamic.mCommandToRunOnShape.mMoveVelocity.y = 0.0f;
           abilityMove.mShapeDynamic.mCommandToRunOnShape.mMoveVelocity.z = 0.0f;

		}
	}
	*/
	//if we are back in sync
    if(abilityMove.mDeltaPosition <= abilityMove.mPosInterpLimitHigh || abilityMove.mShapeDynamic.mServerFrame.mMoveVelocity.isZero())
    {
		abilityMove.mProcessTickStateMachine.changeState(Normal_ProcessTick_Move.getAbilityMoveState());
    }
    else
    {
		//this is what we will set mCommandToRunOnShape.mMoveVelocity to
		Vector3D newVelocity = new Vector3D(); //vector to future server pos

		//first set newVelocity to most recent velocity from server.
 		newVelocity.copyValuesFrom(abilityMove.mShapeDynamic.mServerFrame.mMoveVelocity);

		//normalise it now we know what direction to head in.
        newVelocity.normalise();

		//le'ts find out how fast
		//change in position times our interp factor
        float multiplier = abilityMove.mDeltaPosition * abilityMove.mPosInterpFactor;

		//multiply our normalized velocity by multiplier(change * interpfactor)
		newVelocity.multiply(multiplier);

		//add the latest server position to our newvelocity
		newVelocity.add(abilityMove.mShapeDynamic.mServerFrame.mPosition);

		//now subtract our current position from our new velocity
		newVelocity.subtract(abilityMove.mShapeDynamic.getPosition());

        //dist from client pos to future server pos
        float predictDist = (float)
        (float)java.lang.Math.pow(newVelocity.x, 2) +
        (float)java.lang.Math.pow(newVelocity.y, 2) +
        (float)java.lang.Math.pow(newVelocity.z, 2);
        predictDist = (float)java.lang.Math.sqrt(predictDist);

        //server velocity
		if(abilityMove.mShapeDynamic.mCommandToRunOnShape.mMilliseconds != 0)
        {
			abilityMove.mSpeed = abilityMove.calcuateSpeed(
			abilityMove.mShapeDynamic.mServerFrame.mMoveVelocity,
			abilityMove.mShapeDynamic.mCommandToRunOnShape.mMilliseconds);

			abilityMove.mShapeDynamic.mSpeed = abilityMove.mSpeed;
		}

		if(abilityMove.mSpeed != 0.0)
		{
			//time needed to get to future server pos
			float time = abilityMove.mDeltaPosition * abilityMove.mPosInterpFactor/abilityMove.mSpeed;

			newVelocity.normalise();  //?????what the hell why i am normalizing this after all that work above?

			//client vel needed to get to future server pos in time
			float distTime = predictDist/time;
			newVelocity.multiply(distTime);

			//set newVelocity to mCommandToRunOnShape.mMoveVelocity which is what interpolateTick uses
			//LogString("x:%f",newVelocity.x);

			//keep player from "teleporting"
			abilityMove.regulate(newVelocity);

			abilityMove.mShapeDynamic.mCommandToRunOnShape.mMoveVelocity.copyValuesFrom(newVelocity);

		}
		else
		{
			//why would catchup ever need to set velocity to zero, wouldn't we simply leave catchup state??
			abilityMove.mShapeDynamic.mCommandToRunOnShape.mMoveVelocity.zero();

		}
	}
}

public void exit(AbilityMove abilityMove)
{
}


}