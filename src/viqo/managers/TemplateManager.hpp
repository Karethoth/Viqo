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
      virtual ~TemplateManager()
      {
        items.clear();
      }



      virtual bool Load( std::string& ) = 0;



      virtual bool Add( std::string &key, boost::shared_ptr<T> item )
      {
        items[key] = item;
        return true;
      }



      virtual boost::shared_ptr<T> Get( std::string &key )
      {
        return items[key];
      }



      virtual boost::shared_ptr<T> Get( const char *key )
      {
        return items[std::string(key)];
      }



      virtual boost::shared_ptr<T> Remove( std::string &key )
      {
        boost::shared_ptr<T> ret = items[key];
        items.erase(key);
        return ret;
      }



      virtual std::map< std::string, boost::shared_ptr<T> > *GetPointer()
      {
        return &items;
      }
    };


  } // Namespace viqo::managers
} // Namespace viqo

#endif

