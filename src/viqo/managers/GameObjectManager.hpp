#ifndef __VIQO_MANAGERS_GAMEOBJECTMANAGER_HPP__
#define __VIQO_MANAGERS_GAMEOBJECTMANAGER_HPP__

#include "TemplateManager.hpp"
#include "../gameworld/GameObject.hpp"

namespace viqo
{ 
  namespace managers
  {
    class GameObjectManager : public TemplateManager<viqo::gameworld::GameObject>
    {
     public:
      virtual bool Load( std::string& );
    };
  } // namespace viqo::managers
} // namespace viqo

#endif

