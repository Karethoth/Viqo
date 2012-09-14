#include "Scene.hpp"

using namespace viqo;


CEGUI::MouseButton viqo::ConvertButton( OIS::MouseButtonID buttonID )
{
  switch( buttonID )
  {
    case OIS::MB_Left:
      return CEGUI::LeftButton;

    case OIS::MB_Right:
      return CEGUI::RightButton;

    case OIS::MB_Middle:
      return CEGUI::MiddleButton;

    default:
      return CEGUI::LeftButton;
  }
}




Scene::Scene( void ) : mCamera( 0 ),
                       mSceneMgr( 0 ),
                       mWindow( 0 ),
                       mRenderer( 0 ),
                       mTrayMgr( 0 ),
                       mCameraMan( 0 ),
                       mDetailsPanel( 0 ),
                       mCursorWasVisible( false ),
                       mShutDown( false ),
                       mInputManager( 0 ),
                       mMouse( 0 ),
                       mKeyboard( 0 )
{
}



Scene::~Scene()
{
  DestroyViewports();
  DestroyScene();
  if( mTrayMgr )
    delete mTrayMgr;

  if( mCameraMan )
    delete mCameraMan;

  //Remove ourself as a Window listener
  Ogre::WindowEventUtilities::removeWindowEventListener( mWindow, this );
}



void Scene::ChooseSceneManager()
{
  // Get the SceneManager, in this case a generic one
  mSceneMgr = mRoot->createSceneManager( Ogre::ST_GENERIC );
}



void Scene::CreateCamera()
{
  // Create the camera
  mCamera = mSceneMgr->createCamera( "PlayerCam" );

  // Position it at 500 in Z direction
  mCamera->setPosition( Ogre::Vector3( 0,10,500 ) );

  // Look back along -Z
  mCamera->lookAt( Ogre::Vector3( 0,0,0 ) );
  mCamera->setNearClipDistance( 5 );

  mCameraMan = new OgreBites::SdkCameraMan( mCamera );   // create a default camera controller
}



void Scene::CreateFrameListener()
{
  Ogre::LogManager::getSingletonPtr()->logMessage( "*** Initializing OIS ***" );
  OIS::ParamList pl;
  size_t windowHnd = 0;
  std::ostringstream windowHndStr;

  mWindow->getCustomAttribute( "WINDOW", &windowHnd );
  windowHndStr << windowHnd;
  pl.insert( std::make_pair( std::string("WINDOW"), windowHndStr.str() ) );

  mInputManager = OIS::InputManager::createInputSystem( pl );

  mKeyboard = static_cast<OIS::Keyboard*>( mInputManager->createInputObject( OIS::OISKeyboard, true ) );
  mMouse = static_cast<OIS::Mouse*>( mInputManager->createInputObject( OIS::OISMouse, true ) );

  mMouse->setEventCallback( this );
  mKeyboard->setEventCallback( this );

  //Set initial mouse clipping size
  windowResized( mWindow );

  //Register as a Window listener
  Ogre::WindowEventUtilities::addWindowEventListener( mWindow, this );

  mTrayMgr = new OgreBites::SdkTrayManager( "InterfaceName", mWindow, mMouse, this );
  mTrayMgr->showFrameStats( OgreBites::TL_TOPLEFT );
  mTrayMgr->hideCursor();

  // create a params panel for displaying sample details
  Ogre::StringVector items;
  items.push_back( "cam.pX" );
  items.push_back( "cam.pY" );
  items.push_back( "cam.pZ" );
  items.push_back( "" );
  items.push_back( "cam.oW" );
  items.push_back( "cam.oX" );
  items.push_back( "cam.oY" );
  items.push_back( "cam.oZ" );
  items.push_back( "" );
  items.push_back( "Filtering" );
  items.push_back( "Poly Mode" );

  mDetailsPanel = mTrayMgr->createParamsPanel( OgreBites::TL_NONE, "DetailsPanel", 200, items );
  mDetailsPanel->setParamValue( 9, "Bilinear" );
  mDetailsPanel->setParamValue( 10, "Solid" );
  mDetailsPanel->hide();

  mRoot->addFrameListener( this );
}



void Scene::DestroyScene()
{
}



void Scene::CreateViewports()
{
  // Create one viewport, entire window
  Ogre::Viewport* vp = mWindow->addViewport( mCamera );
  vp->setBackgroundColour( Ogre::ColourValue( 0,0,0 ) );

  // Alter the camera aspect ratio to match the viewport
  mCamera->setAspectRatio(
    Ogre::Real( vp->getActualWidth() ) / Ogre::Real( vp->getActualHeight() )
  );
}



void Scene::DestroyViewports()
{
  mWindow->removeAllViewports();
}


void Scene::CreateResourceListener()
{
}



void Scene::LoadResources()
{
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}



bool Scene::Run()
{

  if( !Setup() )
    return false;

  mRoot->startRendering();

  return !mShutDown;
}



void Scene::Stop()
{
  mRoot->queueEndRendering();
  mRoot->removeFrameListener( this );
  DestroyViewports();
  WindowClosed( mWindow );
  DestroyScene();
}



bool Scene::Setup()
{
  if( !mWindow )
    return false;

  ChooseSceneManager();
  CreateCamera();
  CreateViewports();

  // Set default mipmap level (NB some APIs ignore this)
  Ogre::TextureManager::getSingleton().setDefaultNumMipmaps( 5 );

  // Create any resource listeners (for loading screens)
  CreateResourceListener();

  // Load resources
  LoadResources();

  // Create the scene
  CreateScene();

  CreateFrameListener();

  return true;
};



bool Scene::frameRenderingQueued( const Ogre::FrameEvent& evt )
{
  if( mWindow->isClosed() )
    return false;

  if( mShutDown )
  {
    Stop();
    return false;
  }

  if( mInputManager )
  {
    mKeyboard->capture();
    mMouse->capture();
  }

  mTrayMgr->frameRenderingQueued( evt );

  if( !mTrayMgr->isDialogVisible() )
  {
    mCameraMan->frameRenderingQueued( evt );   // if dialog isn't up, then update the camera
    if( mDetailsPanel->isVisible() )   // if details panel is visible, then update its contents
    {
      mDetailsPanel->setParamValue( 0, Ogre::StringConverter::toString(mCamera->getDerivedPosition().x) );
      mDetailsPanel->setParamValue( 1, Ogre::StringConverter::toString(mCamera->getDerivedPosition().y) );
      mDetailsPanel->setParamValue( 2, Ogre::StringConverter::toString(mCamera->getDerivedPosition().z) );
      mDetailsPanel->setParamValue( 4, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().w) );
      mDetailsPanel->setParamValue( 5, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().x) );
      mDetailsPanel->setParamValue( 6, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().y) );
      mDetailsPanel->setParamValue( 7, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().z) );
    }
  }

  return true;
}



bool Scene::keyPressed( const OIS::KeyEvent &arg )
{
  if( mTrayMgr->isDialogVisible() )
    return true;   // don't process any more keys if dialog is up

  if( arg.key == OIS::KC_F )   // toggle visibility of advanced frame stats
  {
    mTrayMgr->toggleAdvancedFrameStats();
  }
  else if( arg.key == OIS::KC_G )   // toggle visibility of even rarer debugging details
  {
    if( mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE )
    {
      mTrayMgr->moveWidgetToTray( mDetailsPanel, OgreBites::TL_TOPRIGHT, 0 );
      mDetailsPanel->show();
    }
    else
    {
      mTrayMgr->removeWidgetFromTray( mDetailsPanel );
      mDetailsPanel->hide();
    }
  }
  else if( arg.key == OIS::KC_F5 )   // refresh all textures
  {
    Ogre::TextureManager::getSingleton().reloadAll();
  }
  else if( arg.key == OIS::KC_SYSRQ )   // take a screenshot
  {
    mWindow->writeContentsToTimestampedFile( "screenshot", ".jpg" );
  }
  else if( arg.key == OIS::KC_ESCAPE )
  {
    mShutDown = true;
  }

  //mCameraMan->injectKeyDown( arg );
  return true;
}



bool Scene::keyReleased( const OIS::KeyEvent &arg )
{
  //mCameraMan->injectKeyUp( arg );
  return true;
}



bool Scene::mouseMoved( const OIS::MouseEvent &arg )
{
  if( mTrayMgr->injectMouseMove( arg ) )
    return true;

  //mCameraMan->injectMouseMove( arg );
  return true;
}



bool Scene::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
  if( mTrayMgr->injectMouseDown( arg, id ) )
    return true;

  //mCameraMan->injectMouseDown( arg, id );
  return true;
}

bool Scene::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
  if( mTrayMgr->injectMouseUp( arg, id ) )
    return true;

  //mCameraMan->injectMouseUp( arg, id );
  return true;
}



//Adjust mouse clipping area
void Scene::windowResized( Ogre::RenderWindow* rw )
{
  unsigned int width, height, depth;
  int left, top;
  rw->getMetrics( width, height, depth, left, top );

  const OIS::MouseState &ms = mMouse->getMouseState();
  ms.width = width;
  ms.height = height;
}



//Unattach OIS before window shutdown (very important under Linux)
void Scene::WindowClosed( Ogre::RenderWindow* rw )
{
  //Only close for window that created OIS (the main window in these demos)
  if( rw == mWindow )
  {
    if( mInputManager )
    {
      mInputManager->destroyInputObject( mMouse );
      mInputManager->destroyInputObject( mKeyboard );

      OIS::InputManager::destroyInputSystem( mInputManager );
      mInputManager = 0;
    }
  }
}



void Scene::SetRoot( boost::shared_ptr<Ogre::Root> root )
{
  mRoot = root;
}



void Scene::SetWindow( Ogre::RenderWindow *window )
{
  mWindow = window;
}

