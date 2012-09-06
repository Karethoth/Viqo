#ifndef __VIQO_MANAGERS_SCENEMANAGER_HPP__
#define __VIQO_MANAGERS_SCENEMANAGER_HPP__

#include "TemplateManager.hpp"
#include "../Scene.hpp"

namespace viqo
{ 
  namespace managers
  {
    class SceneManager : public TemplateManager<Scene>
    {
     public:
      virtual bool Load( std::string& );
    };
  } // namespace viqo::managers
} // namespace viqo

#endif

