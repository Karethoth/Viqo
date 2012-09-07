#include "IntroScene.hpp"
#include "../singletons/SinSceneManager.hpp"
#include "../singletons/SinSceneStack.hpp"

using namespace viqo::scenes;
using namespace viqo::singletons;


IntroScene::IntroScene()
{
  sceneLengthMs = 5000;
  timer = new Ogre::Timer();
}



IntroScene::~IntroScene()
{
  DestroyScene();
  delete timer;
}



void IntroScene::CreateScene()
{
  Ogre::Light* pointLight = mSceneMgr->createLight( "pointLight" );
  pointLight->setType( Ogre::Light::LT_POINT );
  pointLight->setPosition( Ogre::Vector3(0, 150, 250) );

  pointLight->setDiffuseColour( 1.0, 0.0, 0.0 );
  pointLight->setSpecularColour( 1.0, 0.0, 0.0 );

  Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("Background", "General");
  material->getTechnique(0)->getPass(0)->createTextureUnitState("rockwall.tga");
  material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
  material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
  material->getTechnique(0)->getPass(0)->setLightingEnabled(false);

  // Create background rectangle covering the whole screen
  Ogre::Rectangle2D* rect = new Ogre::Rectangle2D(true);
  rect->setCorners(-1.0, 1.0, 1.0, -1.0);
  rect->setMaterial("Background");

  // Render the background before everything else
  rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);

  // Use infinite AAB to always stay visible
  Ogre::AxisAlignedBox aabInf;
  aabInf.setInfinite();
  rect->setBoundingBox(aabInf);

  // Attach background to the scene
  Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Background");
  node->attachObject( rect );

  timer->reset();
}



void IntroScene::DestroyScene()
{
}



void IntroScene::CreateFrameListener()
{
  Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
  OIS::ParamList pl;
  size_t windowHnd = 0;
  std::ostringstream windowHndStr;

  mWindow->getCustomAttribute( "WINDOW", &windowHnd );
  windowHndStr << windowHnd;
  pl.insert( std::make_pair( std::string("WINDOW"), windowHndStr.str() ) );

  mInputManager = OIS::InputManager::createInputSystem( pl );

  mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
  mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

  mMouse->setEventCallback( this );
  mKeyboard->setEventCallback( this );

  //Set initial mouse clipping size
  windowResized( mWindow );

  //Register as a Window listener
  Ogre::WindowEventUtilities::addWindowEventListener( mWindow, this );

  mRoot->addFrameListener(this);

}



bool IntroScene::frameRenderingQueued( const Ogre::FrameEvent &evt )
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

  if( timer->getMilliseconds() >= sceneLengthMs )
  {
    std::cout << "CHANGING SCENE!\n";
    SinSceneStack.Instance()->Pop();
    SinSceneStack.Instance()->Push( SinSceneManager.Instance()->Get( "TestScene" ) );
    Stop();
    return false;
  }

  return true;
}




bool IntroScene::keyPressed( const OIS::KeyEvent &arg )
{
  return true;
}



bool IntroScene::keyReleased( const OIS::KeyEvent &arg )
{
  return true;
}



bool IntroScene::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
  return true;
}



bool IntroScene::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
  return true;
}



bool IntroScene::mouseMoved( const OIS::MouseEvent &arg )
{
  return true;
}

