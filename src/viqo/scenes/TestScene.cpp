#include "TestScene.hpp"

using namespace viqo::scenes;


TestScene::TestScene()
{
}



TestScene::~TestScene()
{
  DestroyScene();
}



void TestScene::CreateScene()
{
  mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
  CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
  CEGUI::Font::setDefaultResourceGroup("Fonts");
  CEGUI::Scheme::setDefaultResourceGroup("Schemes");
  CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
  CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

  CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
  CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");

  CEGUI::Window *guiRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout("TextDemo.layout");
  CEGUI::System::getSingleton().setGUISheet(guiRoot);

  mSceneMgr->setAmbientLight( Ogre::ColourValue( 0,0,0 ) );
  mSceneMgr->setShadowTechnique( Ogre::SHADOWTYPE_TEXTURE_MODULATIVE );

  Ogre::Entity *entNinja = mSceneMgr->createEntity( "Ninja", "ninja.mesh" );
  entNinja->setCastShadows( true );
  mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject( entNinja );

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
}



void TestScene::DestroyScene()
{
}



void TestScene::CreateFrameListener()
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



bool TestScene::frameRenderingQueued( const Ogre::FrameEvent &evt )
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

  CEGUI::System::getSingleton().injectTimePulse( evt.timeSinceLastFrame );

  return true;
}




bool TestScene::keyPressed( const OIS::KeyEvent &arg )
{
  CEGUI::System &sys = CEGUI::System::getSingleton();
  sys.injectKeyDown(arg.key);
  sys.injectChar(arg.text);
  return true;
}



bool TestScene::keyReleased( const OIS::KeyEvent &arg )
{
  CEGUI::System::getSingleton().injectKeyUp(arg.key);
  return true;
}



bool TestScene::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
  CEGUI::System::getSingleton().injectMouseButtonDown( ConvertButton( id ) );
  return true;
}



bool TestScene::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
  CEGUI::System::getSingleton().injectMouseButtonUp( ConvertButton( id ) );
  return true;
}



bool TestScene::mouseMoved( const OIS::MouseEvent &arg )
{
  CEGUI::System &sys = CEGUI::System::getSingleton();
  sys.injectMouseMove( arg.state.X.rel, arg.state.Y.rel );

  if( arg.state.Z.rel )
    sys.injectMouseWheelChange(arg.state.Z.rel / 120.0f);

  return true;
}

