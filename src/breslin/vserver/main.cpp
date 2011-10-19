#ifdef WIN32
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif

#include <stdio.h>
#include <string.h>

#include <windows.h>

#endif

#ifdef WIN32
#include <shellapi.h>

#else
#include <signal.h>
#include <syslog.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#endif

//possible games
#include "../serverside/game/game.h"
#include "../serverside/game/gameTag.h"
#include "../serverside/game/gameGhostInTheGraveyard.h"

#include "../serverside/server/server.h"
#include "../serverside/network/network.h"
#include "../serverside/tdreamsock/dreamSockLog.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// WIN32 only
#ifdef WIN32

// UNIX only
#else
int runningDaemon;
#endif

Game* game;

#ifdef WIN32

//-----------------------------------------------------------------------------
// Name: empty()
// Desc: 
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND WindowhWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	// Process Messages
	switch(Message)
	{
	case WM_CREATE:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	return DefWindowProc(WindowhWnd, Message, wParam, lParam);
}

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Windows app start position
//-----------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS WinClass;

	WinClass.style			= 0;
	WinClass.lpfnWndProc	= WindowProc;
	WinClass.cbClsExtra		= 0;
	WinClass.cbWndExtra		= 0;
	WinClass.hInstance		= hInstance;
	WinClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	WinClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	WinClass.hbrBackground	= (HBRUSH) (COLOR_MENU);
	WinClass.lpszMenuName	= 0;
	WinClass.lpszClassName	= "WINCLASS1";

	if(!RegisterClass(&WinClass))
		return 0;

	HWND hwnd = CreateWindow(WinClass.lpszClassName,
						"dreamSock server",
						WS_OVERLAPPEDWINDOW | WS_VISIBLE,
						320,
						240,
						320, 240,
						NULL,
						NULL,
						hInstance,
						NULL);

	ShowWindow(hwnd, SW_HIDE);

	StartLogConsole();

	const char* cmdLine = lpCmdLine;

	const char* aGame    = "1";
	const char* aGameTag = "2";
	const char* aGameGhostInTheGraveyard = "3";

	if (strcmp (cmdLine,aGame) == 0)
	{
		game = new Game();
	}

	if (strcmp (cmdLine,aGameTag) == 0)
	{
		game = new GameTag();
	}

	if (strcmp (cmdLine,aGameGhostInTheGraveyard) == 0)
	{
		game = new GameGhostInTheGraveyard();
	}
	
	MSG WinMsg;
	bool done = false;
	int time, oldTime, newTime;

	// first peek the message without removing it
	PeekMessage(&WinMsg, hwnd, 0, 0, PM_NOREMOVE);

	oldTime = game->mServer->mNetwork->dreamSock_GetCurrentSystemTime();

	try
	{
		while(!done)
		{
			while (PeekMessage(&WinMsg, NULL, 0, 0, PM_NOREMOVE))
			{
				if(!GetMessage(&WinMsg, NULL, 0, 0))
				{
					game->mServer->mNetwork->dreamSock_Shutdown();

					done = true;
				}

				TranslateMessage(&WinMsg);
   				DispatchMessage(&WinMsg);
			}

			do
			{
				newTime = game->mServer->mNetwork->dreamSock_GetCurrentSystemTime();
				time = newTime - oldTime;
			} while (time < 1);
			
			game->frame(time);
			

			oldTime = newTime;
		}
	}
	catch(...)
	{
		LogString("Unknown Exception caught in main loop");

		game->mServer->mNetwork->dreamSock_Shutdown();

		MessageBox(NULL, "Unknown Exception caught in main loop", "Error", MB_OK | MB_TASKMODAL);

		return -1;
	}

	return WinMsg.wParam;
}

#else

//-----------------------------------------------------------------------------
// Name: daemonInit()
// Desc: Initialize UNIX daemon
//-----------------------------------------------------------------------------
static int daemonInit(void)
{
	printf("Running daemon...\n\n");

	runningDaemon = 1;

	pid_t pid;

	if((pid = fork()) < 0)
	{
		return -1;
	}
	else if(pid != 0)
	{
		exit(0);
	}

	setsid();

	umask(0);

	close(1);
	close(2);
	close(3);

	return 0;
}

//-----------------------------------------------------------------------------
// Name: keyPress()
// Desc: Check for a keypress
//-----------------------------------------------------------------------------
int keyPress(void)
{
	static char keypressed;
	struct timeval waittime;
	int num_chars_read;
	fd_set mask;
//	struct fd_set mask;
	FD_SET(0, &mask);

	waittime.tv_sec = 0;
	waittime.tv_usec = 0;

	if(select(1, &mask, 0, 0, &waittime))
	{
		num_chars_read = read(0, &keypressed, 1);

		if(num_chars_read == 1) 
			return ((int) keypressed);
	}

	return (-1);
}

//-----------------------------------------------------------------------------
// Name: main()
// Desc: UNIX app start position
//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{

	const char* aGame    = "1";
	const char* aGameTag = "2";

	if (strcmp (argv[1],aGame) == 0)
	{
		game = new Game();
	}

	if (strcmp (argv[1],aGameTag) == 0)
	{
		game = new GameTag();
	}

	LogString("Welcome to Army War Server v2.0");
	LogString("-------------------------------\n");



	if(argc > 1)
	{
		if(strcmp(argv[1], "-daemon") == 0)
		{
			daemonInit();
		}
	}

	// Ignore the SIGPIPE signal, so the program does not terminate if the
	// pipe gets broken
	signal(SIGPIPE, SIG_IGN);

	LogString("Init successful");

	int time, oldTime, newTime;

	oldTime = game->mServer->mNetwork->dreamSock_GetCurrentSystemTime();

	// App main loop
	try
	{
		if(runningDaemon)
		{
			// Keep server alive
			while(1)
			{
				do
				{
					newTime = game->mServer->mNetwork->dreamSock_GetCurrentSystemTime();
					time = newTime - oldTime;
				} while (time < 1);


				oldTime = newTime;
			}
		}
		else
		{
			// Keep server alive (wait for keypress to kill it)
			while(keyPress() == -1)
			{
				do
				{
					newTime = game->mServer->mNetwork->dreamSock_GetCurrentSystemTime();
					time = newTime - oldTime;
				} while (time < 1);

				game->frame(time);

				oldTime = newTime;
			}
		}
	}
	catch(...)
	{

		game->mServer->mNetwork->dreamSock_Shutdown();

		LogString("Unknown Exception caught in main loop");

		return -1;
	}

	LogString("Shutting down everything");


	game->mServer->mNetwork->dreamSock_Shutdown();

	return 0;
}

#endif
