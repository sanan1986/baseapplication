var Game = new Class(
{

initialize: function(applicationBreslin)
{
	// constants
	this.mCommandKey          = 1;
	this.mCommandFrameTime = 2;

	this.mMessageFrame = 2;  //changed this to browser code

	this.mMessageAddShape    = -103;
	this.mMessageRemoveShape = -104;

	//application	
	this.mApplicationBreslin = applicationBreslin;	

	//shapes
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
	this.mPollDelay = 32;
	this.mPollDelayCounter = 0;

       	//states 
	this.mStateMachine = new StateMachine();
        
        this.mGameGlobal = new GameGlobal(this);
        this.mGameInitialize = new GameInitialize(this);
        this.mGamePlay = new GamePlay(this);
        this.mGamePause = new GamePause(this);

        this.mStateMachine.setGlobalState(this.mGameGlobal);
        this.mStateMachine.changeState(this.mGamePlay);

/*
        //set Camera
        // Position it at 500 in Z direction
        mApplicationBreslin->getCamera()->setPosition(Ogre::Vector3(0,20,20));
        // Look back along -Z
        mApplicationBreslin->getCamera()->lookAt(Ogre::Vector3(0,0,0));
        mApplicationBreslin->getCamera()->setNearClipDistance(5);
*/

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

processUpdate: function()
{
	this.mStateMachine.update();

	for (i = 0; i < this.mShapeVector.length; i++)
	{
		this.mShapeVector[i].interpolateTick(this.mApplicationBreslin.getRenderTime());
	}
},

/*********************************
               NETWORK 
**********************************/
/*
void Game::checkForByteBuffer()
{
        int type = 0;

        ByteBuffer* byteBuffer = new ByteBuffer();

        while(mApplicationBreslin->mNetwork->checkForByteBuffer(byteBuffer))
        {
                byteBuffer->BeginReading();

                type = byteBuffer->ReadByte();

                switch(type)
                {
                        case mMessageAddShape:
                                addShape(true,byteBuffer);
                        break;

                        case mMessageRemoveShape:
                            removeShape(byteBuffer);
                        break;

                        case mMessageFrame:
                                readServerTick(byteBuffer);
                        break;
                }
        }
}
*/
checkForByteBuffer: function()
{

	this.mApplicationBreslin.mNetwork.getAllShapes();
 	var shapesTable = document.getElementById('shapes_table');
    	var rowLength = shapesTable.rows.length;
        
	for (i = 2; i < rowLength; i++)
        {
                foundMatch = false;

                for (s = 0; s < this.mShapeVector.length; s++)
                {
                        if (this.mShapeVector[s].mIndex == shapesTable.rows.item(i).cells.item(0).innerHTML)
                        {
                                foundMatch = true;
                        }
                }
                if (foundMatch)
                {
                        //do nothing or set vars
                }
                else
                {
	
		}
	}
/*
	if (this.mMissingID == false)
	{
		this.mApplicationBreslin.mNetwork.checkForByteBuffer();
	}
	else
	{
		//call for shape_table table 
		this.mApplicationBreslin.mNetwork.getAllShapes();
	}
	
	this.mMissingID = false;

 	var positionTable = document.getElementById('shapes_table');

    	var rowLength = positionTable.rows.length;

	for (i = 2; i < rowLength; i++)
	{
		foundMatch = false;			
		
		for (s = 0; s < this.mShapeVector.length; s++)
		{
			if (this.mShapeVector[s].mIndex == positionTable.rows.item(i).cells.item(0).innerHTML)	
			{
				foundMatch = true;
			}
		}	
		if (foundMatch)
		{
			//do nothing or set vars
		}
		else
		{
			this.mMissingID = true;
			missingId = positionTable.rows.item(i).cells.item(0).innerHTML;
 			var getAllShapesTable = document.getElementById('shapes_table');

			if (getAllShapesTable)
			{
    				var allshapeslength = positionTable.rows.length;

				//let's create any shapes we don't have yet		
				for (a = 2; a < allshapeslength; a++)
				{
			 		if (missingId == positionTable.rows.item(a).cells.item(0).innerHTML)
					{
						//create this...
						byteBuffer = new ByteBuffer();
						client = getAllShapesTable.rows.item(a).cells.item(5).innerHTML;	
						x      = getAllShapesTable.rows.item(a).cells.item(6).innerHTML;	
						z      = getAllShapesTable.rows.item(a).cells.item(7).innerHTML;	
						rx     = getAllShapesTable.rows.item(a).cells.item(8).innerHTML;	
						rz     = getAllShapesTable.rows.item(a).cells.item(9).innerHTML;	
						m      = getAllShapesTable.rows.item(a).cells.item(10).innerHTML;	
						a      = getAllShapesTable.rows.item(a).cells.item(11).innerHTML;	
						shape = new Shape(this.mApplicationBreslin,false,missingId,client,x,z,rx,rz,m,a);
						this.mShapeVector.push(shape);
					}
				}
			} 
		}
	}
*/	
	

},
/*
void Game::readServerTick(ByteBuffer* byteBuffer)
{
        // Skip sequences
        short sequence = byteBuffer->ReadShort();

        while (byteBuffer->getReadCount() <= byteBuffer->GetSize())
        {
                mApplicationBreslin->mDetailsPanel->setParamValue(11, Ogre::StringConverter::toString(byteBuffer->GetSize()));

                int id = byteBuffer->ReadByte();

                Shape* shape = NULL;
                shape = getShape(id);

                                if (shape)
                {
                        shape->processDeltaByteBuffer(byteBuffer);
                }
                else
                {
                        //LogString("INVALID SHAPE ID");
                }
        }
}
*/
sendByteBuffer: function()
{
        this.mRunNetworkTime += this.mApplicationBreslin.getRenderTime() * 1000.0;
  	
	// Framerate is too high
        if(this.mRunNetworkTime > (1000 / 60))
        {
                // Build delta-compressed move command
                flags = 0;

                //if key has not been changed return having done nothing
                if(this.mKeyLast != this.mKeyCurrent)
                {
                        flags |= this.mCommandKey;
                }
                else
                {
                        return;
                }

                // Send the packet
                this.mApplicationBreslin.mNetwork.sendMove(this.mMessageFrame,this.mOutgoingSequence,flags,this.mKeyCurrent);

                //set 'last' commands for diff
                this.mKeyLast = this.mKeyCurrent;
                
		this.mOutgoingSequence++; //increase for next time...
		
		//reset network time so we can start count anew
		this.mRunNetworkTime = 0.0;
	}
},


/***************************************
*                       INPUT
******************************************/

processInput: function()
{
	this.mKeyCurrent = 0;

	if (this.mApplicationBreslin.mKey_up)
	{
		this.mKeyCurrent |= this.mKeyUp;
	}
	if (this.mApplicationBreslin.mKey_down)
	{
		this.mKeyCurrent |= this.mKeyDown;
	}
	if (this.mApplicationBreslin.mKey_left)
	{
		this.mKeyCurrent |= this.mKeyLeft;
	}
	if (this.mApplicationBreslin.mKey_right)
	{
		this.mKeyCurrent |= this.mKeyRight;
	}

	if (this.mApplicationBreslin.mKey_z)
	{
		this.mKeyCurrent |= this.mKeyCounterClockwise;
	}
	if (this.mApplicationBreslin.mKey_x)
	{
		this.mKeyCurrent |= this.mKeyClockwise;
	}
}

});

