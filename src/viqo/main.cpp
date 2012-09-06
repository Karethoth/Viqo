#include "scenes/TestScene.hpp"
#include "singletons/SinSceneManager.hpp"
#include "singletons/SinSceneStack.hpp"

using namespace viqo;
using namespace viqo::managers;
using namespace viqo::singletons;
using namespace viqo::scenes;


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


  // Introduce LoadScenes()
  bool LoadScenes();


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
  INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
  int main(int argc, char *argv[])
#endif
  {

    // Load Scenes, quit if it fails.
    if( !LoadScenes() )
      return 1;

    // Push the start scene to the stack
    std::string startSceneName = "TestScene";
    SinSceneStack.Instance()->Push( SinSceneManager.Instance()->Get( startSceneName ) );

    try
    {
      // Run the first scene of the stack
      SinSceneStack.Instance()->Get( 0 )->Run();
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


  bool LoadScenes()
  {
    boost::shared_ptr<TestScene> tScene(new TestScene());

    std::string tSceneName = "TestScene";
    SinSceneManager.Instance()->Add( tSceneName, tScene );

    return true;
  }

#ifdef __cplusplus
}
#endif

