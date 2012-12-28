var Game = new Class(
{

initialize: function(applicationBreslin)
{
/*
        //set Camera
        // Position it at 500 in Z direction
        mApplicationBreslin->getCamera()->setPosition(Ogre::Vector3(0,20,20));
        // Look back along -Z
        mApplicationBreslin->getCamera()->lookAt(Ogre::Vector3(0,0,0));
        mApplicationBreslin->getCamera()->setNearClipDistance(5);
*/
	
	this.mApplicationBreslin = applicationBreslin;	

	this.mShapeVector      = new Array();
	this.mShapeGhostVector = new Array();

        //keys
        this.mKeyUp = 1;
        this.mKeyDown = 2;
        this.mKeyLeft = 4;
        this.mKeyRight = 8;
        this.mKeyCounterClockwise = 16;
        this.mKeyClockwise = 32;

        //input
        this.mKeyCurrent = 0;
        this.mKeyLast = 0;

        //sequence
        this.mOutgoingSequence               = 1;

        //time
        this.mRunNetworkTime = 0.0;

       	//states 
	this.mStateMachine = new StateMachine();
        
        this.mGameGlobal = new GameGlobal(this);
        this.mGameInitialize = new GameInitialize(this);
        this.mGamePlay = new GamePlay(this);
        this.mGamePause = new GamePause(this);

        this.mStateMachine.setGlobalState(this.mGameGlobal);
        this.mStateMachine.changeState(this.mGamePlay);



},

log: function(msg)
{
	setTimeout(function()
        {
        	throw new Error(msg);
        }, 0);
},
/*********************************
                Update
**********************************/
/*
void Game::processUpdate()
{
        mStateMachine->update();

        for (unsigned int i = 0; i < mShapeVector->size(); i++)
        {
                mShapeVector->at(i)->interpolateTick(mApplicationBreslin->getRenderTime());
        }
}
*/

processUpdate: function()
{
	this.mStateMachine.update();

	for (i = 0; i < this.mShapeVector.length; i++)
	{
		this.mShapeVector[i].interpolateTick(this.mApplicationBreslin.getRenderTime());
	}
}



});

