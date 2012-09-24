#ifndef __VIQO_GAMEWORLD_GAMETERRAIN_HPP__
#define __VIQO_GAMEWORLD_GAMETERRAIN_HPP__

#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
#include <OgreSceneManager.h>
#include <OgreLight.h>

#include "GameRoadSystem.hpp"
#include "GameBuildingSystem.hpp"


namespace viqo
{
  namespace gameworld
  {
    class GameTerrain
    {
     protected:
      Ogre::TerrainGlobalOptions *terrainGlobals;
      Ogre::TerrainGroup *terrainGroup;
      bool terrainImported;
      Ogre::SceneManager *sceneMan;

      GameRoadSystem     roadSys;
      GameBuildingSystem buildingSys;


     public:
      GameTerrain( Ogre::SceneManager* );
      virtual ~GameTerrain();

      void DefineTerrain( long x, long y );
      void InitBlendMaps( Ogre::Terrain *terrain );
      void ConfigureTerrainDefaults( Ogre::Light *light );
      void GetTerrainImage( bool flipX, bool flipY, Ogre::Image &img );
    };
  }
}

#endif

