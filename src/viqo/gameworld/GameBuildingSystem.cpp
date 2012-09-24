#include "GameBuildingSystem.hpp"
#include <OgreManualObject.h>

using namespace viqo::gameworld;


Ogre::String GameBuildingSystem::BuildBuildings( Ogre::SceneManager *sceneMan, GameRoadGraphAreaTreeNode *roadRoot )
{
  Ogre::ManualObject *manObject = NULL;
  Ogre::String manObjectName    = "Buildings";
  manObject = sceneMan->createManualObject( manObjectName );
  manObject->setDynamic( false );
  
  BuildToArea( roadRoot, manObject );

  manObject->setCastShadows( true );

  sceneMan->getRootSceneNode()->createChildSceneNode()->attachObject( manObject );
  return manObjectName;
}



bool GameBuildingSystem::BuildToArea( GameRoadGraphAreaTreeNode *area, Ogre::ManualObject *manObject )
{
  if( area->children[0] != NULL )
    BuildToArea( area->children[0], manObject );

  if( area->children[1] != NULL )
    BuildToArea( area->children[1], manObject );

  if( area->children[0] != NULL &&
      area->children[1] != NULL )
  {
    return true;
  }

  GenerateBuilding( area, manObject );
}



bool GameBuildingSystem::GenerateBuilding( GameRoadGraphAreaTreeNode *area, Ogre::ManualObject *manObject )
{
  GameRoadGraphArea *a = area->area;

  float areaA = (a->d->location.x - a->a->location.x) *
                (a->d->location.z - a->a->location.z);

  Ogre::Vector3 xMod( 50, 0, 0 );
  Ogre::Vector3 yMod( 0, areaA*(1+rand()%4), 0 );
  Ogre::Vector3 zMod( 0, 0, 50 );

  if( a->b->location.x <= a->c->location.x )
  {
    GameRoadGraph *tmp = a->b;
    a->b = a->c;
    a->c = tmp;
  }

  Ogre::Vector3 cornerA( a->a->location*100 + xMod + zMod );
  Ogre::Vector3 cornerB( a->b->location*100 - xMod + zMod );
  Ogre::Vector3 cornerC( a->c->location*100 + xMod - zMod );
  Ogre::Vector3 cornerD( a->d->location*100 - xMod - zMod );

  Ogre::Vector3 cornerE( cornerA + yMod );
  Ogre::Vector3 cornerF( cornerB + yMod );
  Ogre::Vector3 cornerG( cornerC + yMod );
  Ogre::Vector3 cornerH( cornerD + yMod );

  Ogre::Vector3 frontNormal( 0, 0, 1 );
  Ogre::Vector3 backNormal( 0, 0, -1 );
  Ogre::Vector3 rightNormal( 1, 0, 1 );
  Ogre::Vector3 leftNormal( -1, 0, 0 );
  Ogre::Vector3 topNormal( 0, 1, 0 );

  manObject->begin( "BaseBuilding", Ogre::RenderOperation::OT_TRIANGLE_LIST );

  // Front
  manObject->position( cornerD );
  manObject->normal( frontNormal );
  manObject->position( cornerH );
  manObject->normal( frontNormal );
  manObject->position( cornerG );
  manObject->normal( frontNormal );

  manObject->position( cornerG );
  manObject->normal( frontNormal );
  manObject->position( cornerC );
  manObject->normal( frontNormal );
  manObject->position( cornerD );
  manObject->normal( frontNormal );

  // Back
  manObject->position( cornerE );
  manObject->normal( backNormal );
  manObject->position( cornerB );
  manObject->normal( backNormal );
  manObject->position( cornerA );
  manObject->normal( backNormal );

  manObject->position( cornerB );
  manObject->normal( backNormal );
  manObject->position( cornerE );
  manObject->normal( backNormal );
  manObject->position( cornerF );
  manObject->normal( backNormal );

  // Right
  manObject->position( cornerD );
  manObject->normal( rightNormal );
  manObject->position( cornerB );
  manObject->normal( rightNormal );
  manObject->position( cornerH );
  manObject->normal( rightNormal );

  manObject->position( cornerB );
  manObject->normal( rightNormal );
  manObject->position( cornerF );
  manObject->normal( rightNormal );
  manObject->position( cornerH );
  manObject->normal( rightNormal );

  // Left
  manObject->position( cornerE );
  manObject->normal( leftNormal );
  manObject->position( cornerA );
  manObject->normal( leftNormal );
  manObject->position( cornerC );
  manObject->normal( leftNormal );

  manObject->position( cornerE );
  manObject->normal( leftNormal );
  manObject->position( cornerC );
  manObject->normal( leftNormal );
  manObject->position( cornerG );
  manObject->normal( leftNormal );

  // Top
  manObject->position( cornerG );
  manObject->normal( topNormal );
  manObject->position( cornerF );
  manObject->normal( topNormal );
  manObject->position( cornerE );
  manObject->normal( topNormal );

  manObject->position( cornerF );
  manObject->normal( topNormal );
  manObject->position( cornerG );
  manObject->normal( topNormal );
  manObject->position( cornerH );
  manObject->normal( topNormal );


  manObject->end();
  return true;
}

