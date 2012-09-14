#include "GameCamera.hpp"

using namespace viqo;


GameCamera::GameCamera( Ogre::SceneManager *sMan )
{
  sceneMan = sMan;
  camNode  = sceneMan->getRootSceneNode()->createChildSceneNode( "GameCamera" );
  cam      = sceneMan->createCamera( "GameCamera" );

  camNode->attachObject( cam );

  upKey    = false;
  downKey  = false;
  leftKey  = false;
  rightKey = false;
}



GameCamera::~GameCamera()
{
  camNode->detachAllObjects();
  delete cam;
  sceneMan->destroySceneNode( "GameCamera" );
}



Ogre::Camera *GameCamera::GetCamera()
{
  return cam;
}



void GameCamera::Update( Ogre::Real timeSinceLastFrame )
{
  if( upKey )
    velocity.z -= 5.0;
  if( downKey )
    velocity.z += 5.0;
  if( rightKey )
    velocity.x += 5.0;
  if( leftKey )
    velocity.x -= 5.0;

  velocity.x *= 0.9;
  velocity.z *= 0.9;
  velocity.y *= 0.98;

  cam->pitch( Ogre::Radian( Ogre::Degree( -velocity.y/10 ) ) );

  camNode->translate( velocity );
}



bool GameCamera::keyPressed( const OIS::KeyEvent &arg )
{
  switch( arg.key )
  {
    case( OIS::KC_UP ):
    case( OIS::KC_W ):
      upKey = true;
      break;

    case( OIS::KC_DOWN ):
    case( OIS::KC_S ):
      downKey = true;
      break;

    case( OIS::KC_RIGHT ):
    case( OIS::KC_D ):
      rightKey = true;
      break;

    case( OIS::KC_LEFT ):
    case( OIS::KC_A ):
      leftKey = true;
      break;
  }
  return true;
}



bool GameCamera::keyReleased( const OIS::KeyEvent &arg )
{
  switch( arg.key )
  {
    case( OIS::KC_UP ):
    case( OIS::KC_W ):
      upKey = false;
      break;

    case( OIS::KC_DOWN ):
    case( OIS::KC_S ):
      downKey = false;
      break;

    case( OIS::KC_RIGHT ):
    case( OIS::KC_D ):
      rightKey = false;
      break;

    case( OIS::KC_LEFT ):
    case( OIS::KC_A ):
      leftKey = false;
      break;
  }

  return true;
}



bool GameCamera::mouseMoved( const OIS::MouseEvent &arg )
{
  if( arg.state.Z.rel )
    velocity.y -= arg.state.Z.rel/120;

  return true;
}



bool GameCamera::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
  return true;
}



bool GameCamera::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
  return true;
}

