#ifndef __VIQO_GAMEWORLD_GAMEOBJECT_HPP__
#define __VIQO_GAMEWORLD_GAMEOBJECT_HPP__

#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OpenSteer/Vec3.h>

using OpenSteer::Vec3;


namespace viqo
{
  namespace gameworld
  {
    class GameObject
    {
     public:
      Vec3 location;
      Vec3 rotation;
      Vec3 velocity;
      Vec3 scale;

      std::string name;

      Ogre::SceneManager *sceneMan;
      Ogre::Entity *entity;
      Ogre::AnimationState *animationState;


     public:
      GameObject( Ogre::SceneManager* );
      virtual ~GameObject();

      virtual bool Load( std::string objectName, std::string path );
      virtual bool Update( Ogre::Real );
    };
  }
}

#endif

