#include "GameRoadGraph.hpp"

using namespace viqo::gameworld;


long int roadGraphCounter=0;



GameRoadGraphArea *GameRoadGraphArea::Divide( std::vector<GameRoadGraph*> *graphs )
{
  a->BreakConnection( b );
  c->BreakConnection( d );

  GameRoadGraph *e = new GameRoadGraph( GenerateRoadGraphId() );
  GameRoadGraph *f = new GameRoadGraph( GenerateRoadGraphId() );

  e->location.x = (a->location.x + b->location.x) / 2.0;
  e->location.z = (a->location.z + b->location.z) / 2.0;

  f->location.x = (c->location.x + d->location.x) / 2.0;
  f->location.z = (c->location.z + d->location.z) / 2.0;

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

  GameRoadGraphArea *ret = new GameRoadGraphArea[2];
  ret[0].Init( a, e, c, f );
  ret[1].Init( e, b, f, d );
  return ret;
}

