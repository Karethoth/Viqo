#ifndef __VIQO_SINGLETONS_SINSCENEMANAGER_HPP__
#define __VIQO_SINGLETONS_SINSCENEMANAGER_HPP__

#include "TemplateSingleton.hpp"
#include "../managers/SceneManager.hpp"


namespace viqo
{
  namespace singletons
  {
    extern TemplateSingleton<viqo::managers::SceneManager> SinSceneManager;
  }
}

#endif

