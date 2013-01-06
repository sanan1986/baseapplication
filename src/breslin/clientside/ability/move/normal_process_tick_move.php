var NormalProcessTickMove = new Class(
{

Extends: AbilityMoveState,
initialize: function (abilityMove)
{
	this.mAbilityMove = abilityMove;
},

enter: function()
{

},

execute: function()
{
        // if distance exceeds threshold && server velocity is zero
        if(this.mAbilityMove.mDeltaPosition > this.mAbilityMove.mPosInterpLimitHigh && !this.mAbilityMove.mShape.mServerCommandCurrent.mVelocity.isZero())
        {
                this.mAbilityMove.mProcessTickStateMachine.changeState(this.mAbilityMove.mCatchupProcessTickMove);
        }
        serverVelocity = new Vector3D();
        serverVelocity.copyValuesFrom(this.mAbilityMove.mShape.mServerCommandCurrent.mVelocity);
        serverVelocity.normalise();

        if(this.mAbilityMove.mShape.mCommandToRunOnShape.mFrameTime != 0)
        {
                this.mAbilityMove.mShape.mSpeed = abilityMove.calcuateSpeed(
                this.mAbilityMove.mShape.mServerCommandCurrent.mVelocity,
                this.mAbilityMove.mShape.mCommandToRunOnShape.mFrameTime);
        }

        serverVelocity.multiply(this.mAbilityMove.mShape.mSpeed);
        //abilityMove->regulate(serverVelocity);
        this.mAbilityMove.mShape.mCommandToRunOnShape.mVelocity.copyValuesFrom(serverVelocity);
},

exit: function()
{

}


});

