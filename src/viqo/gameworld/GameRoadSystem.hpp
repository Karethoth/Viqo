#ifndef __VIQO_GAMEWORLD_GAMEROADSYSTEM_HPP__
#define __VIQO_GAMEWORLD_GAMEROADSYSTEM_HPP__

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


     public:
      GameRoadSystem();
      ~GameRoadSystem();

      bool Generate();
      void Print();

      std::vector<GameRoadGraph*> *GetGraphs();
    };
  }
}

#endif

