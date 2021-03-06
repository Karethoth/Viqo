#ifndef __VIQO_SCENE_HPP__
#define __VIQO_SCENE_HPP__

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreMaterial.h>
#include <OgreConfigFile.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <SdkTrays.h>
#include <SdkCameraMan.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>


namespace viqo
{
  class Scene : public Ogre::FrameListener,
                    public Ogre::WindowEventListener,
                    public OIS::KeyListener,
                    public OIS::MouseListener,
                    OgreBites::SdkTrayListener
  {
   protected:
    virtual bool Setup();
    virtual void ChooseSceneManager();
    virtual void CreateCamera();
    virtual void CreateFrameListener();
    virtual void CreateScene() = 0;
    virtual void DestroyScene();
    virtual void CreateViewports();
    virtual void DestroyViewports();
    virtual void CreateResourceListener();
    virtual void LoadResources();

    // Ogre::FrameListener
    virtual bool frameRenderingQueued( const Ogre::FrameEvent& evt );

    // OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    // Ogre::WindowEventListener
    //Adjust mouse clipping area
    virtual void windowResized( Ogre::RenderWindow* rw );

    //Unattach OIS before window shutdown (very important under Linux)
    virtual void WindowClosed( Ogre::RenderWindow* rw );

    boost::shared_ptr<Ogre::Root> mRoot;
    Ogre::Camera* mCamera;
    Ogre::SceneManager* mSceneMgr;
    Ogre::RenderWindow* mWindow;
    CEGUI::OgreRenderer *mRenderer;

    // OgreBites
    OgreBites::SdkTrayManager* mTrayMgr;
    OgreBites::SdkCameraMan* mCameraMan;       // basic camera controller
    OgreBites::ParamsPanel* mDetailsPanel;     // sample details panel
    bool mCursorWasVisible;                    // was cursor visible before dialog appeared
    bool mShutDown;

    //OIS Input devices
    OIS::InputManager* mInputManager;
    OIS::Mouse*    mMouse;
    OIS::Keyboard* mKeyboard;


   public:
    Scene();
    virtual ~Scene();

    virtual bool Run();
    virtual void Stop();
    virtual void SetRoot( boost::shared_ptr<Ogre::Root> );
    virtual void SetWindow( Ogre::RenderWindow* );
  };

  extern CEGUI::MouseButton ConvertButton( OIS::MouseButtonID );
} // namespace viqo

#endif

