#include "scenes/TestScene.hpp"
#include "scenes/GameScene.hpp"
#include "scenes/IntroScene.hpp"
#include "singletons/SinSceneManager.hpp"
#include "singletons/SinSceneStack.hpp"
#include "gameworld/GameRoadGraph.hpp"
#include "gameworld/GameRoadSystem.hpp"

using namespace viqo;
using namespace viqo::managers;
using namespace viqo::singletons;
using namespace viqo::scenes;
using namespace viqo::gameworld;


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


  // Introduce some functions
  Ogre::RenderWindow* Configure( boost::shared_ptr<Ogre::Root> );
  void SetupResources( Ogre::String );
  bool LoadScenes( boost::shared_ptr<Ogre::Root>, Ogre::RenderWindow* );


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
  INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
  int main(int argc, char *argv[])
#endif
  {
    srand( time( NULL ) );


    Ogre::String resourcesCfg = "assets/config/resources.cfg";
    Ogre::String pluginsCfg  = "assets/config/plugins.cfg";

    boost::shared_ptr<Ogre::Root> root( new Ogre::Root( pluginsCfg ) );

    Ogre::RenderWindow *window = Configure( root );
    SetupResources( resourcesCfg );

    // Load Scenes, quit if it fails.
    if( !LoadScenes( root, window ) )
      return 1;

    // Push the start scene to the stack
    std::string sceneName( "IntroScene" );
    SinSceneStack.Instance()->Push( SinSceneManager.Instance()->Get( sceneName ) );

    try
    {
      // Run always the first scene of the stack
      while( SinSceneStack.Instance()->Get( 0 )->Run() )
      {
        // Scene changed
      }
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

    delete window;

    return 0;
  }



  void SetupResources( Ogre::String resourcesCfg )
  {
    // load resource paths from config file

    Ogre::ConfigFile cf;
    cf.load( resourcesCfg );

    // go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while( seci.hasMoreElements() )
    {
      secName = seci.peekNextKey();
      Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
      Ogre::ConfigFile::SettingsMultiMap::iterator i;
      for( i = settings->begin(); i != settings->end(); ++i )
      {
        typeName = i->first;
        archName = i->second;
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
          archName, typeName, secName
        );
      }
    }
  }



  bool LoadScenes( boost::shared_ptr<Ogre::Root> root, Ogre::RenderWindow *window )
  {
    boost::shared_ptr<IntroScene> introScene( new IntroScene() );
    introScene->SetRoot( root );
    introScene->SetWindow( window );
    std::string sceneName( "IntroScene" );
    SinSceneManager.Instance()->Add( sceneName, introScene );

    boost::shared_ptr<GameScene> gameScene( new GameScene() );
    gameScene->SetRoot( root );
    gameScene->SetWindow( window );
    sceneName = std::string( "GameScene" );
    SinSceneManager.Instance()->Add( sceneName, gameScene );

    boost::shared_ptr<TestScene> testScene( new TestScene() );
    testScene->SetRoot( root );
    testScene->SetWindow( window );
    sceneName = std::string( "TestScene" );
    SinSceneManager.Instance()->Add( sceneName, testScene );

    return true;
  }



  Ogre::RenderWindow* Configure( boost::shared_ptr<Ogre::Root> root )
  {
    Ogre::RenderWindow *window = NULL;

    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if( root->showConfigDialog() )
    {
      // If returned true, user clicked OK so initialise
      // Here we choose to let the system create a default rendering window by passing 'true'
      window = root->initialise( true, "Viqo" );
    }
    return window;
  }

#ifdef __cplusplus
}
#endif

