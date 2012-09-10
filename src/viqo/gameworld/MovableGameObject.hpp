#ifndef __VIQO_GAMEWORLD_MOVABLEGAMEOBJECT_HPP__
#define __VIQO_GAMEWORLD_MOVABLEGAMEOBJECT_HPP__

#include "GameObject.hpp"
#include <deque>


class MovableGameObject : public viqo::gameworld::GameObject
{
 public:
  Ogre::Vector3 destination;
  Ogre::Real distance;

  std::deque<Ogre::Vector3> walkList;

  bool moving;

 public:
  MovableGameObject();
  virtual ~MovableGameObject();

  virtual bool Update( Ogre::Real );
  virtual bool RotateToDirection();
};

#endif

