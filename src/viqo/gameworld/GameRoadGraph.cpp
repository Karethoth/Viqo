#include "GameRoadGraph.hpp"

using namespace viqo::gameworld;


long int roadGraphCounter=0;



GameRoadGraphArea::GameRoadGraphArea()
{
  a = NULL;
  b = NULL;
  c = NULL;
  d = NULL;
}



GameRoadGraphArea::GameRoadGraphArea( GameRoadGraph *A,
                                      GameRoadGraph *B,
                                      GameRoadGraph *C,
                                      GameRoadGraph *D )
{
  Init( A, B, C, D );
}



void GameRoadGraphArea::Init( GameRoadGraph *A,
                              GameRoadGraph *B,
                              GameRoadGraph *C,
                              GameRoadGraph *D )
{
  a = A;
  b = B;
  c = C;
  d = D;
}



void GameRoadGraphArea::Rotate()
{
  GameRoadGraph *tmp;
  tmp = c;
  c   = b;
  b   = tmp;
}



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



GameRoadGraphAreaTreeNode::GameRoadGraphAreaTreeNode( GameRoadGraphArea *a )
{
  area = a;
  children[0] = NULL;
  children[1] = NULL;
}


void GameRoadGraphAreaTreeNode::SetChildren( GameRoadGraphAreaTreeNode *a,
                                        GameRoadGraphAreaTreeNode *b )
{
  children[0] = a;
  children[1] = b;
}

