#include "GameScene.hpp"
#include <map>

using namespace viqo::scenes;
using namespace viqo::gameworld;
using namespace viqo;

using std::string;


GameScene::GameScene()
{
}



GameScene::~GameScene()
{
  DestroyScene();
}



void GameScene::CreateScene()
{
  mSceneMgr->setAmbientLight( Ogre::ColourValue( 0,0,0 ) );
  mSceneMgr->setShadowTechnique( Ogre::SHADOWTYPE_TEXTURE_MODULATIVE );

  // How game object management's supposed to work:
  boost::shared_ptr<GameObject> goNinja( new GameObject( mSceneMgr ) );
  string goNinjaName = "Ninja";
  goNinja->Load( goNinjaName, "ninja.mesh" );
  goManager.Add( goNinjaName, goNinja );

  boost::shared_ptr<GameObject> goRobot( new GameObject( mSceneMgr ) );
  string goRobotName = "Robot";
  goRobot->Load( goRobotName, "robot.mesh" );
  goManager.Add( goRobotName, goRobot );


  // And other stuff
  Ogre::Plane plane( Ogre::Vector3::UNIT_Y, 0 );

  Ogre::MeshManager::getSingleton().createPlane( "ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                                 plane, 1500, 1500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z );

  Ogre::Entity *entGround = mSceneMgr->createEntity( "GroundEntity", "ground" );
  mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject( entGround );

  entGround->setMaterialName( "Examples/Rockwall" );
  entGround->setCastShadows( false );

  Ogre::Light* pointLight = mSceneMgr->createLight( "pointLight" );
  pointLight->setType( Ogre::Light::LT_POINT );
  pointLight->setPosition( Ogre::Vector3(0, 150, 250) );

  pointLight->setDiffuseColour( 1.0, 0.0, 0.0 );
  pointLight->setSpecularColour( 1.0, 0.0, 0.0 );


  Ogre::Light* pointLight2 = mSceneMgr->createLight( "pointLight2" );
  pointLight2->setType( Ogre::Light::LT_POINT );
  pointLight2->setPosition( Ogre::Vector3(20, 150, -250) );

  pointLight2->setDiffuseColour( 1.0, 0.0, 1.0 );
  pointLight2->setSpecularColour( 1.0, 0.0, 1.0 );
}

/*


void GameScene::DestroyScene()
{
}



void GameScene::CreateFrameListener()
{
  Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
  OIS::ParamList pl;
  size_t windowHnd = 0;
  std::ostringstream windowHndStr;

  mWindow->getCustomAttribute("WINDOW", &windowHnd);
  windowHndStr << windowHnd;
  pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

  mInputManager = OIS::InputManager::createInputSystem( pl );

  mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
  mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

  mMouse->setEventCallback(this);
  mKeyboard->setEventCallback(this);

  //Register as a Window listener
  Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

  mRoot->addFrameListener(this);
}


*/

bool GameScene::frameRenderingQueued( const Ogre::FrameEvent &evt )
{
  if( mWindow->isClosed() )
    return false;

  if( mShutDown )
    return false;

  if( mInputManager )
  {
    mKeyboard->capture();
    mMouse->capture();
  }


  std::map< string, boost::shared_ptr<GameObject> > *gos;
  std::map< string, boost::shared_ptr<GameObject> >::iterator gosIt;

  gos = goManager.GetPointer();

  for( gosIt = gos->begin(); gosIt != gos->end(); ++gosIt )
  {
    (*gosIt).second->Update( evt.timeSinceLastFrame );
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

/*


bool GameScene::keyPressed( const OIS::KeyEvent &arg )
{
  CEGUI::System &sys = CEGUI::System::getSingleton();
  sys.injectKeyDown(arg.key);
  sys.injectChar(arg.text);
  return true;
}



bool GameScene::keyReleased( const OIS::KeyEvent &arg )
{
  CEGUI::System::getSingleton().injectKeyUp(arg.key);
  return true;
}



bool GameScene::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
  CEGUI::System::getSingleton().injectMouseButtonDown( ConvertButton( id ) );
  return true;
}



bool GameScene::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
  CEGUI::System::getSingleton().injectMouseButtonUp( ConvertButton( id ) );
  return true;
}



bool GameScene::mouseMoved( const OIS::MouseEvent &arg )
{
  CEGUI::System &sys = CEGUI::System::getSingleton();
  sys.injectMouseMove( arg.state.X.rel, arg.state.Y.rel );

  if( arg.state.Z.rel )
    sys.injectMouseWheelChange(arg.state.Z.rel / 120.0f);

  return true;
}
*/

