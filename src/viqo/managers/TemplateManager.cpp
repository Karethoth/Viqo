#include "TemplateManager.hpp"

using namespace viqo::managers;
using boost::shared_ptr;
using std::string;
using std::pair;


template<typename T>
TemplateManager<T>::~TemplateManager()
{
  items.clear();
}



template<typename T>
bool TemplateManager<T>::Add( string key, shared_ptr<T> item )
{
  items[key] = item;
  return true;
}



template<typename T>
shared_ptr<T> TemplateManager<T>::Get( string &key )
{
  return items[key];
}



template<typename T>
shared_ptr<T> TemplateManager<T>::Remove( string &key )
{
  shared_ptr<T> ret = items[key];
  items.erase[key];
  return ret;
}

