#include "GameObject.hpp"

using namespace viqo::gameworld;
using std::string;


GameObject::GameObject( Ogre::SceneManager *sceneManager )
                      : sceneMan( sceneManager ),
                        entity( NULL ),
                        animationState( NULL )
{
}



GameObject::~GameObject()
{
  if( animationState )
    delete animationState;

  if( entity )
    delete entity;
}



bool GameObject::Load( string objectName, string path )
{
  name = objectName;
  entity = sceneMan->createEntity( objectName, path );
  entity->setCastShadows( true );
  sceneMan->getRootSceneNode()->createChildSceneNode()->attachObject( entity );

  animationState = entity->getAnimationState( "Walk" );
  animationState->setLoop( true );
  animationState->setEnabled( true );

  return true;
}



bool  GameObject::Update( Ogre::Real timeSinceLastFrame )
{
  if( animationState )
    animationState->addTime( timeSinceLastFrame );

  return true;
}
