#ifndef INCLUDED_p2jxpcom
#define INCLUDED_p2jxpcom

#include <exception>
#include <string>
#include "Utilities.hxx"
#include "Ifacexpcom.hxx"
#include "pick-jni.h"
#include <map>

#ifdef IN_MODULE_JavaSupport
#define PUBLIC_JAVA_PRE CDA_EXPORT_PRE
#define PUBLIC_JAVA_POST CDA_EXPORT_POST
#else
#define PUBLIC_JAVA_PRE CDA_IMPORT_PRE
#define PUBLIC_JAVA_POST CDA_IMPORT_POST
#endif

namespace p2j
{
  namespace XPCOM
  {
    PUBLIC_JAVA_PRE class PUBLIC_JAVA_POST IObject
      : public virtual iface::XPCOM::IObject
    {
    public:
      CDA_IMPL_REFCOUNT;

      IObject() {}
      PUBLIC_JAVA_PRE IObject(JNIEnv* aEnv, jobject aObject) PUBLIC_JAVA_POST;
      PUBLIC_JAVA_PRE ~IObject() PUBLIC_JAVA_POST;

      PUBLIC_JAVA_PRE std::string objid() throw() PUBLIC_JAVA_POST;

      PUBLIC_JAVA_PRE void* query_interface(const std::string&) throw() PUBLIC_JAVA_POST;
      PUBLIC_JAVA_PRE std::vector<std::string> supported_interfaces() throw() PUBLIC_JAVA_POST;

      jobject unwrap() { return mObject; }

    protected:
      JavaVM* mVM;
      jobject mObject;
    };
  };
};

PUBLIC_JAVA_PRE jobject wrap_XPCOM_IObject(JNIEnv* aEnv, iface::XPCOM::IObject* aObj) PUBLIC_JAVA_POST;

PUBLIC_JAVA_PRE jstring ConvertWcharStringToJString(JNIEnv* env, const wchar_t* aString) PUBLIC_JAVA_POST;

PUBLIC_JAVA_PRE class PUBLIC_JAVA_POST P2JFactory
{
public:
  P2JFactory(const char* aIfaceName, const char* aJavaName)
    : ifaceName(aIfaceName), javaName(aJavaName)
  {
    P2JFactory::registerP2JFactory(this);
  }

  static P2JFactory* findP2J(const std::string& name)
  {
    if (mMap == NULL)
      return NULL;

    std::map<std::string, P2JFactory*>::iterator i = mMap->find(name);

    if (i == mMap->end())
      return NULL;

    return (*i).second;
  }

  PUBLIC_JAVA_PRE static void registerP2JFactory(P2JFactory* f) PUBLIC_JAVA_POST;

  const char* pcmInterfaceClass()
  {
    return ifaceName;
  }

  const char* javaInterfaceClass()
  {
    return javaName;
  }

  virtual void* create(JNIEnv* env, jobject obj) = 0;

private:
  const char * ifaceName, * javaName;
  static std::map<std::string, P2JFactory*>* mMap;
};

#endif // INCLUDED_p2jxpcom
