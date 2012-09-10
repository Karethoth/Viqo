#include "GameObject.hpp"

using namespace viqo::gameworld;
using std::string;


GameObject::GameObject()
{
}



GameObject::~GameObject()
{
  if( animationState )
    delete animationState;

  if( entity )
    delete entity;
}



void GameObject::Init( Ogre::SceneManager *sceneManager,
                       Ogre::SceneNode *sceneNode )
{
  sceneMan = sceneManager;
  node     = sceneNode;
  entity   = NULL;
  animationState = NULL;
}



bool GameObject::Load( string objectName, string path )
{
  name = objectName;
  entity = sceneMan->createEntity( objectName, path );
  entity->setCastShadows( true );
  node->attachObject( entity );

  animationState = entity->getAnimationState( "Idle2" );
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

