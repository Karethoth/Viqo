#ifndef __VIQO_SINGLETONS_SINSCENESTACK_HPP__
#define __VIQO_SINGLETONS_SINSCENESTACK_HPP__

#include "TemplateSingleton.hpp"
#include "../stacks/TemplateStack.hpp"
#include "../Scene.hpp"


namespace viqo
{
  namespace singletons
  {
    static TemplateSingleton< viqo::stacks::TemplateStack<Scene> > SinSceneStack;
  }
}

#endif

