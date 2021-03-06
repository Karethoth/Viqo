#ifndef __VIQO_STACKS_TEMPLATESTACK_HPP__
#define __VIQO_STACKS_TEMPLATESTACK_HPP__

#include <vector>
#include <boost/shared_ptr.hpp>

namespace viqo
{
  namespace stacks
  {

    template<typename T>
    class TemplateStack
    {
     protected:
      std::vector< boost::shared_ptr<T> > stack;


     public:
      virtual ~TemplateStack()
      {
        stack.clear();
      }



      virtual bool Push( boost::shared_ptr<T> item )
      {
        stack.insert( stack.begin(), item );
        return true;
      }



      virtual boost::shared_ptr<T> Get( unsigned int index )
      {
        return stack.at( index );
      }



      virtual boost::shared_ptr<T> Pop()
      {
        boost::shared_ptr<T> ret = stack.front();
        stack.clear();
        return ret;
      }
    };


  } // Namespace viqo::stacks
} // Namespace viqo

#endif

