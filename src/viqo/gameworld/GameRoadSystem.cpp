#include "GameRoadSystem.hpp"
#include <OgreManualObject.h>

using namespace viqo::gameworld;


GameRoadSystem::GameRoadSystem()
{
  roadRoot = NULL;
}



GameRoadSystem::~GameRoadSystem()
{
}


bool GameRoadSystem::Generate()
{
  GameRoadGraph *a = new GameRoadGraph( GenerateRoadGraphId() );
  GameRoadGraph *b = new GameRoadGraph( GenerateRoadGraphId() );
  GameRoadGraph *c = new GameRoadGraph( GenerateRoadGraphId() );
  GameRoadGraph *d = new GameRoadGraph( GenerateRoadGraphId() );

  a->location = Ogre::Vector3( 0, 0, 0 );
  b->location = Ogre::Vector3( 100, 0, 0 );
  c->location = Ogre::Vector3( 0, 0, 100 );
  d->location = Ogre::Vector3( 100, 0, 100 );
  
  roadGraphs.push_back( a );
  roadGraphs.push_back( b );
  roadGraphs.push_back( c );
  roadGraphs.push_back( d );


  a->AddChild( b );
  a->AddChild( c );
  b->AddChild( d );
  c->AddChild( d );

  d->AddParent( b );
  d->AddParent( c );
  c->AddParent( a );
  b->AddParent( a );

  roadRoot = new GameRoadGraphAreaTreeNode( NULL );
  roadRoot->area = new GameRoadGraphArea( a, b, c, d );

  GenerateChildren( roadRoot );

  return true;
}



bool GameRoadSystem::GenerateChildren( GameRoadGraphAreaTreeNode *node )
{
  float area = (node->area->d->location.x - node->area->a->location.x) *
               (node->area->d->location.z - node->area->a->location.z);
  if( area <= 100 &&
      (area <= 20 ||
       rand()%100 >= 90) )
  {
    return false;
  }

  GameRoadGraphAreaTreeNode *childA;
  GameRoadGraphAreaTreeNode *childB;

  GameRoadGraphArea *childrenAreas;

  if( rand()%3 )
    node->area->Rotate();

  childrenAreas = node->area->Divide( &roadGraphs );

  childA = new GameRoadGraphAreaTreeNode( &childrenAreas[0] );
  childB = new GameRoadGraphAreaTreeNode( &childrenAreas[1] );

  node->SetChildren( childA, childB );

  GenerateChildren( childA );
  GenerateChildren( childB );

  return true;
}



void GameRoadSystem::Print()
{
  std::vector<GameRoadGraph*>::iterator it;
  std::vector<GameRoadGraph*>::iterator sit;
  for( it = roadGraphs.begin(); it != roadGraphs.end(); ++it )
  {
    printf( "\n----------------\n" );
    printf( "| ROADGRAPH %d X:%f Z:%f\n", (*it)->id, (*it)->location.x, (*it)->location.z );
    printf( "----------------\n" );

    printf( "\tPARENTS:\n" );
    for( sit = (*it)->parents.begin(); sit != (*it)->parents.end(); ++sit )
    {
      printf( "\t\t%d\n", (*sit)->id );
    }

    printf( "\tCHILDREN:\n" );
    for( sit = (*it)->children.begin(); sit != (*it)->children.end(); ++sit )
    {
      printf( "\t\t%d\n", (*sit)->id );
    }
  }
}



std::vector<GameRoadGraph*> *GameRoadSystem::GetGraphs()
{
  return &roadGraphs;
}



GameRoadGraphAreaTreeNode *GameRoadSystem::GetRoadRoot()
{
  return roadRoot;
}



Ogre::String GameRoadSystem::BuildRoads( Ogre::SceneManager *sceneMan )
{
  Ogre::ManualObject *manObject = NULL;
  Ogre::String manObjectName    = "Roads";
  manObject = sceneMan->createManualObject( manObjectName );
  manObject->setDynamic( false );
  
  manObject->begin( "BaseRoads", Ogre::RenderOperation::OT_LINE_LIST );
  BuildArea( roadRoot, manObject );
  manObject->end();

  sceneMan->getRootSceneNode()->createChildSceneNode()->attachObject( manObject );
  return manObjectName;
}



bool GameRoadSystem::BuildArea( GameRoadGraphAreaTreeNode *area, Ogre::ManualObject *manObject )
{

  if( area->children[0] != NULL )
    BuildArea( area->children[0], manObject );

  if( area->children[1] != NULL )
    BuildArea( area->children[1], manObject );

  if( area->children[0] != NULL &&
      area->children[1] != NULL )
  {
    return true;
  }


  GameRoadGraphArea *a = area->area;

  manObject->position( a->a->location*100 );
  manObject->position( a->b->location*100 );

  manObject->position( a->a->location*100 );
  manObject->position( a->c->location*100 );

  manObject->position( a->b->location*100 );
  manObject->position( a->d->location*100 );

  manObject->position( a->c->location*100 );
  manObject->position( a->d->location*100 );
  return true;
}

