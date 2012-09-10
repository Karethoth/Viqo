#ifndef __VIQO_SCENES_GAMESCENE_HPP__
#define __VIQO_SCENES_GAMESCENE_HPP__

#include "../Scene.hpp"
#include "../managers/GameObjectManager.hpp"
#include "../gameworld/GameObject.hpp"


namespace viqo
{
  namespace scenes
  {
    class GameScene : public viqo::Scene
    {
     protected:
      virtual void CreateScene();
      //virtual void DestroyScene();
      //virtual void CreateFrameListener();
      virtual bool frameRenderingQueued( const Ogre::FrameEvent &evt );
      //virtual void CreateCamera();
      //virtual void CreateViewports();

      //virtual bool keyPressed( const OIS::KeyEvent &arg );
      //virtual bool keyReleased( const OIS::KeyEvent &arg );

      //virtual bool mouseMoved( const OIS::MouseEvent &arg );
      //virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
      //virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

      viqo::managers::GameObjectManager goManager;

     public:
      GameScene();
      virtual ~GameScene();

      //virtual void Run();
    };
  } // namespace viqo::scenes
} // namespace viqo

#endif

