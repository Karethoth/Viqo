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
        location = Ogre::Vector3( 0.0, 0.0, 0.0 );
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



    struct GameRoadGraphArea
    {
      GameRoadGraph *a;
      GameRoadGraph *b;
      GameRoadGraph *c;
      GameRoadGraph *d;


      // Initializers
      GameRoadGraphArea();
      GameRoadGraphArea( GameRoadGraph *A,
                         GameRoadGraph *B,
                         GameRoadGraph *C,
                         GameRoadGraph *D );

      void Init( GameRoadGraph *A,
                 GameRoadGraph *B,
                 GameRoadGraph *C,
                 GameRoadGraph *D );

      // Swaps places of B and C.
      // Used when wanted to divide area by other axis.
      void Rotate();

      // Divides area, returns a pointer to GameRoadGraphArea[2].
      GameRoadGraphArea *Divide( std::vector<GameRoadGraph*> *graphs );
    };



    static long int GenerateRoadGraphId()
    {
      return roadGraphCounter++;
    }



    struct GameRoadGraphAreaTreeNode
    {
      GameRoadGraphArea         *area;
      GameRoadGraphAreaTreeNode *children[2];

      GameRoadGraphAreaTreeNode( GameRoadGraphArea* );

      void SetChildren( GameRoadGraphAreaTreeNode*,
                        GameRoadGraphAreaTreeNode* );
    };
  }
}

#endif

