#ifndef __VIQO_GAMEWORLD_GAMEOBJECT_HPP__
#define __VIQO_GAMEWORLD_GAMEOBJECT_HPP__

#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OpenSteer/Vec3.h>

using Ogre::Vector3;


namespace viqo
{
  namespace gameworld
  {
    class GameObject
    {
     public:
      Vector3 location;
      Vector3 rotation;
      Vector3 velocity;
      Vector3 scale;

      std::string name;

      Ogre::SceneManager *sceneMan;
      Ogre::SceneNode *node;
      Ogre::Entity *entity;
      Ogre::AnimationState *animationState;


     public:
      GameObject();
      virtual ~GameObject();

      virtual void Init( Ogre::SceneManager*, Ogre::SceneNode* );

      virtual bool Load( std::string objectName, std::string path );
      virtual bool Update( Ogre::Real );
    };
  }
}

#endif

