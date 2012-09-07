#ifndef __VIQO_SCENES_INTROSCENE_HPP_
#define __VIQO_SCENES_INTROSCENE_HPP_

#include "../Scene.hpp"
#include <OGRE/OgreTimer.h>


namespace viqo
{
  namespace scenes
  {
    class IntroScene : public viqo::Scene
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

      Ogre::Timer  *timer;
      unsigned long sceneLengthMs;


     public:
      IntroScene();
      virtual ~IntroScene();

      //virtual void Run();
    };
  } // namespace viqo::scenes
} // namespace viqo

#endif

