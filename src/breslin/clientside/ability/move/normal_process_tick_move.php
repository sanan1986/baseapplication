var NormalProcessTickMove = new Class(
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
        this.log(this.mAbilityMove.mShape.mCommandToRunOnShape.mFrameTime);
	
//	this.log('ex normal process tick');
        // if distance exceeds threshold && server velocity is zero
/*
        if(this.mAbilityMove.mDeltaPosition > this.mAbilityMove.mPosInterpLimitHigh && !this.mAbilityMove.mShape.mServerCommandCurrent.mVelocity.isZero())
        {
                this.mAbilityMove.mProcessTickStateMachine.changeState(this.mAbilityMove.mCatchupProcessTickMove);
        }
*/
//	this.log('x:' + this.mAbilityMove.mShape.mServerCommandCurrent.mVelocity.x); 
//	this.log('z:' + this.mAbilityMove.mShape.mServerCommandCurrent.mVelocity.z); 
        serverVelocity = new Vector3D();

        serverVelocity.copyValuesFrom(this.mAbilityMove.mShape.mServerCommandCurrent.mVelocity);
        serverVelocity.normalise();

//        this.log('mFrameTime:' + this.mAbilityMove.mShape.mCommandToRunOnShape.mFrameTime)
        if(this.mAbilityMove.mShape.mCommandToRunOnShape.mFrameTime != 0)
        {
                v = this.mAbilityMove.mShape.mServerCommandCurrent.mVelocity;
                f = this.mAbilityMove.mShape.mCommandToRunOnShape.mFrameTime;
		
                this.mAbilityMove.mShape.mSpeed = this.mAbilityMove.calculateSpeed(v,f);
//		this.log('speed:' + this.mAbilityMove.mShape.mSpeed);
        }
        serverVelocity.multiply(this.mAbilityMove.mShape.mSpeed);
//	this.log('x:' + serverVelocity.x); 
//	this.log('z:' + serverVelocity.z); 
        //abilityMove->regulate(serverVelocity);
        this.mAbilityMove.mShape.mCommandToRunOnShape.mVelocity.copyValuesFrom(serverVelocity);
},

exit: function()
{

}


});

