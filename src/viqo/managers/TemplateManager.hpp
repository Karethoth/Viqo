#ifndef __VIQO_MANAGERS_TEMPLATEMANAGER_HPP__
#define __VIQO_MANAGERS_TEMPLATEMANAGER_HPP__

#include <map>
#include <boost/shared_ptr.hpp>

namespace viqo
{
  namespace managers
  {

    template<typename T>
    class TemplateManager
    {
     protected:
      std::map< std::string, boost::shared_ptr<T> > items;


     public:
      virtual ~TemplateManager();

      virtual bool Load( std::string& ) = 0;
      virtual bool Add( std::string, boost::shared_ptr<T> );

      virtual boost::shared_ptr<T> Get( std::string& );
      virtual boost::shared_ptr<T> Remove( std::string& );
    };


  } // Namespace viqo::managers
} // Namespace viqo

#endif

