#include <exception>
#include <inttypes.h>
#include "IfaceDOM-APISPEC.hxx"
#include <map>
#include <string>
#include <list>

class DOMNamespaceContext;

class DOMWriter
{
public:
  DOMWriter();

  void writeNode(DOMNamespaceContext* dnc,
                 iface::dom::Node* n, std::wstring& appendTo)
    throw(std::exception&);

  void writeElement(DOMNamespaceContext* dnc,
                    iface::dom::Element* el, std::wstring& appendTo)
    throw(std::exception&);
  void writeAttr(DOMNamespaceContext* dnc, iface::dom::Attr* at, std::wstring& appendTo)
    throw(std::exception&);
  void writeText(DOMNamespaceContext* dnc, iface::dom::Text* txt, std::wstring& appendTo)
    throw(std::exception&);
  void writeCDATASection(DOMNamespaceContext* dnc, iface::dom::CDATASection* cds, std::wstring& appendTo)
    throw(std::exception&);
  void writeEntity(DOMNamespaceContext* dnc, iface::dom::Entity* er, std::wstring& appendTo)
    throw(std::exception&);
  void writeEntityReference(DOMNamespaceContext* dnc, iface::dom::EntityReference* er,
                            std::wstring& appendTo)
    throw(std::exception&);
  void writeProcessingInstruction(DOMNamespaceContext* dnc, iface::dom::ProcessingInstruction* pi,
                                  std::wstring& appendTo)
    throw(std::exception&);
  void writeComment(DOMNamespaceContext* dnc, iface::dom::Comment* comment,
                    std::wstring& appendTo)
    throw(std::exception&);
  void writeDocument(DOMNamespaceContext* dnc, iface::dom::Document* doc, std::wstring& appendTo)
    throw(std::exception&);
  void writeDocumentType(DOMNamespaceContext* dnc, iface::dom::DocumentType* dt, std::wstring& appendTo)
    throw(std::exception&);
  void writeDocumentFragment(DOMNamespaceContext* dnc, iface::dom::DocumentFragment* df, std::wstring& appendTo)
    throw(std::exception);
  void writeNotation(DOMNamespaceContext* dnc, iface::dom::Notation* nt, std::wstring& appendTo)
    throw(std::exception);
};

class DOMNamespaceContext
{
public:
  DOMNamespaceContext(DOMNamespaceContext* aParent);

  void setDefaultNamespace(const std::wstring& newNS);
  void recordPrefix(const std::wstring& prefix, const std::wstring& ns);

  std::wstring getDefaultNamespace();
  std::wstring findNamespaceForPrefix(const std::wstring& prefix);
  std::wstring findPrefixForNamespace(const std::wstring& ns);
  void possiblyInventPrefix(const std::wstring& prefix);
  void resolveOrInventPrefixes();

  void writeXMLNS(std::wstring& appendTo);
private:
  DOMNamespaceContext* mParent;
  bool mOverrideDefaultNamespace;
  std::wstring defaultNamespace;
  std::map<std::wstring,std::wstring> URIfromPrefix;
  std::map<std::wstring,std::wstring> PrefixfromURI;
  std::list<std::wstring> NamespacesNeedingPrefixes;
};
