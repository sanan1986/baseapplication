
//log
#include "../clientside/tdreamsock/dreamSockLog.h"

//app
#include "../clientside/game/applicationOgre.h"

//graphics
#include "../clientside/graphics/graphicsOgre.h"

/******************** MAIN ***********************/
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
       
		StartLogConsole();
        
		Application* applicationOgre = new Application(strCmdLine,30004);
#else
        Application* applicationOgre = new Application(argv[1],30004);
#endif
        try
		{
			applicationOgre->go();
            applicationOgre->gameLoop();
        }
		catch( Ogre::Exception& e )
		{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }
        return 0;
    }

#ifdef __cplusplus
}
#endif

