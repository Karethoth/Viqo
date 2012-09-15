#include "GameScene.hpp"
#include <map>
#include "../gameworld/MovableGameObject.hpp"

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
  if( cam )
    delete cam;
}



void GameScene::CreateScene()
{
  mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
  CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
  CEGUI::Font::setDefaultResourceGroup("Fonts");
  CEGUI::Scheme::setDefaultResourceGroup("Schemes");
  CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
  CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

  CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
  CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");

  CEGUI::MouseCursor::getSingleton().setImage( "TaharezLook", "MouseArrow" );

  //CEGUI::Window *guiRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout("TextDemo.layout");
  //CEGUI::System::getSingleton().setGUISheet(guiRoot);


  mSceneMgr->setAmbientLight( Ogre::ColourValue( 0,0,0 ) );
  mSceneMgr->setShadowTechnique( Ogre::SHADOWTYPE_TEXTURE_MODULATIVE );

  // How game object management's supposed to work:
  boost::shared_ptr<GameObject> goNinja( new MovableGameObject() );
  string goNinjaName = "Ninja";
  Ogre::SceneNode *ninjaNode = mSceneMgr->getRootSceneNode()->
                               createChildSceneNode( goNinjaName+"Node" );
  goNinja->Init( mSceneMgr, ninjaNode );
  goNinja->Load( goNinjaName, "ninja.mesh" );
  goManager.Add( goNinjaName, goNinja );

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



void GameScene::CreateCamera()
{
  cam = new GameCamera( mSceneMgr );

  cam->GetCamera()->setPosition( Ogre::Vector3( 0,300,900 ) );

  cam->GetCamera()->lookAt( Ogre::Vector3( 0,0,0 ) );
  cam->GetCamera()->setNearClipDistance( 5 );
}



void GameScene::CreateViewports()
{
  // Create one viewport, entire window
  Ogre::Viewport* vp = mWindow->addViewport( cam->GetCamera() );
  vp->setBackgroundColour( Ogre::ColourValue( 0,0,0 ) );

  // Alter the camera aspect ratio to match the viewport
  cam->GetCamera()->setAspectRatio
  (
    Ogre::Real( vp->getActualWidth() ) / Ogre::Real( vp->getActualHeight() )
  );
}



void GameScene::DestroyScene()
{
}



void GameScene::CreateFrameListener()
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

  mTrayMgr = new OgreBites::SdkTrayManager( "InterfaceName", mWindow, mMouse, (OgreBites::SdkTrayListener*)this );
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

  cam->Update( evt.timeSinceLastFrame );
  mTrayMgr->frameRenderingQueued( evt );

  if( !mTrayMgr->isDialogVisible() )
  {
    if( mDetailsPanel->isVisible() )
    {
      mDetailsPanel->setParamValue( 0, Ogre::StringConverter::toString(cam->GetCamera()->getDerivedPosition().x) );
      mDetailsPanel->setParamValue( 1, Ogre::StringConverter::toString(cam->GetCamera()->getDerivedPosition().y) );
      mDetailsPanel->setParamValue( 2, Ogre::StringConverter::toString(cam->GetCamera()->getDerivedPosition().z) );
      mDetailsPanel->setParamValue( 4, Ogre::StringConverter::toString(cam->GetCamera()->getDerivedOrientation().w) );
      mDetailsPanel->setParamValue( 5, Ogre::StringConverter::toString(cam->GetCamera()->getDerivedOrientation().x) );
      mDetailsPanel->setParamValue( 6, Ogre::StringConverter::toString(cam->GetCamera()->getDerivedOrientation().y) );
      mDetailsPanel->setParamValue( 7, Ogre::StringConverter::toString(cam->GetCamera()->getDerivedOrientation().z) );
    }
  }

  return true;
}



bool GameScene::keyPressed( const OIS::KeyEvent &arg )
{
  CEGUI::System &sys = CEGUI::System::getSingleton();
  sys.injectKeyDown(arg.key);
  sys.injectChar(arg.text);
  cam->keyPressed( arg );

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

  return true;
}



bool GameScene::keyReleased( const OIS::KeyEvent &arg )
{
  cam->keyReleased( arg );
  return true;
}



bool GameScene::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
  CEGUI::System::getSingleton().injectMouseButtonDown( ConvertButton( id ) );
  cam->mousePressed( arg, id );

  if( id == OIS::MB_Right )
  {
    CEGUI::Point mousePos = CEGUI::MouseCursor::getSingleton().getPosition();
    Ogre::Ray mouseRay    = cam->GetCamera()->getCameraToViewportRay
    (
      mousePos.d_x/float(arg.state.width),
      mousePos.d_y/float(arg.state.height)
    );

    Ogre::RaySceneQuery *rayQuery;
    rayQuery = mSceneMgr->createRayQuery( mouseRay );

    Ogre::RaySceneQueryResult &result = rayQuery->execute();
    Ogre::RaySceneQueryResult::iterator itr = result.begin();

    if( itr != result.end() )
    {
      boost::shared_ptr<GameObject> goNinja = goManager.Get( "Ninja" );
      if( goNinja )
      {
        boost::shared_ptr<MovableGameObject> ninja = boost::static_pointer_cast<MovableGameObject>( goNinja );
        Ogre::Vector3 pos = itr->movable->getParentSceneNode()->getPosition();
        ninja->walkList.push_back( Ogre::Vector3(pos.x,0,pos.z) );
      }
    }

    delete rayQuery;
  }
  return true;
}



bool GameScene::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
  CEGUI::System::getSingleton().injectMouseButtonUp( ConvertButton( id ) );
  cam->mouseReleased( arg, id );
  return true;
}



bool GameScene::mouseMoved( const OIS::MouseEvent &arg )
{
  CEGUI::System &sys = CEGUI::System::getSingleton();
  sys.injectMouseMove( arg.state.X.rel, arg.state.Y.rel );

  if( arg.state.Z.rel )
    sys.injectMouseWheelChange(arg.state.Z.rel / 120.0f);

  cam->mouseMoved( arg );
  return true;
}

