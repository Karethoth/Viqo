#ifndef __TESTSCENE_HPP__
#define __TESTSCENE_HPP__

#include "ViqoScene.hpp"


class TestScene : public ViqoScene
{
 protected:
  virtual void CreateScene();
  virtual void DestroyScene();
  //virtual void CreateCamera();
  //virtual void CreateViewports();

 public:
  TestScene();
  virtual ~TestScene();

  //virtual void Run();

};

#endif

