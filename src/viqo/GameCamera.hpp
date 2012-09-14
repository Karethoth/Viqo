#ifndef __VIQO_GAMECAMERA_HPP__
#define __VIQO_GAMECAMERA_HPP__

#include <OgreCamera.h>
#include <OgreSceneManager.h>
#include <OgreFrameListener.h>
#include <OISKeyboard.h>
#include <OISMouse.h>


namespace viqo
{
  class GameCamera: public OIS::KeyListener,
                    public OIS::MouseListener
  {
   protected:
    Ogre::Camera       *cam;
    Ogre::SceneNode    *camNode;
    Ogre::SceneManager *sceneMan;

    Ogre::Vector3 location;
    Ogre::Vector3 velocity;

    bool upKey, downKey, rightKey, leftKey;


   public:
    GameCamera( Ogre::SceneManager* );
    ~GameCamera();

    Ogre::Camera *GetCamera();

    void Update();

    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );

    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
  };
}

#endif

