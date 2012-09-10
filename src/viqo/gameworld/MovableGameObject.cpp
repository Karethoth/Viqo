#include "MovableGameObject.hpp"

using namespace viqo::gameworld;

MovableGameObject::MovableGameObject()
{
  walkList.push_back( Ogre::Vector3( 550, 0, 550 ) );
  rotation = Ogre::Vector3::ZERO;
  moving = false;
}



MovableGameObject::~MovableGameObject()
{
  walkList.clear();
}



bool MovableGameObject::Update( Ogre::Real timeSinceLastFrame )
{
  timeSinceLastFrame *= 2;
  animationState->addTime( timeSinceLastFrame );
  if( !moving )
  {
    if( !walkList.empty() )
    {
      animationState = entity->getAnimationState( "Walk" );
      animationState->setLoop( true );
      animationState->setEnabled( true );

      destination = walkList.front();
      moving = true;
      rotation = destination - node->getPosition();
      distance = rotation.normalise();
      Ogre::Vector3 src = node->getOrientation() * Ogre::Vector3::UNIT_Z * -1.0;
      Ogre::Quaternion quat = src.getRotationTo( rotation );
      node->rotate( quat );
    }
  }
  else
  {
    Ogre::Real move = timeSinceLastFrame * 35.0;
    distance -= move;
    if( distance <= 0.0 )
    {
      node->setPosition( destination );
      moving = false;
      animationState = entity->getAnimationState( "Idle2" );
      animationState->setLoop( true );
      animationState->setEnabled( true );
      walkList.pop_front();
      return true;
    }
    
    node->translate( rotation * move );
  }
  return true;
}



bool MovableGameObject::RotateToDirection()
{
}

