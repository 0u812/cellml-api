#include "p2jxpcom.hxx"

p2j::XPCOM::IObject::IObject(JNIEnv* aEnv, jobject aObject)
  : env(aEnv), mObject(env->NewGlobalRef(aObject)), _cda_refcount(1)
{
}

p2j::XPCOM::IObject::~IObject()
{
  env->DeleteGlobalRef(mObject);
}

char*
p2j::XPCOM::IObject::objid()
  throw()
{
  jclass jlo = env->FindClass("java/lang/Object");
  jmethodID meth = env->GetMethodID(jlo, "hashCode", "()I");
  jint ret = env->CallIntMethod(mObject, meth);
  char* buf = reinterpret_cast<char*>(malloc(21));
  snprintf(buf, 20, "%lu", ret);

  return buf;
};

void*
p2j::XPCOM::IObject::query_interface(const char* name) throw()
{
  P2JFactory* f = P2JFactory::findP2J(name);

  if (f == NULL)
  {
    if (!strcmp(name, "XPCOM::IObject"))
      return reinterpret_cast<void*>(static_cast<iface::XPCOM::IObject*>(this));
    return NULL;
  }

  // Check if it is legal to cast to this interface...
  jclass clazz = env->FindClass(f->javaInterfaceClass());
  if (!env->IsInstanceOf(mObject, clazz))
    return NULL;

  // Type check passed, so allow it...
  return f->create(env, mObject);
}

jstring
ConvertWcharStringToJString(JNIEnv* env, const wchar_t* aString)
{
  uint32_t l = wcslen(aString);
  jchar* buf = new jchar[l];
  for (uint32_t i = 0; i < l; i++)
    buf[i] = static_cast<jchar>(aString[i]);

  jstring ret = env->NewString(buf, l);
  delete [] buf;

  return ret;
}

std::map<std::string, P2JFactory*>* P2JFactory::mMap = NULL;

jobject
wrap_XPCOM_IObject(JNIEnv* aEnv, iface::XPCOM::IObject* aObj)
{
  // We only support unwrapping here since we can't annotate java.lang.Object adequately...
  p2j::XPCOM::IObject* pio = dynamic_cast<p2j::XPCOM::IObject*>(aObj);
  if (pio == NULL)
    return NULL;

  return aEnv->NewLocalRef(pio->unwrap());
}
