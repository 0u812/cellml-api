#include <exception>
#include <sys/types.h>
#include "IfaceDOM-APISPEC.hxx"
#include "IfaceMathML-content-APISPEC.hxx"
#include <libgdome/gdome.h>
#include <libgdome/gdome-events.h>
#include "ThreadWrapper.hxx"
#include <map>

class CDA_DOMImplementation
  : public virtual iface::dom::DOMImplementation,
    public virtual iface::mathml_dom::MathMLDOMImplementation
{
public:
  CDA_IMPL_REFCOUNT
  CDA_IMPL_QI2(dom::DOMImplementation, mathml_dom::MathMLDOMImplementation)

  CDA_DOMImplementation();
  CDA_DOMImplementation(GdomeDOMImplementation*);
  virtual ~CDA_DOMImplementation();

  bool hasFeature(const iface::dom::DOMString feature,
                  const iface::dom::DOMString version)
    throw(std::exception&);

  iface::dom::DocumentType* createDocumentType
  (
   const iface::dom::DOMString qualifiedName,
   const iface::dom::DOMString publicId,
   const iface::dom::DOMString systemId
  )
    throw(std::exception&);

  iface::dom::Document* createDocument
  (
   const iface::dom::DOMString namespaceURI,
   const iface::dom::DOMString qualifiedName,
   iface::dom::DocumentType* doctype
  )
    throw(std::exception&);

  iface::mathml_dom::MathMLDocument* createMathMLDocument()
    throw(std::exception&);
private:
  GdomeDOMImplementation* impl;
};

class CDA_Node
  : public virtual iface::dom::Node
{
public:
  CDA_Node() {}
  virtual ~CDA_Node() {};

  iface::dom::DOMString nodeName() throw(std::exception&);
  iface::dom::DOMString nodeValue() throw(std::exception&);
  void nodeValue(iface::dom::DOMString attr) throw(std::exception&);
  u_int16_t nodeType() throw(std::exception&);
  iface::dom::Node* parentNode() throw(std::exception&);
  iface::dom::NodeList* childNodes() throw(std::exception&);
  iface::dom::Node* firstChild() throw(std::exception&);
  iface::dom::Node* lastChild() throw(std::exception&);
  iface::dom::Node* previousSibling() throw(std::exception&);
  iface::dom::Node* nextSibling() throw(std::exception&);
  iface::dom::NamedNodeMap* attributes() throw(std::exception&);
  iface::dom::Document* ownerDocument() throw(std::exception&);
  iface::dom::Node* insertBefore(iface::dom::Node* newChild,
                                 iface::dom::Node* refChild)
    throw(std::exception&);
  iface::dom::Node* replaceChild(iface::dom::Node* newChild,
                                 iface::dom::Node* oldChild)
    throw(std::exception&);
  iface::dom::Node* removeChild(iface::dom::Node* oldChild)
    throw(std::exception&);
  iface::dom::Node* appendChild(iface::dom::Node* newChild)
    throw(std::exception&);
  bool hasChildNodes() throw(std::exception&);
  iface::dom::Node* cloneNode(bool deep) throw(std::exception&);
  void normalize() throw(std::exception&);
  bool isSupported(const iface::dom::DOMString feature,
                   const iface::dom::DOMString version) throw(std::exception&);
  iface::dom::DOMString namespaceURI() throw(std::exception&);
  iface::dom::DOMString prefix() throw(std::exception&);
  void prefix(iface::dom::DOMString attr) throw(std::exception&);
  iface::dom::DOMString localName() throw(std::exception&);
  bool hasAttributes() throw(std::exception&);
  void addEventListener(const iface::dom::DOMString type,
                        iface::events::EventListener* listener,
                        bool useCapture) throw(std::exception&);
  void removeEventListener(const iface::dom::DOMString type,
                           iface::events::EventListener* listener,
                           bool useCapture) throw(std::exception&);
  bool dispatchEvent(iface::events::Event* evt) throw(std::exception&);

  /* Implementation only... */
  virtual GdomeNode* fetchNode() const = 0;

private:
  struct EventListenerData
  {
    ~EventListenerData()
    {
      if (listener != NULL)
      {
        EXCEPTION_TRY;
        gdome_evntl_unref(listener, &exc);
        // No catch, if it fails let it fail.
      }
      if (callee != NULL)
      {
        callee->release_ref();
      }
    }
    u_int32_t copies;
    GdomeEventListener* listener;
    iface::events::EventListener* callee;
  };
  static std::map<iface::events::EventListener*,struct EventListenerData*> activeEventListeners;
  static CDAMutex aelprotect;
};

class CDA_NodeList
  : public iface::dom::NodeList
{
public:
  CDA_IMPL_REFCOUNT
  CDA_IMPL_QI1(dom::NodeList)

  CDA_NodeList(GdomeNodeList* nl);
  virtual ~CDA_NodeList();

  iface::dom::Node* item(u_int32_t index) throw(std::exception&);
  u_int32_t length() throw(std::exception&);

  GdomeNodeList* impl;
};

class CDA_NamedNodeMap
  : public iface::dom::NamedNodeMap
{
public:
  CDA_NamedNodeMap(GdomeNamedNodeMap* nnm);
  virtual ~CDA_NamedNodeMap();

  CDA_IMPL_REFCOUNT
  CDA_IMPL_QI1(dom::NamedNodeMap)

  iface::dom::Node* getNamedItem(const iface::dom::DOMString name)
    throw(std::exception&);
  iface::dom::Node* setNamedItem(iface::dom::Node* arg)
    throw(std::exception&);
  iface::dom::Node* removeNamedItem(const iface::dom::DOMString name)
    throw(std::exception&);
  iface::dom::Node* item(u_int32_t index) throw(std::exception&);
  u_int32_t length() throw(std::exception&);
  iface::dom::Node* getNamedItemNS(const iface::dom::DOMString namespaceURI,
                                   const iface::dom::DOMString localName)
    throw(std::exception&);
  iface::dom::Node* setNamedItemNS(iface::dom::Node* arg)
    throw(std::exception&);
  iface::dom::Node* removeNamedItemNS(const iface::dom::DOMString namespaceURI,
                                      const iface::dom::DOMString localName)
    throw(std::exception&);

  GdomeNamedNodeMap* impl;
};

class CDA_CharacterData
  : public virtual iface::dom::CharacterData,
    public CDA_Node
{
public:
  iface::dom::DOMString data() throw(std::exception&);
  void data(iface::dom::DOMString attr) throw(std::exception&);
  u_int32_t length() throw(std::exception&);
  iface::dom::DOMString substringData(u_int32_t offset, u_int32_t count)
    throw(std::exception&);
  void appendData(const iface::dom::DOMString arg) throw(std::exception&);
  void insertData(u_int32_t offset, const iface::dom::DOMString arg)
    throw(std::exception&);
  void deleteData(u_int32_t offset, u_int32_t count)
    throw(std::exception&);
  void replaceData(u_int32_t offset, u_int32_t count,
                   const iface::dom::DOMString arg) throw(std::exception&);

  /* Implementation only... */
  virtual GdomeCharacterData* fetchCData() const = 0;
};

class CDA_Attr
  : public iface::dom::Attr,
    public CDA_Node
{
public:
  CDA_Attr(GdomeAttr* at);
  virtual ~CDA_Attr();

  CDA_IMPL_REFCOUNT
  CDA_IMPL_QI2(dom::Node, dom::Attr)

  iface::dom::DOMString name() throw(std::exception&);
  bool specified() throw(std::exception&);
  iface::dom::DOMString value() throw(std::exception&);
  void value(iface::dom::DOMString attr) throw(std::exception&);
  iface::dom::Element* ownerElement() throw(std::exception&);

  GdomeAttr* impl;
  GdomeNode* fetchNode() const;
};

class CDA_Element
  : public virtual iface::dom::Element,
    public virtual CDA_Node
{
public:
  CDA_Element(GdomeElement* el);
  virtual ~CDA_Element();

  CDA_IMPL_REFCOUNT
  CDA_IMPL_QI2(dom::Node, dom::Element)

  iface::dom::DOMString tagName()
    throw(std::exception&);
  iface::dom::DOMString getAttribute(const iface::dom::DOMString name)
    throw(std::exception&);
  void setAttribute(const iface::dom::DOMString name,
                    const iface::dom::DOMString value) throw(std::exception&);
  void removeAttribute(const iface::dom::DOMString name)
    throw(std::exception&);
  iface::dom::Attr* getAttributeNode(const iface::dom::DOMString name)
    throw(std::exception&);
  iface::dom::Attr* setAttributeNode(iface::dom::Attr* newAttr)
    throw(std::exception&);
  iface::dom::Attr* removeAttributeNode(iface::dom::Attr* oldAttr)
    throw(std::exception&);
  iface::dom::NodeList* getElementsByTagName(const iface::dom::DOMString name)
    throw(std::exception&);
  iface::dom::DOMString getAttributeNS(const iface::dom::DOMString namespaceURI,
                                       const iface::dom::DOMString localName)
    throw(std::exception&);
  void setAttributeNS(const iface::dom::DOMString namespaceURI,
                      const iface::dom::DOMString qualifiedName,
                      const iface::dom::DOMString value)
    throw(std::exception&);
  void removeAttributeNS(const iface::dom::DOMString namespaceURI,
                         const iface::dom::DOMString localName)
    throw(std::exception&);
  iface::dom::Attr* getAttributeNodeNS(const iface::dom::DOMString namespaceURI,
                                       const iface::dom::DOMString localName)
    throw(std::exception&);
  iface::dom::Attr* setAttributeNodeNS(iface::dom::Attr* newAttr)
    throw(std::exception&);
  iface::dom::NodeList*
  getElementsByTagNameNS(const iface::dom::DOMString namespaceURI,
                         const iface::dom::DOMString localName)
    throw(std::exception&);
  bool hasAttribute(const iface::dom::DOMString name) throw(std::exception&);
  bool hasAttributeNS(const iface::dom::DOMString namespaceURI,
                      const iface::dom::DOMString localName)
    throw(std::exception&);

  GdomeElement* impl;
  GdomeNode* fetchNode() const;
};

class CDA_TextBase
  : public virtual iface::dom::Text,
    public virtual CDA_CharacterData
{
public:
  iface::dom::Text* splitText(u_int32_t offset) throw(std::exception&);

  virtual GdomeText* fetchText() const = 0;
};

class CDA_Text
  : public virtual CDA_TextBase
{
public:
  CDA_Text(GdomeText* txt);
  virtual ~CDA_Text();
  
  CDA_IMPL_REFCOUNT
  CDA_IMPL_QI3(dom::Text, dom::CharacterData, dom::Node)

  GdomeText* impl;
  GdomeNode* fetchNode() const;
  GdomeCharacterData* fetchCData() const;
  GdomeText* fetchText() const;
};

class CDA_Comment
  : public iface::dom::Comment,
    public CDA_CharacterData
{
public:
  CDA_Comment(GdomeComment* c);
  virtual ~CDA_Comment();

  CDA_IMPL_REFCOUNT
  CDA_IMPL_QI3(dom::Comment, dom::CharacterData, dom::Node)

  GdomeComment* impl;
  GdomeNode* fetchNode() const;
  GdomeCharacterData* fetchCData() const;
};

class CDA_CDATASection
  : public iface::dom::CDATASection,
    public CDA_TextBase
{
public:
  CDA_CDATASection(GdomeCDATASection* cds);
  virtual ~CDA_CDATASection();

  CDA_IMPL_REFCOUNT
  CDA_IMPL_QI4(dom::CDATASection, dom::Text, dom::CharacterData, dom::Node)

  GdomeCDATASection* impl;
  GdomeNode* fetchNode() const;
  GdomeCharacterData* fetchCData() const;
  GdomeText* fetchText() const;
};

class CDA_DocumentType
  : public virtual CDA_Node,
    public virtual iface::dom::DocumentType
{
public:
  CDA_DocumentType(GdomeDocumentType* dt);
  virtual ~CDA_DocumentType();

  CDA_IMPL_REFCOUNT
  CDA_IMPL_QI2(dom::DocumentType, dom::Node)

  iface::dom::DOMString name() throw(std::exception&);
  iface::dom::NamedNodeMap* entities() throw(std::exception&);
  iface::dom::NamedNodeMap* notations() throw(std::exception&);
  iface::dom::DOMString publicId() throw(std::exception&);
  iface::dom::DOMString systemId() throw(std::exception&);
  iface::dom::DOMString internalSubset() throw(std::exception&);

  GdomeDocumentType* impl;
  GdomeNode* fetchNode() const;
};

class CDA_Notation
  : public iface::dom::Notation,
    public CDA_Node
{
public:
  CDA_Notation(GdomeNotation* nt);
  virtual ~CDA_Notation();

  CDA_IMPL_REFCOUNT
  CDA_IMPL_QI2(dom::Notation, dom::Node)

  iface::dom::DOMString publicId() throw(std::exception&);
  iface::dom::DOMString systemId() throw(std::exception&);

  GdomeNotation* impl;
  GdomeNode* fetchNode() const;
};

class CDA_Entity
  : public iface::dom::Entity,
    public CDA_Node
{
public:
  CDA_Entity(GdomeEntity* ent);
  virtual ~CDA_Entity();

  CDA_IMPL_REFCOUNT
  CDA_IMPL_QI2(dom::Entity, dom::Node)

  iface::dom::DOMString publicId() throw(std::exception&);
  iface::dom::DOMString systemId() throw(std::exception&);
  iface::dom::DOMString notationName() throw(std::exception&);

  GdomeEntity* impl;
  GdomeNode* fetchNode() const;
};

class CDA_EntityReference
  : public iface::dom::EntityReference,
    public CDA_Node
{
public:
  CDA_EntityReference(GdomeEntityReference* ent);
  virtual ~CDA_EntityReference();

  CDA_IMPL_REFCOUNT
  CDA_IMPL_QI2(dom::EntityReference, dom::Node)

  GdomeEntityReference* impl;
  GdomeNode* fetchNode() const;
};

class CDA_ProcessingInstruction
  : public iface::dom::ProcessingInstruction,
    public CDA_Node
{
public:
  CDA_ProcessingInstruction(GdomeProcessingInstruction* pri);
  virtual ~CDA_ProcessingInstruction();

  CDA_IMPL_REFCOUNT
  CDA_IMPL_QI2(dom::ProcessingInstruction, dom::Node)

  iface::dom::DOMString target() throw(std::exception&);
  iface::dom::DOMString data() throw(std::exception&);
  void data(iface::dom::DOMString attr) throw(std::exception&);

  GdomeProcessingInstruction* impl;
  GdomeNode* fetchNode() const;
};

class CDA_DocumentFragment
  : public iface::dom::DocumentFragment,
    public CDA_Node
{
public:
  CDA_DocumentFragment(GdomeDocumentFragment* df);
  virtual ~CDA_DocumentFragment();

  CDA_IMPL_REFCOUNT
  CDA_IMPL_QI2(dom::DocumentFragment, dom::Node)

  GdomeDocumentFragment* impl;
  GdomeNode* fetchNode() const;
};

class CDA_Document
  : public virtual iface::dom::Document,
    public virtual CDA_Node
{
public:
  CDA_Document(GdomeDocument* doc);
  virtual ~CDA_Document();

  CDA_IMPL_REFCOUNT
  CDA_IMPL_QI2(dom::Document, dom::Node)

  iface::dom::DocumentType* doctype()
    throw(std::exception&);
  iface::dom::DOMImplementation* implementation()
    throw(std::exception&);
  iface::dom::Element* documentElement()
    throw(std::exception&);
  iface::dom::Element* createElement(const iface::dom::DOMString tagName)
    throw(std::exception&);
  iface::dom::DocumentFragment* createDocumentFragment()
    throw(std::exception&);
  iface::dom::Text* createTextNode(const iface::dom::DOMString data)
    throw(std::exception&);
  iface::dom::Comment* createComment(const iface::dom::DOMString data)
    throw(std::exception&);
  iface::dom::CDATASection* createCDATASection(const iface::dom::DOMString data)
    throw(std::exception&);
  iface::dom::ProcessingInstruction* createProcessingInstruction
  (const iface::dom::DOMString target, const iface::dom::DOMString data)
    throw(std::exception&);
  iface::dom::Attr* createAttribute(const iface::dom::DOMString name)
    throw(std::exception&);
  iface::dom::EntityReference* createEntityReference
    (const iface::dom::DOMString name) throw(std::exception&);
  iface::dom::NodeList* getElementsByTagName
    (const iface::dom::DOMString tagname) throw(std::exception&);
  iface::dom::Node* importNode(iface::dom::Node* importedNode, bool deep)
    throw(std::exception&);
  iface::dom::Element*
    createElementNS(const iface::dom::DOMString namespaceURI,
                    const iface::dom::DOMString qualifiedName)
    throw(std::exception&);
  iface::dom::Attr* createAttributeNS(const iface::dom::DOMString namespaceURI,
                                      const iface::dom::DOMString qualifiedName)
    throw(std::exception&);
  iface::dom::NodeList* getElementsByTagNameNS
    (const iface::dom::DOMString namespaceURI,
     const iface::dom::DOMString localName) throw(std::exception&);
  iface::dom::Element* getElementById(const iface::dom::DOMString elementId)
    throw(std::exception&);
  iface::events::Event* createEvent(const iface::dom::DOMString domEventType)
    throw(std::exception&);
  
  GdomeDocument* impl;
  GdomeNode* fetchNode() const;
};

class CDA_Event
  : public iface::events::Event
{
public:
  iface::events::DOMString type() throw(std::exception&);
  u_int16_t eventPhase() throw(std::exception&);
  bool bubbles() throw(std::exception&);
  bool cancelable() throw(std::exception&);
  iface::events::DOMTimeStamp timeStamp() throw(std::exception&);
  void stopPropagation() throw(std::exception&);
  void preventDefault() throw(std::exception&);
  void initEvent(const iface::events::DOMString eventTypeArg,
                 bool canBubbleArg, bool cancelableArg) throw(std::exception&);

  virtual GdomeEvent* fetchEvent() const = 0;
};

class CDA_MutationEvent
  : public iface::events::MutationEvent,
    public CDA_Event
{
public:
  CDA_MutationEvent(GdomeMutationEvent* me);
  virtual ~CDA_MutationEvent();

  CDA_IMPL_REFCOUNT

  iface::events::Node relatedNode() throw(std::exception&);
  iface::events::DOMString prevValue() throw(std::exception&);
  iface::events::DOMString newValue() throw(std::exception&);
  iface::events::DOMString attrName() throw(std::exception&);
  u_int16_t attrChange() throw(std::exception&);
  void initMutationEvent(const iface::events::DOMString typeArg,
                         bool canBubbleArg, bool cancelableArg,
                         iface::events::Node relatedNodeArg,
                         const iface::events::DOMString prevValueArg,
                         const iface::events::DOMString newValueArg,
                         const iface::events::DOMString attrNameArg,
                         u_int16_t attrChangeArg)
    throw(std::exception&);

  GdomeMutationEvent* impl;
  GdomeEvent* fetchEvent() const;
};

iface::dom::Node* CDA_WrapNode(GdomeNode* n);
iface::dom::Element* CDA_WrapElement(GdomeElement* e);
iface::dom::Document* CDA_WrapDocument(GdomeDocument* d);
iface::events::Event* CDA_WrapEvent(GdomeEvent* ev);
