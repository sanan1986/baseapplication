#include "serverPartido.h"

//log
#include "../tdreamsock/dreamSockLog.h"

#include "../game/gamePartido.h"
#include "../client/clientPartido.h"

ServerPartido::ServerPartido(Ogre::Root* root, const char *localIP, int serverPort) 
:
 Server(root, localIP, serverPort)
{
	//get schools
	getSchools();

	//get questions
        getQuestions();

	//create game partido!
 	mGameVector.push_back(new Game(this,2));
}

ServerPartido::~ServerPartido()
{
}

void ServerPartido::parsePacket(Message *mes, struct sockaddr *address)
{
        ClientPartido* client;
        mes->BeginReading();

        int type = mes->ReadByte();

        /***CONNECT********/
        //this should just create a client then client should do what need be done.
        
	if (type == mMessageConnect || type == mMessageConnectBrowser || type == mMessageConnectNode)
	{
		if (type == mMessageConnect)
        	{
                	ClientPartido* client = new ClientPartido(this, address, 0);
        	}

        	else if (type == mMessageConnectBrowser)
        	{
                	int clientID = mes->ReadByte();
                	ClientPartido* client = new ClientPartido(this, address, clientID);
        	}

        	else if (type == mMessageConnectNode)
        	{
                	int clientID = mes->ReadByte();
                	ClientPartido* client = new ClientPartido(this, address, -1);
        	}     	 
	}
	else
	{
		Server::parsePacket(mes,address);
	}
}

void ServerPartido::getQuestions()
{
        PGconn          *conn;
        PGresult        *res;
        int             rec_count;
        int             row;
        int             col;
        conn = PQconnectdb("dbname=abcandyou host=localhost user=postgres password=mibesfat");
        res = PQexec(conn,
       "select * from questions ORDER BY id");
        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
                puts("We did not get any data!");
                //exit(0);
        }
        rec_count = PQntuples(res);
        printf("We received %d records.\n", rec_count);
        for (row=0; row<rec_count; row++)
        {
                const char* a = PQgetvalue(res, row, 0);
                std::string aString(a);
                mQuestionIDVector.push_back(aString);

                const char* b = PQgetvalue(res, row, 1);
                std::string bString(b);
                mQuestionsVector.push_back(bString);

                const char* c = PQgetvalue(res, row, 2);
                std::string cString(c);
                mAnswersVector.push_back(cString);

                const char* d = PQgetvalue(res, row, 3);
                std::string dString(d);
                mLevelsVector.push_back(dString);
        }

        PQclear(res);

        PQfinish(conn);
}

void ServerPartido::getSchools()
{
        PGconn          *conn;
        PGresult        *res;
        int             rec_count;
        int             row;
        int             col;
        conn = PQconnectdb("dbname=abcandyou host=localhost user=postgres password=mibesfat");
        res = PQexec(conn,
       "select * from schools");
        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
                puts("We did not get any data!");
                //exit(0);
        }
        rec_count = PQntuples(res);
        printf("We received %d records.\n", rec_count);
        for (row=0; row<rec_count; row++)
        {
                const char* c = PQgetvalue(res, row, 1);
                std::string school(c);

                mSchoolVector.push_back(school);
		LogString("adding school..");
        }

        PQclear(res);

        PQfinish(conn);
}

