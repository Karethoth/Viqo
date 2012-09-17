#include "GameTerrain.hpp"

using namespace viqo::gameworld;


GameTerrain::GameTerrain( Ogre::SceneManager *sceneManager )
{
  sceneMan = sceneManager;
  terrainImported = false;

  Ogre::Vector3 lightDir( 0.55, -0.3, 0.75 );

  Ogre::Light* light = sceneMan->createLight("tstLight");
  light->setType( Ogre::Light::LT_DIRECTIONAL );
  light->setDirection( lightDir );
  light->setDiffuseColour( Ogre::ColourValue::White );
  light->setSpecularColour( Ogre::ColourValue(0.4, 0.4, 0.4) );

  sceneMan->setAmbientLight( Ogre::ColourValue(0.2, 0.2, 0.2) );

  terrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
  terrainGroup   = OGRE_NEW Ogre::TerrainGroup( sceneMan, Ogre::Terrain::ALIGN_X_Z, 513, 12000.0f );
  terrainGroup->setFilenameConvention( Ogre::String("ViqoTerrain"), Ogre::String("dat") );
  terrainGroup->setOrigin( Ogre::Vector3::ZERO );

  ConfigureTerrainDefaults( light );

  for( long x = 0; x <= 0; ++x )
    for( long y = 0; y <= 0; ++y )
      DefineTerrain( x, y );

  // sync load since we want everything in place when we start
  terrainGroup->loadAllTerrains( true );

  if( terrainImported )
  {
    Ogre::TerrainGroup::TerrainIterator ti = terrainGroup->getTerrainIterator();
    while( ti.hasMoreElements() )
    {
      Ogre::Terrain *t = ti.getNext()->instance;
      InitBlendMaps(t);
    }
  }

  terrainGroup->freeTemporaryResources();
}



GameTerrain::~GameTerrain()
{
}



void GameTerrain::DefineTerrain( long x, long y )
{
  Ogre::String filename = terrainGroup->generateFilename( x, y );
  if( Ogre::ResourceGroupManager::getSingleton().resourceExists( terrainGroup->getResourceGroup(), filename ) )
  {
    terrainGroup->defineTerrain( x, y );
  }
  else
  {
    Ogre::Image img;
    GetTerrainImage( x % 2 != 0, y % 2 != 0, img );
    terrainGroup->defineTerrain( x, y, &img );
    terrainImported = true;
  }
}



void GameTerrain::InitBlendMaps( Ogre::Terrain *terrain )
{
  Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap( 1 );
  Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap( 2 );
  Ogre::Real minHeight0 = 70;
  Ogre::Real fadeDist0  = 40;
  Ogre::Real minHeight1 = 70;
  Ogre::Real fadeDist1  = 15;

  float* pBlend1 = blendMap1->getBlendPointer();

  for( Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y )
  {
    for( Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x )
    {
      Ogre::Real tx, ty;

      blendMap0->convertImageToTerrainSpace( x, y, &tx, &ty );
      Ogre::Real height = terrain->getHeightAtTerrainPosition( tx, ty );
      Ogre::Real val = (height - minHeight0) / fadeDist0;
      val = Ogre::Math::Clamp( val, (Ogre::Real)0, (Ogre::Real)1 );

      val = (height - minHeight1) / fadeDist1;
      val = Ogre::Math::Clamp( val, (Ogre::Real)0, (Ogre::Real)1 );
      *pBlend1++ = val;
    }
  }
  blendMap0->dirty();
  blendMap1->dirty();
  blendMap0->update();
  blendMap1->update();
}



void GameTerrain::ConfigureTerrainDefaults( Ogre::Light *light )
{
  // Configure global
  terrainGlobals->setMaxPixelError( 8);

  // testing composite map
  terrainGlobals->setCompositeMapDistance( 3000 );

  // Important to set these so that the terrain knows what to use for derived (non-realtime) data
  terrainGlobals->setLightMapDirection( light->getDerivedDirection() );
  terrainGlobals->setCompositeMapAmbient( sceneMan->getAmbientLight() );
  terrainGlobals->setCompositeMapDiffuse( light->getDiffuseColour() );

  // Configure default import settings for if we use imported image
  Ogre::Terrain::ImportData& defaultimp = terrainGroup->getDefaultImportSettings();
  defaultimp.terrainSize = 513;
  defaultimp.worldSize = 12000.0f;
  defaultimp.inputScale = 600;
  defaultimp.minBatchSize = 33;
  defaultimp.maxBatchSize = 65;

  // textures
  defaultimp.layerList.resize( 3 );
  defaultimp.layerList[0].worldSize = 100;
  defaultimp.layerList[0].textureNames.push_back( "dirt_grayrocky_diffusespecular.dds" );
  defaultimp.layerList[0].textureNames.push_back( "dirt_grayrocky_normalheight.dds" );
  defaultimp.layerList[1].worldSize = 30;
  defaultimp.layerList[1].textureNames.push_back( "grass_green-01_diffusespecular.dds" );
  defaultimp.layerList[1].textureNames.push_back( "grass_green-01_normalheight.dds" );
  defaultimp.layerList[2].worldSize = 200;
  defaultimp.layerList[2].textureNames.push_back( "growth_weirdfungus-03_diffusespecular.dds" );
  defaultimp.layerList[2].textureNames.push_back( "growth_weirdfungus-03_normalheight.dds" );
}



void GameTerrain::GetTerrainImage( bool flipX, bool flipY, Ogre::Image &img )
{
    img.load( "terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );

    if (flipX)
        img.flipAroundY();
    if (flipY)
        img.flipAroundX();
}

