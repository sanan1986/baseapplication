var CatchupProcessTickMove = new Class(
{

Extends: AbilityMoveState,
initialize: function (abilityMove)
{
	this.mAbilityMove = abilityMove;
},

log: function(msg)
{
        setTimeout(function()
        {
                throw new Error(msg);
        }, 0);
},

enter: function()
{

},

execute: function()
{
/*
        //if we are back in sync
        if(abilityMove->mDeltaPosition <= abilityMove->mPosInterpLimitHigh || abilityMove->mShape->mServerCommandCurrent->mVelocity->isZero())
        {
                abilityMove->mProcessTickStateMachine->changeState(Normal_ProcessTick_Move::Instance());
        }

        //this is what we will set mCommandToRunOnShape->mVelocity to
        Vector3D* newVelocity = new Vector3D(); //vector to future server pos

        //first set newVelocity to most recent velocity from server.
        newVelocity->copyValuesFrom(abilityMove->mShape->mServerCommandCurrent->mVelocity);

        //normalise it now we know what direction to head in.
        newVelocity->normalise();

        //le'ts find out how fast
        //change in position times our interp factor
        float multiplier = abilityMove->mDeltaPosition * abilityMove->mPosInterpFactor;

        //multiply our normalized velocity by multiplier(change * interpfactor)
        newVelocity->multiply(multiplier);

        //add the latest server position to our newvelocity
        newVelocity->add(abilityMove->mShape->mServerCommandCurrent->mPosition);

        //now subtract our current position from our new velocity
        newVelocity->subtract(abilityMove->mShape->getPosition());

        //dist from client pos to future server pos
        float predictDist = pow(newVelocity->x, 2) + pow(newVelocity->y, 2) + pow(newVelocity->z, 2);
        predictDist = sqrt(predictDist);

        //server velocity
        if(abilityMove->mShape->mCommandToRunOnShape->mFrameTime != 0)
        {
                abilityMove->mShape->mSpeed = abilityMove->calcuateSpeed(
abilityMove->mShape->mServerCommandCurrent->mVelocity,
               abilityMove->mShape->mCommandToRunOnShape->mFrameTime);
        }

        if(abilityMove->mShape->mSpeed != 0.0)
        {
                //time needed to get to future server pos
                float time = abilityMove->mDeltaPosition * abilityMove->mPosInterpFactor/abilityMove->mShape->mSpeed;

                newVelocity->normalise();  //?????what the hell why i am normalizing this after all that work above?

                //client vel needed to get to future server pos in time
                float distTime = predictDist/time;
                newVelocity->multiply(distTime);

                //set newVelocity to mCommandToRunOnShape->mVelocity which is what interpolateTick uses
                                //abilityMove->regulate(newVelocity);
                abilityMove->mShape->mCommandToRunOnShape->mVelocity->copyValuesFrom(newVelocity);
        }
        else
        {
                //why would catchup ever need to set velocity to zero, wouldn't we simply leave catchup state??
                abilityMove->mShape->mCommandToRunOnShape->mVelocity->zero();
        }
*/
	this.log('catchup');        
	//if we are back in sync
        if(this.mAbilityMove.mDeltaPosition <= this.mAbilityMove.mPosInterpLimitHigh || this.mAbilityMove.mShape.mServerCommandCurrent.mVelocity.isZero())
        {
                this.mAbilityMove.mProcessTickStateMachine.changeState(this.mAbilityMove.mNormalProcessTickMove);
        }
        //this is what we will set mCommandToRunOnShape->mVelocity to
        newVelocity = new Vector3D(); //vector to future server pos

        //first set newVelocity to most recent velocity from server.
        newVelocity.copyValuesFrom(this.mAbilityMove.mShape.mServerCommandCurrent.mVelocity);

        //normalise it now we know what direction to head in.
        newVelocity.normalise();

        //le'ts find out how fast
        //change in position times our interp factor
        multiplier = this.mAbilityMove.mDeltaPosition * this.mAbilityMove.mPosInterpFactor;

        //multiply our normalized velocity by multiplier(change * interpfactor)
        newVelocity.multiply(multiplier);

        //add the latest server position to our newvelocity
        newVelocity.add(this.mAbilityMove.mShape.mServerCommandCurrent.mPosition);

        //now subtract our current position from our new velocity
        newVelocity.subtract(this.mAbilityMove.mShape.getPosition());

        //dist from client pos to future server pos
        predictDist = Math.pow(newVelocity.x, 2) + Math.pow(newVelocity.y, 2) + Math.pow(newVelocity.z, 2);
        predictDist = Math.sqrt(predictDist);

        //server velocity
        if(this.mAbilityMove.mShape.mCommandToRunOnShape.mFrameTime != 0)
        {
                this.mAbilityMove.mShape.mSpeed = abilityMove.calcuateSpeed(
this.mAbilityMove.mShape.mServerCommandCurrent.mVelocity,
               this.mAbilityMove.mShape.mCommandToRunOnShape.mFrameTime);
        }

        if(this.mAbilityMove.mShape.mSpeed != 0.0)
        {
                //time needed to get to future server pos
                time = this.mAbilityMove.mDeltaPosition * this.mAbilityMove.mPosInterpFactor/this.mAbilityMove.mShape.mSpeed;

                newVelocity.normalise();  //?????what the hell why i am normalizing this after all that work above?

                //client vel needed to get to future server pos in time
                distTime = predictDist/time;
                newVelocity.multiply(distTime);

                //set newVelocity to mCommandToRunOnShape->mVelocity which is what interpolateTick uses
                                //abilityMove->regulate(newVelocity);
                this.mAbilityMove.mShape.mCommandToRunOnShape.mVelocity.copyValuesFrom(newVelocity);
        }
        else
        {
                //why would catchup ever need to set velocity to zero, wouldn't we simply leave catchup state??
                this.mAbilityMove.mShape.mCommandToRunOnShape.mVelocity.zero();
        }
},

exit: function()
{

}


});

