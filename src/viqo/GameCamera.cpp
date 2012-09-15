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
  turnLeftKey  = false;
  turnRightKey = false;

  velocity = Ogre::Vector3( 0,0,0 );
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
  velocity.x *= 0.9;
  velocity.z *= 0.9;
  velocity.y *= 0.9;

  if( upKey )
    velocity.z -= 5.0;
  if( downKey )
    velocity.z += 5.0;
  if( rightKey )
    velocity.x += 5.0;
  if( leftKey )
    velocity.x -= 5.0;

  turnVelocity *= 0.8;

  if( turnRightKey )
    turnVelocity += 1.0;
  if( turnLeftKey )
    turnVelocity -= 1.0;

  Ogre::Degree pitch    = Ogre::Degree( -velocity.y/20 );
  Ogre::Degree curPitch = Ogre::Degree( cam->getOrientation().getPitch() );
  if( pitch + curPitch >= Ogre::Degree( -70.0 ) &&
      pitch + curPitch <= Ogre::Degree( -10.0 ) &&
      camNode->getPosition().y >= 100 &&
      camNode->getPosition().y <= 1000 )
  {
    cam->pitch( Ogre::Radian( pitch ) );
  }

  if( camNode->getPosition().y + velocity.y >= 0 &&
      camNode->getPosition().y + velocity.y <= 2000 )
    camNode->translate( 0, velocity.y, 0, Ogre::Node::TS_LOCAL );

  if( turnVelocity >= 1.0 || turnVelocity <= -1.0 )
    camNode->yaw( Ogre::Radian( Ogre::Degree( turnVelocity ) ) );

  if( velocity.length() >= 1.0 )
  {
    camNode->translate( velocity.x, 0, velocity.z, Ogre::Node::TS_LOCAL );
  }
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

    case( OIS::KC_Q ):
      turnRightKey = true;
      break;

    case( OIS::KC_E ):
      turnLeftKey = true;
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

    case( OIS::KC_Q ):
      turnRightKey = false;
      break;

    case( OIS::KC_E ):
      turnLeftKey = false;
      break;
  }

  return true;
}



bool GameCamera::mouseMoved( const OIS::MouseEvent &arg )
{
  if( arg.state.Z.rel )
    velocity.y -= 5*arg.state.Z.rel/120;

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

