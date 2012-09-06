#ifndef __VIQO_SINGLETONS_TEMPLATESINGLETON_HPP__
#define __VIQO_SINGLETONS_TEMPLATESINGLETON_HPP__


namespace viqo
{
  namespace singletons
  {
    template<typename T>
    class TemplateSingleton
    {
     protected:
      static T *instance;

     public:
      static T *Instance()
      {
        if( !TemplateSingleton::instance )
        {
          TemplateSingleton::instance = new T;
        }
        return instance;
      }
    };
    
    template<typename T>
    T* TemplateSingleton<T>::instance;
  }
}

#endif

