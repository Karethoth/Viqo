#include "GameRoadSystem.hpp"
#include <OgreManualObject.h>
#include <OgreMath.h>

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

  a->location = Ogre::Vector3( -50, 0, -50 );
  b->location = Ogre::Vector3( 50, 0, -50 );
  c->location = Ogre::Vector3( -50, 0, 50 );
  d->location = Ogre::Vector3( 50, 0, 50 );
  
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
  
  BuildArea( roadRoot, manObject );

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

  BuildRoad( a->a, a->b, manObject );
  BuildRoad( a->a, a->c, manObject );

  return true;
}



bool GameRoadSystem::BuildRoad( GameRoadGraph *a, GameRoadGraph *b, Ogre::ManualObject *manObject )
{
  Ogre::Vector3 direction = b->location - a->location;
  direction.normalise();

  Ogre::Vector3 xMod = Ogre::Vector3( 50, 50, 50 ) * direction;
  Ogre::Vector3 zMod;

  Ogre::Real theta = Ogre::Math::DegreesToRadians( 90 );
  Ogre::Real cs = Ogre::Math::Cos( theta );
  Ogre::Real sn = Ogre::Math::Sin( theta );

  zMod.y = 0;
  zMod.x = xMod.x * cs - xMod.z * sn;
  zMod.z = xMod.x * sn + xMod.z * cs;

  Ogre::Vector3 upNormal( 0, 1, 0 );


  Ogre::Vector3 cornerA( a->location*100+xMod - zMod );
  Ogre::Vector3 cornerB( a->location*100+xMod + zMod );
  Ogre::Vector3 cornerC( b->location*100-xMod - zMod );
  Ogre::Vector3 cornerD( b->location*100-xMod + zMod );

  manObject->begin( "Examples/WaterStream", Ogre::RenderOperation::OT_TRIANGLE_LIST );

  manObject->position( cornerA );
  manObject->normal( upNormal );
  manObject->textureCoord( 0, 0 );
  manObject->position( cornerB );
  manObject->normal( upNormal );
  manObject->textureCoord( 1, 0 );
  manObject->position( cornerC );
  manObject->normal( upNormal );
  manObject->textureCoord( 0, 1 );

  manObject->position( cornerD );
  manObject->normal( upNormal );
  manObject->textureCoord( 1, 1 );
  manObject->position( cornerC );
  manObject->normal( upNormal );
  manObject->textureCoord( 0, 1 );
  manObject->position( cornerB );
  manObject->normal( upNormal );
  manObject->textureCoord( 1, 0 );

  manObject->end();
}

