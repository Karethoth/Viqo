#include "GameRoadGraph.hpp"

using namespace viqo::gameworld;


long int roadGraphCounter=0;



void viqo::gameworld::Divide( GameRoadGraph *a,
                              GameRoadGraph *b,
                              GameRoadGraph *c,
                              GameRoadGraph *d,
                              std::vector<GameRoadGraph*> *graphs )
{
  a->BreakConnection( b );
  c->BreakConnection( d );

  GameRoadGraph *e = new GameRoadGraph( GenerateRoadGraphId() );
  GameRoadGraph *f = new GameRoadGraph( GenerateRoadGraphId() );

  graphs->push_back( e );
  graphs->push_back( f );

  // A <-> E
  a->AddChild( e );
  e->AddParent( a );

  // C <-> F
  c->AddChild( f );
  f->AddParent( c );
  
  // E <-> F
  e->AddChild( f );
  f->AddParent( e );

  // E <-> B
  e->AddChild( b );
  b->AddParent( e );

  // F <-> D
  f->AddChild( d );
  d->AddParent( f );
}

