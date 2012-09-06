#ifndef __TESTSCENE_HPP__
#define __TESTSCENE_HPP__

#include "Scene.hpp"

namespace viqo
{
  namespace scenes
  {
    class TestScene : public viqo::Scene
    {
     protected:
      virtual void CreateScene();
      virtual void DestroyScene();
      virtual void CreateFrameListener();
      virtual bool frameRenderingQueued( const Ogre::FrameEvent &evt );
      //virtual void CreateCamera();
      //virtual void CreateViewports();

      virtual bool keyPressed( const OIS::KeyEvent &arg );
      virtual bool keyReleased( const OIS::KeyEvent &arg );

      virtual bool mouseMoved( const OIS::MouseEvent &arg );
      virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
      virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );


     public:
      TestScene();
      virtual ~TestScene();

      //virtual void Run();
    };
  } // namespace viqo::scene
} // namespace viqo

#endif

