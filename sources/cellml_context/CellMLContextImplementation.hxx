#include <exception>
#include <inttypes.h>
#include <map>
#include <string>
#include "IfaceCellML-Context.hxx"
#include "Utilities.hxx"

class CDA_ModuleManager;

class CDA_CellMLModuleIterator
  : public iface::cellml_context::CellMLModuleIterator
{
public:
  CDA_CellMLModuleIterator(CDA_ModuleManager* aMM,
                           std::list<iface::cellml_context::CellMLModule*>&
                           aList);
  ~CDA_CellMLModuleIterator();

  CDA_IMPL_REFCOUNT;
  CDA_IMPL_COMPARE_NAIVE(CDA_CellMLModuleIterator);
  CDA_IMPL_QI1(cellml_context::CellMLModuleIterator)

  iface::cellml_context::CellMLModule* nextModule()
    throw(std::exception&);

private:
  CDA_ModuleManager* mMM;
  std::list<iface::cellml_context::CellMLModule*>& mList;
  std::list<iface::cellml_context::CellMLModule*>::iterator
    mCurrent;
};

class CDA_ModelNode;
class CDA_ModelList;

class CDA_ModelNodeIterator
  : public iface::cellml_context::ModelNodeIterator
{
public:
  CDA_ModelNodeIterator(CDA_ModelList* aML,
                        std::list<CDA_ModelNode*>&
                        aList);
  ~CDA_ModelNodeIterator();

  CDA_IMPL_REFCOUNT;
  CDA_IMPL_COMPARE_NAIVE(CDA_ModelNodeIterator);
  CDA_IMPL_QI1(cellml_context::ModelNodeIterator)

  iface::cellml_context::ModelNode* nextModelNode()
    throw(std::exception&);

private:
  CDA_ModelList* mML;
  std::list<CDA_ModelNode*> mList;
  std::list<CDA_ModelNode*>::iterator
    mCurrent;
};

class CDA_TypeAnnotationManager
  : public iface::cellml_context::CellMLTypeAnnotationManager
{
public:
  CDA_TypeAnnotationManager() {}
  ~CDA_TypeAnnotationManager();

  CDA_IMPL_REFCOUNT;
  CDA_IMPL_COMPARE_NAIVE(CDA_TypeAnnotationManager);
  CDA_IMPL_QI1(cellml_context::CellMLTypeAnnotationManager);

  void setUserData(const wchar_t* type, const wchar_t* key,
                   iface::XPCOM::IObject* data)
    throw(std::exception&);
  iface::XPCOM::IObject* getUserData(const wchar_t* type, const wchar_t* key)
    throw(std::exception&);

private:
  std::map<std::pair<std::wstring,std::wstring>, iface::XPCOM::IObject*>
    annotations;
};

class CDA_ModuleManager
  : public iface::cellml_context::CellMLModuleManager
{
public:
  CDA_ModuleManager();
  ~CDA_ModuleManager();

  CDA_IMPL_REFCOUNT;
  CDA_IMPL_COMPARE_NAIVE(CDA_ModuleManager);
  CDA_IMPL_QI1(cellml_context::CellMLModuleManager);

  void registerModule(iface::cellml_context::CellMLModule* aModule)
    throw(std::exception&);
  void deregisterModule(iface::cellml_context::CellMLModule* aModule)
    throw(std::exception&);
  iface::cellml_context::CellMLModule* findModuleByName
  (const wchar_t* moduleName, const wchar_t* moduleVersion)
    throw(std::exception&);
  void requestModuleByName(const wchar_t* moduleName,
                           const wchar_t* moduleVersion)
    throw(std::exception&);
  void addMonitor(iface::cellml_context::CellMLModuleMonitor* aModuleMonitor)
    throw(std::exception&);
  void removeMonitor(iface::cellml_context::CellMLModuleMonitor*
                     aModuleMonitor)
    throw(std::exception&);
  iface::cellml_context::CellMLModuleIterator* iterateModules()
    throw(std::exception&);

private:
  std::map<std::pair<std::wstring,std::wstring>,
           iface::cellml_context::CellMLModule*> mRegisteredModules;
  std::list<iface::cellml_context::CellMLModule*> mRegisteredModuleList;
  std::list<iface::cellml_context::CellMLModuleMonitor*> mMonitors;
};

class CDA_ModelList;

class CDA_ModelNode
  : public iface::cellml_context::ModelNode
{
public:
  CDA_ModelNode(iface::cellml_api::Model* aModel);
  ~CDA_ModelNode();
  
  CDA_IMPL_REFCOUNT;
  CDA_IMPL_QI1(cellml_context::ModelNode);
  CDA_IMPL_COMPARE_NAIVE(CDA_ModelNode);

  void name(const wchar_t* name) throw(std::exception&);
  wchar_t* name() throw(std::exception&);
  iface::cellml_context::ModelNode* getLatestDerivative()
    throw(std::exception&);
  iface::cellml_context::ModelNode* getWritable() throw(std::exception&);
  bool isFrozen() throw(std::exception&);
  void freeze() throw(std::exception&);
  uint32_t modificationTimestamp() throw(std::exception&);
  void stampModifiedNow() throw(std::exception&);
  iface::cellml_api::Model* model() throw(std::exception&);
  iface::cellml_context::ModelList* derivedModels() throw(std::exception&);
  void addModelMonitor(iface::cellml_context::ModelNodeMonitor* monitor)
    throw(std::exception&);
  void removeModelMonitor(iface::cellml_context::ModelNodeMonitor* monitor)
    throw(std::exception&);

private:
  std::wstring mName;
  uint32_t mTimestamp;
  iface::cellml_api::Model* mModel;
  CDA_ModelList* mDerivedModels;
  bool mIsFrozen;
  std::list<iface::cellml_context::ModelNodeMonitor*> mModelMonitors;
public: // within CellMLContextImplementation only...
  // Not refcounted, but set/cleared automatically when added/removed.
  CDA_ModelList* mParentList;
};

class CDA_ModelList
  : public iface::cellml_context::ModelList
{
public:
  CDA_ModelList();
  ~CDA_ModelList();

  CDA_IMPL_REFCOUNT;
  CDA_IMPL_QI1(cellml_context::ModelList);
  CDA_IMPL_COMPARE_NAIVE(CDA_ModelList);

  void addModelMonitor(iface::cellml_context::ModelNodeMonitor* monitor)
    throw(std::exception&);
  void removeModelMonitor(iface::cellml_context::ModelNodeMonitor* monitor)
    throw(std::exception&);
  void addListMonitor(iface::cellml_context::ModelListMonitor* monitor)
    throw(std::exception&);
  void removeListMonitor(iface::cellml_context::ModelListMonitor* monitor)
    throw(std::exception&);
  iface::cellml_context::ModelNode* makeNode(iface::cellml_api::Model* mod)
    throw(std::exception&);
  void addModel(iface::cellml_context::ModelNode* node)
    throw(std::exception&);
  void removeModel(iface::cellml_context::ModelNode* node)
    throw(std::exception&);
  iface::cellml_context::ModelNodeIterator* iterateModelNodes()
    throw(std::exception&);

private:
  std::list<CDA_ModelNode*> mModels;
public: // within CellMLContextImplementation only...
  // Not refcounted, but set/cleared automatically when added/removed.
  CDA_ModelNode* mParentNode;
  std::list<iface::cellml_context::ModelNodeMonitor*> mNodeMonitors;
  std::list<iface::cellml_context::ModelListMonitor*> mListMonitors;
};

class CDA_CellMLContext
  : public iface::cellml_context::CellMLContext
{
public:
  CDA_CellMLContext();
  ~CDA_CellMLContext();

  CDA_IMPL_REFCOUNT;
  CDA_IMPL_QI1(cellml_context::CellMLContext);
  CDA_IMPL_COMPARE_NAIVE(CDA_CellMLContext);

  iface::cellml_context::CellMLModuleManager*
  moduleManager()
    throw(std::exception&)
  {
    mModuleManager->add_ref();
    return mModuleManager;
  }

  iface::cellml_context::CellMLTypeAnnotationManager*
  typeAnnotationManager()
    throw(std::exception&)
  {
    mTypeAnnotationManager->add_ref();
    return mTypeAnnotationManager;
  }

  iface::cellml_api::CellMLBootstrap*
  cellmlBoostrap()
    throw(std::exception&)
  {
    mCellMLBootstrap->add_ref();
    return mCellMLBootstrap;
  }

  iface::cellml_context::ModelList*
  loadedModels()
    throw(std::exception&)
  {
    mModelList->add_ref();
    return mModelList;
  }

private:
  iface::cellml_context::CellMLModuleManager* mModuleManager;
  iface::cellml_context::CellMLTypeAnnotationManager* mTypeAnnotationManager;
  iface::cellml_api::CellMLBootstrap* mCellMLBootstrap;
  iface::cellml_context::ModelList* mModelList;
};
