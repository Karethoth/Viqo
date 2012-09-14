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



void GameCamera::Update()
{
  if( upKey )
    velocity.z -= 1.0;
  if( downKey )
    velocity.z += 1.0;
  if( rightKey )
    velocity.x += 1.0;
  if( leftKey )
    velocity.x -= 1.0;

  velocity.x *= 0.9;
  velocity.y *= 0.9;
  velocity.z *= 0.9;

  camNode->translate( velocity );
}



bool GameCamera::keyPressed( const OIS::KeyEvent &arg )
{
  switch( arg.key )
  {
    case( OIS::KC_UP ):
      upKey = true;
      break;

    case( OIS::KC_DOWN ):
      downKey = true;
      break;

    case( OIS::KC_RIGHT ):
      rightKey = true;
      break;

    case( OIS::KC_LEFT ):
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
      upKey = false;
      break;

    case( OIS::KC_DOWN ):
      downKey = false;
      break;

    case( OIS::KC_RIGHT ):
      rightKey = false;
      break;

    case( OIS::KC_LEFT ):
      leftKey = false;
      break;
  }
  return true;
}



bool GameCamera::mouseMoved( const OIS::MouseEvent &arg )
{
  if( arg.state.Z.rel )
    printf( "rel: %i\n", arg.state.Z.rel );
  return true;
}



bool GameCamera::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
  camNode->translate( Ogre::Vector3( 10, 10, 10 ) );
  return true;
}



bool GameCamera::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
  return true;
}

