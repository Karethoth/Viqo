#ifndef __VIQO_GAMEWORLD_GAMEROADSYSTEM_HPP__
#define __VIQO_GAMEWORLD_GAMEROADSYSTEM_HPP__

#include <OgreString.h>
#include <OgreSceneManager.h>
#include "GameRoadGraph.hpp"


namespace viqo
{
  namespace gameworld
  {
    class GameRoadSystem
    {
     protected:
      std::vector<GameRoadGraph*> roadGraphs;
      GameRoadGraphAreaTreeNode  *roadRoot;

      bool GenerateChildren( GameRoadGraphAreaTreeNode *node );
      bool BuildArea( GameRoadGraphAreaTreeNode*, Ogre::ManualObject* );
      bool BuildRoad( GameRoadGraph*, GameRoadGraph*, Ogre::ManualObject* );


     public:
      GameRoadSystem();
      ~GameRoadSystem();

      bool Generate();
      void Print();

      Ogre::String BuildRoads( Ogre::SceneManager* );

      std::vector<GameRoadGraph*> *GetGraphs();
      GameRoadGraphAreaTreeNode *GetRoadRoot();
    };
  }
}

#endif

