var PLAY_PARTIDO_GAME = new Class(
{
Extends: State,

initialize: function()
{

},

log: function(msg)
{
        setTimeout(function()
        {
                throw new Error(msg);
        }, 0);
},

enter: function(gamePartido)
{
	gamePartido.reset();
},

execute: function(gamePartido)
{
        if (gamePartido.mApplicationPartido.mGameReset)
        {
                gamePartido.mPartidoStateMachine.changeState(gamePartido.mRESET_PARTIDO_GAME);
        }
},	

exit: function(gamePartido)
{
}

});

var RESET_PARTIDO_GAME = new Class(
{
Extends: State,

initialize: function()
{
},

log: function(msg)
{
        setTimeout(function()
        {
                throw new Error(msg);
        }, 0);
},

enter: function(gamePartido)
{
},

execute: function(gamePartido)
{
	if (!gamePartido.mApplicationPartido.mGameReset)
        {
                gamePartido.mPartidoStateMachine.changeState(gamePartido.mPLAY_PARTIDO_GAME);
        }
},	

exit: function(gamePartido)
{
}

});

var BATTLE_OFF = new Class(
{
Extends: State,

initialize: function()
{
},

log: function(msg)
{
        setTimeout(function()
        {
                throw new Error(msg);
        }, 0);
},

enter: function(gamePartido)
{
},

execute: function(gamePartido)
{
 	if (gamePartido.mBattleStart)
        {
                gamePartido.mBattleStart = false;
                gamePartido.mPartidoStateMachine.changeState(gamePartido.mANSWER_QUESTION);
        }
},	

exit: function(gamePartido)
{
}

});

var ANSWER_QUESTION = new Class(
{
Extends: State,

initialize: function()
{
},

log: function(msg)
{
        setTimeout(function()
        {
                throw new Error(msg);
        }, 0);
},
/*
ApplicationPartido* app = gamePartido->mApplicationPartido;
        app->createBattleScreen();
        app->showBattleScreen();

        app->mAnswerTime = 0;

        //set mKeyArray to false
        for (int i = 0; i < 128; i++)
        {
                gamePartido->mApplicationPartido->mKeyArray[i] = false;
        }
        gamePartido->mFirstTimeExecute = true;
*/
enter: function(gamePartido)
{
	//reset text labels	
	gamePartido.mApplicationPartido.mStringAnswer = '';
	gamePartido.mApplicationPartido.mLabelAnswer.value = '';
	gamePartido.mApplicationPartido.mLabelAnswer.focus();
	
	gamePartido.mApplicationPartido.showBattleScreen();

	//gamePartido.mBattleStart = false;
        //gamePartido.mBattleEnd = false;
        //gamePartido.mCorrectAnswerStart = false;
        ///gamePartido.mApplicationPartido.mGameReset = false;

	gamePartido.mApplicationPartido.mAnswerTime = 0;
},

execute: function(gamePartido)
{
/*
	if (gamePartido.mApplicationPartido.mAnswerTime > 2) //overtime....
        {
                gamePartido.mApplicationPartido.mLabelAnswer.value = 'oot';
                gamePartido.mApplicationPartido.sendAnswer();
		gamePartido.mApplicationPartido.mAnswerTime = 0;
        }
*/

	gamePartido.mApplicationPartido.mLabelAnswer.focus();

	if (gamePartido.mCorrectAnswerStart)
        {
                gamePartido.mBattleStateMachine.changeState(gamePartido.mSHOWCORRECTANSWER_PARTIDO_GAME);
        }

	//for enter
   	if (gamePartido.mApplicationPartido.mLabelAnswer == document.activeElement)
       	{ 
		if (gamePartido.mApplicationPartido.mKey_enter == true)
		{
        		gamePartido.mApplicationPartido.sendAnswer();
			gamePartido.mApplicationPartido.mKey_enter = false;
		
			//reset text labels	
			gamePartido.mApplicationPartido.mStringAnswer = '';
			gamePartido.mApplicationPartido.mLabelQuestion.value = '';
			gamePartido.mApplicationPartido.mLabelAnswer.value = '';
		}
        }
},	

exit: function(gamePartido)
{
	this.log('GamePlayPartidoBattle::exit');
	gamePartido.mApplicationPartido.mStringAnswer = '';
        gamePartido.mApplicationPartido.hideBattleScreen();
        //gamePartido.mBattleEnd   = false;
        gamePartido.mBattleStart = false;
	
	//reset text labels	
	gamePartido.mApplicationPartido.mStringAnswer = '';
	gamePartido.mApplicationPartido.mLabelQuestion.value = '';
	gamePartido.mApplicationPartido.mLabelAnswer.value = '';
}

});

var SHOWCORRECTANSWER_PARTIDO_GAME = new Class(
{
Extends: State,

initialize: function()
{
},

log: function(msg)
{
        setTimeout(function()
        {
                throw new Error(msg);
        }, 0);
},

enter: function(gamePartido)
{
	gamePartido.mApplicationPartido.showCorrectAnswerScreen();
        gamePartido.mCorrectAnswerStart = false;
},

execute: function(gamePartido)
{
 	if (gamePartido.mCorrectAnswerEnd)
        {
                gamePartido.mPartidoStateMachine.changeState(gamePartido.mANSWER_QUESTION);
        }
},	

exit: function(gamePartido)
{
	this.log('GamePlayPartidoCorrectAnswer:exit');
 	gamePartido.mApplicationPartido.hideCorrectAnswerScreen();
        gamePartido.mCorrectAnswerStart = false;
        gamePartido.mCorrectAnswer      = false;
        gamePartido.mCorrectAnswerEnd   = false;

        //reset text box
        gamePartido.mApplicationPartido.mStringCorrectAnswer = '';
        gamePartido.mApplicationPartido.mLabelCorrectAnswer.value = '';
}

});
