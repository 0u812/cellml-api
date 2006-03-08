class CDA_ModelLoader
  : public iface::cellml_api::DOMModelLoader
{
public:
  CDA_ModelLoader(iface::cellml_api::DOMURLLoader* aURLLoader);
  virtual ~CDA_ModelLoader() { mURLLoader->release_ref(); }

  CDA_IMPL_REFCOUNT
  CDA_IMPL_QI2(cellml_api::DOMModelLoader, cellml_api::ModelLoader);
  CDA_IMPL_COMPARE_NAIVE(CDA_ModelLoader);

  iface::cellml_api::Model* loadFromURL(const wchar_t* URL)
    throw(std::exception&);
  void asyncLoadFromURL(const wchar_t* URL,
                        iface::cellml_api::ModelLoadedListener* listener)
    throw(std::exception&);
  wchar_t* lastErrorMessage() throw(std::exception&);

  iface::cellml_api::Model*
  createFromDOM(const wchar_t* url,
                iface::cellml_api::DOMURLLoader* loader)
    throw(std::exception&);
  void asyncCreateFromDOM(const wchar_t* URL,
                          iface::cellml_api::DOMURLLoader* loader,
                          iface::cellml_api::ModelLoadedListener* listener)
    throw(std::exception&);

  std::wstring mLastError;
private:
  iface::cellml_api::DOMURLLoader* mURLLoader;
};

class CDA_DOMURLLoader
  : public iface::cellml_api::DOMURLLoader
{
public:
  CDA_DOMURLLoader(CDA_DOMImplementation* aDOMImpl);
  virtual ~CDA_DOMURLLoader() { mDOMImpl->release_ref(); }

  CDA_IMPL_REFCOUNT
  CDA_IMPL_QI1(cellml_api::DOMURLLoader);
  CDA_IMPL_COMPARE_NAIVE(CDA_DOMURLLoader);

  iface::dom::Document* loadDocument(const wchar_t* URL)
    throw(std::exception&);
  void asyncLoadDocument(const wchar_t* URL,
                         iface::cellml_api::DocumentLoadedListener* listener)
    throw(std::exception&);
  wchar_t* lastErrorMessage() throw(std::exception&);
private:
  CDA_DOMImplementation* mDOMImpl;
  std::wstring mLastError;
};

class CDA_CellMLBootstrap
  : public iface::cellml_api::CellMLBootstrap
{
public:
  CDA_CellMLBootstrap();
  virtual ~CDA_CellMLBootstrap() { domimpl->release_ref(); }

  CDA_IMPL_REFCOUNT
  CDA_IMPL_QI1(cellml_api::CellMLBootstrap);
  CDA_IMPL_COMPARE_NAIVE(CDA_CellMLBootstrap);

  iface::cellml_api::DOMModelLoader* modelLoader() throw(std::exception&);
  iface::dom::DOMImplementation* domImplementation() throw(std::exception&);
  iface::cellml_api::DOMURLLoader* localURLLoader() throw(std::exception&);
private:
  CDA_DOMImplementation* domimpl;
};
