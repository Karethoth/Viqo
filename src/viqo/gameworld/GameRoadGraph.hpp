#ifndef __VIQO_GAMEWORLD_GAMEROADGRAPH_HPP__
#define __VIQO_GAMEWORLD_GAMEROADGRAPH_HPP__

#include <OgreRoot.h>
#include <vector>

extern long int roadGraphCounter;

namespace viqo
{
  namespace gameworld
  {
    struct GameRoadGraph
    {
      long int                    id;

      Ogre::Vector3               location;
      std::vector<GameRoadGraph*> parents;
      std::vector<GameRoadGraph*> children;
      bool drawn;


      GameRoadGraph( long int graphId )
      {
        id = graphId;
      }



      void AddChild( GameRoadGraph *graph )
      {
        children.push_back( graph );
      }



      void AddParent( GameRoadGraph *graph )
      {
        parents.push_back( graph );
      }



      void BreakConnection( GameRoadGraph *graph )
      {
        std::vector<GameRoadGraph*>::iterator it;

        for( it = parents.begin(); it != parents.end(); )
        {
          if( (*it)->id == graph->id )
          {
            GameRoadGraph *connection = (*it);
            it = parents.erase( it );
            connection->BreakConnection( this );
            break;
          }
          else
          {
            ++it;
          }
        }

        for( it = children.begin(); it != children.end(); )
        {
          if( (*it)->id == graph->id )
          {
            GameRoadGraph *connection = (*it);
            it = children.erase( it );
            connection->BreakConnection( this );
            break;
          }
          else
          {
            ++it;
          }
        }
      }
    };



    static long int GenerateRoadGraphId()
    {
      return roadGraphCounter++;
    }



    void Divide( GameRoadGraph *a,
                 GameRoadGraph *b,
                 GameRoadGraph *c,
                 GameRoadGraph *d,
                 std::vector<GameRoadGraph*> *graphs );
  }
}

#endif

