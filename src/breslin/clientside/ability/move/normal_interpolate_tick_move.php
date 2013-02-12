var NormalInterpolateTickMove = new Class(
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
        //to be used to setPosition
        transVector = new Vector3D();


        //copy values from mVelocity so we don't make changes to original
        transVector.copyValuesFrom(this.mAbilityMove.mShape.mCommandToRunOnShape.mVelocity);
//	document.getElementById('mMessageFrame').innerHTML='bx_p: ' + transVector.x + ' bz: ' + transVector.z;
        //get the mulitplier
        parsedRenderTime = parseFloat(this.mAbilityMove.mShape.mApplicationBreslin.getRenderTime());
	
	multipliedRenderTime = parsedRenderTime * 1000;
	//document.getElementById('mMessageFrame').innerHTML='mrt: ' + multipliedRenderTime;

        //multiply our vector using render values
        transVector.multiply(multipliedRenderTime);
//	document.getElementById('mMessageFrame').innerHTML='xx: ' + transVector.x + ' zz: ' + transVector.z;

	transVector.add(this.mAbilityMove.mShape.getPosition());

//	document.getElementById('mMessageFrame').innerHTML='spx: ' + this.mAbilityMove.mShape.getPosition().x + ' spz: ' + this.mAbilityMove.mShape.getPosition().z;
	//add our velocity to current position

	//set new position
	this.mAbilityMove.mShape.setPosition(transVector);
	
/*
        if (abilityMove->mShape->mLocal == 1)
        {
                abilityMove->mShape->mApplicationBreslin->getCamera()->setPosition(Ogre::Vector3(transVector->x,transVector->y + 20,transVector->z + 20));
                abilityMove->mShape->mApplicationBreslin->getCamera()->lookAt(Ogre::Vector3(transVector->x,transVector->y,transVector->z));

        }

*/
},

exit: function()
{

}


});

