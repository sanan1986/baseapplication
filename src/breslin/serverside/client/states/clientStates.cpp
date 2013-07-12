//parent
#include "clientStates.h"

//log
#include "../../tdreamsock/dreamSockLog.h"

//states
#include "../../../fsm/stateMachine.h"

//ability
#include "../client.h"


/*****************************************
*******       GLOBAL    ******************
****************************************/
GlobalClient* GlobalClient::Instance()
{
  static GlobalClient instance;
  return &instance;
}
void GlobalClient::enter(Client* client)
{
}
void GlobalClient::execute(Client* client)
{

}
void GlobalClient::exit(Client* client)
{
}
bool GlobalClient::onMessage(Client* client, const Telegram& msg)
{
        return true;
}


/*****************************************
*******       CLIENT STATES    ******************	
****************************************/

/*****************************************
	Logged_Out
****************************************/
Logged_Out* Logged_Out::Instance()
{
  static Logged_Out instance;
  return &instance;
}
void Logged_Out::enter(Client* client)
{
}
void Logged_Out::execute(Client* client)
{

}
void Logged_Out::exit(Client* client)
{
}
bool Logged_Out::onMessage(Client* client, const Telegram& msg)
{
	return true;
}

/*****************************************
	Lobby
****************************************/
Lobby* Lobby::Instance()
{
  static Lobby instance;
  return &instance;
}
void Lobby::enter(Client* client)
{
}
void Lobby::execute(Client* client)
{

}
void Lobby::exit(Client* client)
{
}
bool Lobby::onMessage(Client* client, const Telegram& msg)
{
	return true;
}

/*****************************************
		GAME_MODE		
****************************************/
Game_Mode* Game_Mode::Instance()
{
  static Game_Mode instance;
  return &instance;
}
void Game_Mode::enter(Client* client)
{
}
void Game_Mode::execute(Client* client)
{

}
void Game_Mode::exit(Client* client)
{
}
bool Game_Mode::onMessage(Client* client, const Telegram& msg)
{
	return true;
}

/*****************************************
*******       CONTROL    ******************	
****************************************/

/*****************************************
	Human
****************************************/
Human* Human::Instance()
{
  static Human instance;
  return &instance;
}
void Human::enter(Client* client)
{
}
void Human::execute(Client* client)
{

}
void Human::exit(Client* client)
{
}
bool Human::onMessage(Client* client, const Telegram& msg)
{
	return true;
}


/*****************************************
	Computer
****************************************/
Computer_Mode* Computer_Mode::Instance()
{
	static Computer_Mode instance;
	return &instance;
}
void Computer_Mode::enter(Client* client)
{
	LogString("enter computer");
}
void Computer_Mode::execute(Client* client)
{
	//client->mClientStateMachine->changeState(Human::Instance());
	//client->checkForTimeout();
}
void Computer_Mode::exit(Client* client)
{
}
bool Computer_Mode::onMessage(Client* client, const Telegram& msg)
{
	return true;
}


/*****************************************
*******       PERMANENCE    ******************	
****************************************/
/*****************************************
        INITIALIZE_PERMANENCE
****************************************/
Initialize_Permanence* Initialize_Permanence::Instance()
{
  static Initialize_Permanence instance;
  return &instance;
}
void Initialize_Permanence::enter(Client* client)
{
}
void Initialize_Permanence::execute(Client* client)
{
	if (client->mPermanence)
	{
		client->mPermanenceStateMachine->changeState(Permanent::Instance());
	}
	else
	{
		client->mPermanenceStateMachine->changeState(Temporary::Instance());
	}

}
void Initialize_Permanence::exit(Client* client)
{
}
bool Initialize_Permanence::onMessage(Client* client, const Telegram& msg)
{
        return true;
}

/*****************************************
	PERMANENT	
****************************************/
Permanent* Permanent::Instance()
{
  static Permanent instance;
  return &instance;
}
void Permanent::enter(Client* client)
{
	LogString("enter");
}
void Permanent::execute(Client* client)
{
}
void Permanent::exit(Client* client)
{
}
bool Permanent::onMessage(Client* client, const Telegram& msg)
{
	return true;
}

/*****************************************
	TEMPORARY	
****************************************/
Temporary* Temporary::Instance()
{
  static Temporary instance;
  return &instance;
}
void Temporary::enter(Client* client)
{
	LogString("enter");
}
void Temporary::execute(Client* client)
{
}
void Temporary::exit(Client* client)
{
}
bool Temporary::onMessage(Client* client, const Telegram& msg)
{
	return true;
}
