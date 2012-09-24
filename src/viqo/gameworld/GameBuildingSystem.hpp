#ifndef __VIQO_GAMEWORLD_GAMEBUILDINGSYSTEM_HPP__
#define __VIQO_GAMEWORLD_GAMEBUILDINGSYSTEM_HPP__

#include "GameRoadSystem.hpp"


namespace viqo
{
  namespace gameworld
  {
    class GameBuildingSystem
    {
     private:
     public:
      Ogre::String BuildBuildings( Ogre::SceneManager *sceneMan, GameRoadGraphAreaTreeNode *roadRoot );
      bool BuildToArea( GameRoadGraphAreaTreeNode *rootArea, Ogre::ManualObject *manObject );
      bool GenerateBuilding( GameRoadGraphAreaTreeNode *area, Ogre::ManualObject *ManualObject );
    };
  }
}

#endif

