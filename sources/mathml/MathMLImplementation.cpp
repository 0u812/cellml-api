#include "Utilities.hxx"
#include "MathMLImplementation.hxx"

#define MATHML_NS L"http://www.w3.org/1998/Math/MathML"
#define XLINK_NS L"http://www.w3.org/1999/xlink"

iface::mathml_dom::MathMLDocument*
CDA_DOMImplementation::createMathMLDocument()
  throw(std::exception&)
{
  iface::dom::DocumentType* dt =
    createDocumentType(L"math", L"-//W3C//DTD MathML 2.0//EN",
                       L"http://www.w3.org/Math/DTD/mathml2/mathml2.dtd");
  return dynamic_cast<iface::mathml_dom::MathMLDocument*>
    (createDocument(MATHML_NS, L"math", dt));
  dt->release_ref();
}

CDA_MathMLDocument::CDA_MathMLDocument(GdomeDocument* d)
  : CDA_Document(d)
{
}

iface::dom::DOMString
CDA_MathMLDocument::referrer() throw(std::exception&)
{
  return wcsdup(L"");
}

iface::dom::DOMString
CDA_MathMLDocument::domain() throw(std::exception&)
{
  return wcsdup(L"");
}

iface::dom::DOMString
CDA_MathMLDocument::URI() throw(std::exception&)
{
  return wcsdup(L"");
}

static bool
IsQualifier(iface::dom::Node* n)
{
  static wchar_t* qualifiers[] =
    {L"lowlimit", L"uplimit", L"bvar", L"degree", L"logbase", L"interval",
     L"condition", L"domainofapplication", L"momentabout"};
#if 0
  wchar_t* str = n->namespaceURI();
  if (wcscmp(str, MATHML_NS))
  {
    free(str);
    return false;
  }
  free(str);
#endif

  wchar_t* str = n->localName();
  u_int32_t i;
  for (i = 0; i < (sizeof(qualifiers)/sizeof(*qualifiers)); i++)
    if (!wcscmp(qualifiers[i], str))
    {
      free(str);
      return true;
    }
  free(str);
  return false;
}

static bool
IsArgument(iface::dom::Node* n)
{
  wchar_t* str = n->namespaceURI();
  if (wcscmp(str, MATHML_NS))
  {
    free(str);
    return false;
  }
  free(str);

  str = n->localName();
  if (!wcscmp(str, L"declare"))
  {
    free(str);
    return false;
  }
  free(str);

  return !IsQualifier(n);
}

static bool
IsDeclarator(iface::dom::Node* n)
{
  wchar_t* str = n->namespaceURI();
  if (wcscmp(str, MATHML_NS))
  {
    free(str);
    return false;
  }
  free(str);

  str = n->localName();
  if (!wcscmp(str, L"declare"))
  {
    free(str);
    return true;
  }
  free(str);
  return false;
}

static bool
IsRow(iface::dom::Node* n)
{
  wchar_t* str = n->namespaceURI();
  if (wcscmp(str, MATHML_NS))
  {
    free(str);
    return false;
  }
  free(str);

  str = n->localName();
  if (!wcscmp(str, L"row"))
  {
    free(str);
    return true;
  }
  free(str);
  return false;
}

static bool
IsPiece(iface::dom::Node* n)
{
  wchar_t* str = n->namespaceURI();
  if (wcscmp(str, MATHML_NS))
  {
    free(str);
    return false;
  }
  free(str);

  str = n->localName();
  if (!wcscmp(str, L"piece"))
  {
    free(str);
    return true;
  }
  free(str);
  return false;
}

static bool
IsBvar(iface::dom::Node* n)
{
  wchar_t* str = n->namespaceURI();
  if (wcscmp(str, MATHML_NS))
  {
    free(str);
    return false;
  }
  free(str);

  str = n->localName();
  if (!wcscmp(str, L"bvar"))
  {
    free(str);
    return true;
  }
  free(str);
  return false;
}

static bool
IsCondition(iface::dom::Node* n)
{
  wchar_t* str = n->namespaceURI();
  if (wcscmp(str, MATHML_NS))
  {
    free(str);
    return false;
  }
  free(str);

  str = n->localName();
  if (!wcscmp(str, L"condition"))
  {
    free(str);
    return true;
  }
  free(str);
  return false;
}

static bool
IsDegree(iface::dom::Node* n)
{
  wchar_t* str = n->namespaceURI();
  if (wcscmp(str, MATHML_NS))
  {
    free(str);
    return false;
  }
  free(str);

  str = n->localName();
  if (!wcscmp(str, L"degree"))
  {
    free(str);
    return true;
  }
  free(str);
  return false;
}

static bool
IsDomainOfApplication(iface::dom::Node* n)
{
  wchar_t* str = n->namespaceURI();
  if (wcscmp(str, MATHML_NS))
  {
    free(str);
    return false;
  }
  free(str);

  str = n->localName();
  if (!wcscmp(str, L"domainofapplication"))
  {
    free(str);
    return true;
  }
  free(str);
  return false;
}

static bool
IsMomentAbout(iface::dom::Node* n)
{
  wchar_t* str = n->namespaceURI();
  if (wcscmp(str, MATHML_NS))
  {
    free(str);
    return false;
  }
  free(str);

  str = n->localName();
  if (!wcscmp(str, L"momentabout"))
  {
    free(str);
    return true;
  }
  free(str);
  return false;
}

static bool
IsLowLimit(iface::dom::Node* n)
{
  wchar_t* str = n->namespaceURI();
  if (wcscmp(str, MATHML_NS))
  {
    free(str);
    return false;
  }
  free(str);

  str = n->localName();
  if (!wcscmp(str, L"lowlimit"))
  {
    free(str);
    return true;
  }
  free(str);
  return false;
}

static bool
IsUpLimit(iface::dom::Node* n)
{
  wchar_t* str = n->namespaceURI();
  if (wcscmp(str, MATHML_NS))
  {
    free(str);
    return false;
  }
  free(str);

  str = n->localName();
  if (!wcscmp(str, L"uplimit"))
  {
    free(str);
    return true;
  }
  free(str);
  return false;
}

static bool
IsContentElement(iface::dom::Node* n)
{
  return (dynamic_cast<iface::mathml_dom::MathMLContentElement*>(n) != NULL);
}

static bool
IsOtherwise(iface::dom::Node* n)
{
  wchar_t* str = n->namespaceURI();
  if (wcscmp(str, MATHML_NS))
  {
    free(str);
    return false;
  }
  free(str);

  str = n->localName();
  if (!wcscmp(str, L"otherwise"))
  {
    free(str);
    return true;
  }
  free(str);
  return false;
}

static wchar_t*
GetArity(const wchar_t* name)
{
#define AV L"variable"
#define A0 L"0"
#define A1 L"1"
#define A2 L"2"
  struct
    {
      wchar_t* name;
      wchar_t* arity;
    } ArityTable[] =
  {
    {L"abs", A1}, {L"and", AV}, {L"approx", A2}, {L"arccos", A1},
    {L"arccosh", A1}, {L"arccot", A1}, {L"arccoth", A1}, {L"arccsc", A1},
    {L"arccsch", A1}, {L"arcsec", A1}, {L"arcsech", A1}, {L"arcsin", A1},
    {L"arcsinh", A1}, {L"arctan", A1}, {L"arctanh", A1}, {L"arg", A1},
    {L"card", A1}, {L"cartesianproduct", AV}, {L"ceiling", A1},
    {L"codomain", A1}, {L"complexes", A0}, {L"compose", AV},
    {L"conjugate", A1}, {L"cos", A1}, {L"cosh", A1}, {L"cot", A1},
    {L"coth", A1}, {L"csc", A1}, {L"csch", A1}, {L"curl", A1},
    {L"determinant", A1}, {L"diff", A1}, {L"divergence", A1}, {L"divide", A2},
    {L"domain", A1}, {L"emptyset", A0}, {L"eq", AV}, {L"equivalent", A2},
    {L"eulergamma", A0}, {L"exists", A1}, {L"exp", A1}, {L"exponentiale", A0},
    {L"factorial", A1}, {L"factorof", A2}, {L"false", A0}, {L"floor", A1},
    {L"forall", A1}, {L"gcd", AV}, {L"geq", A2}, {L"grad", A1}, {L"gt", A2},
    {L"ident", A1}, {L"image", A1}, {L"imaginary", A1}, {L"imaginaryi", A1},
    {L"implies", A2}, {L"in", A2}, {L"infinity", A0}, {L"int", A1},
    {L"integers", A0}, {L"intersect", AV}, {L"inverse", A1},
    {L"laplacian", A1}, {L"lcm", AV}, {L"leq", A2}, {L"limit", A1},
    {L"ln", A1}, {L"log", A1}, {L"lt", A2}, {L"max", AV}, {L"mean", AV},
    {L"median", AV}, {L"min", AV}, {L"minus", AV}, {L"mode", AV},
    {L"moment", A1}, {L"naturalnumbers", A0}, {L"neq", A2}, {L"not", A1},
    {L"notanumber", A0}, {L"notin", A2}, {L"notprsubset", A2},
    {L"notsubset", A2}, {L"or", AV}, {L"outerproduct", A2},
    {L"partialdiff", A1}, {L"pi", A0}, {L"plus", AV}, {L"power", A2},
    {L"primes", A0}, {L"product", A1}, {L"prsubset", A2}, {L"quotient", A2},
    {L"rationals", A0}, {L"real", A1}, {L"reals", A0}, {L"rem", A2},
    {L"root", A1}, {L"scalarproduct", A2}, {L"sdev", AV}, {L"sec", A1},
    {L"sech", A1}, {L"selector", AV}, {L"setdiff", A2}, {L"sin", A1},
    {L"sinh", A1}, {L"subset", A2}, {L"sum", A1},  {L"tan", A1}, {L"tanh", A1},
    {L"times", AV}, {L"transpose", A1}, {L"true", A0}, {L"union", AV},
    {L"variance", AV}, {L"vectorproduct", A2}, {L"xor", AV}
  };
#undef AV
#undef A0
#undef A1
#undef A2

  u_int32_t min_entry = 0;
  u_int32_t max_entry = sizeof(ArityTable)/sizeof(*ArityTable) - 1;
  while (min_entry <= max_entry)
  {
    u_int32_t selentry = (max_entry + min_entry) / 2;
    int x = wcscmp(name, ArityTable[selentry].name);
    if (x == 0)
    {
      return ArityTable[selentry].arity;
    }
    else if (x > 0)
    {
      if (selentry == sizeof(ArityTable)/sizeof(*ArityTable) - 1)
        break;
      min_entry = selentry + 1;
    }
    else
    {
      if (selentry == 0)
        break;
      max_entry = selentry - 1;
    }
  }
  return L"variable";
}


class CDA_MathMLFilteredNodeList
  : public virtual iface::mathml_dom::MathMLNodeList
{
public:
  const static unsigned int FILTER_ARGUMENT = 0;
  const static unsigned int FILTER_DECLARATOR = 1;
  const static unsigned int FILTER_ROW = 2;
  const static unsigned int FILTER_PIECE = 3;
  const static unsigned int FILTER_BVAR = 4;
  const static unsigned int FILTER_CONTENT = 5;

  CDA_IMPL_REFCOUNT
  CDA_IMPL_QI2(dom::NodeList, mathml_dom::MathMLNodeList)

  CDA_MathMLFilteredNodeList(iface::dom::Node* parent,
                             int ifilterType)
    : _cda_refcount(1), filterType(ifilterType)
  {
    children = parent->childNodes();
    parent->release_ref();
  }

  virtual ~CDA_MathMLFilteredNodeList()
  {
    children->release_ref();
  }

  iface::dom::Node* item(u_int32_t index)
    throw(std::exception&)
  {
    // Count all the nodes...
    u_int32_t pos;
    index++;
    if (index == 0)
      throw iface::dom::DOMException();

    for (pos = 0; index != 0 && (pos < children->length()); pos++)
    {
      iface::dom::Node* n = children->item(pos);
      if (filter(n))
        index--;
    }

    if (index == 0)
      return children->item(pos);

    return NULL;
  }

  u_int32_t length()
    throw(std::exception&)
  {
    // Count all the nodes...
    u_int32_t pos, count = 0;
    for (pos = 0; pos < children->length(); pos++)
    {
      iface::dom::Node* n = children->item(pos);
      if (filter(n))
        count++;
    }
    return count;
  }
private:
  int filterType;
  iface::dom::Node* parent;
  iface::dom::NodeList* children;

  bool
  filter(iface::dom::Node* node)
  {
    switch (filterType)
    {
    case FILTER_ARGUMENT:
      return IsArgument(node);
    case FILTER_DECLARATOR:
      return IsDeclarator(node);
    case FILTER_PIECE:
      return IsPiece(node);
    case FILTER_ROW:
      return IsRow(node);
    case FILTER_BVAR:
      return IsBvar(node);
    case FILTER_CONTENT:
      return IsContentElement(node);
    }
    // This shouldn't happen.
    return false;
  }
};

CDA_MathMLElement::CDA_MathMLElement(GdomeElement* el)
  : CDA_Element(el)
{
}

iface::dom::DOMString
CDA_MathMLElement::className()
  throw(std::exception&)
{
  return static_cast<CDA_Element*>(this)->getAttribute(L"class");
}

void
CDA_MathMLElement::className(iface::dom::DOMString attr)
  throw(std::exception&)
{
  static_cast<CDA_Element*>(this)->setAttribute(L"class", attr);
}

iface::dom::DOMString
CDA_MathMLElement::mathElementStyle()
  throw(std::exception&)
{
  return static_cast<CDA_Element*>(this)->getAttribute(L"style");
}

void
CDA_MathMLElement::mathElementStyle(iface::dom::DOMString attr)
  throw(std::exception&)
{
  static_cast<CDA_Element*>(this)->setAttribute(L"style", attr);
}

iface::dom::DOMString
CDA_MathMLElement::id()
  throw(std::exception&)
{
  return static_cast<CDA_Element*>(this)->getAttribute(L"id");
}

void
CDA_MathMLElement::id(iface::dom::DOMString attr)
  throw(std::exception&)
{
  static_cast<CDA_Element*>(this)->setAttribute(L"id", attr);
}

iface::dom::DOMString
CDA_MathMLElement::xref()
  throw(std::exception&)
{
  return static_cast<CDA_Element*>(this)->getAttributeNS(MATHML_NS, L"xref");
}

void
CDA_MathMLElement::xref(iface::dom::DOMString attr)
  throw(std::exception&)
{
  static_cast<CDA_Element*>(this)->setAttributeNS(MATHML_NS, L"xref",
                                                  attr);
}

iface::dom::DOMString
CDA_MathMLElement::href()
  throw(std::exception&)
{
  return static_cast<CDA_Element*>(this)->getAttributeNS(XLINK_NS, L"href");
}

void
CDA_MathMLElement::href(iface::dom::DOMString attr)
  throw(std::exception&)
{
  static_cast<CDA_Element*>(this)->setAttributeNS(XLINK_NS, L"href",
                                                  attr);
}

iface::mathml_dom::MathMLMathElement*
CDA_MathMLElement::ownerMathElement()
  throw(std::exception&)
{
  iface::dom::Node* n = static_cast<CDA_Element*>(this)->parentNode(), *t;
  while (n != NULL)
  {
    wchar_t* ns = n->namespaceURI();
    bool isMath = !wcscmp(ns, MATHML_NS);
    free(ns);
    if (!isMath)
      t = n->parentNode();
    else
      return dynamic_cast<iface::mathml_dom::MathMLMathElement*>(n);
    n->release_ref();
    n = t;
  }
  return NULL;
}

CDA_MathMLContainer::CDA_MathMLContainer()
{
}

u_int32_t
CDA_MathMLContainer::nArguments()
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_ARGUMENT);
  return mfnl.length();
}

iface::mathml_dom::MathMLNodeList*
CDA_MathMLContainer::arguments()
  throw(std::exception&)
{
  return new CDA_MathMLFilteredNodeList(this,
                                        CDA_MathMLFilteredNodeList::
                                        FILTER_ARGUMENT);
}

iface::mathml_dom::MathMLNodeList*
CDA_MathMLContainer::declarations()
  throw(std::exception&)
{
  return new CDA_MathMLFilteredNodeList(this,
                                        CDA_MathMLFilteredNodeList::
                                        FILTER_DECLARATOR);
}

iface::mathml_dom::MathMLElement*
CDA_MathMLContainer::getArgument(u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_ARGUMENT);
  if (index == 0)
    throw iface::dom::DOMException();
  return dynamic_cast<iface::mathml_dom::MathMLElement*>(mfnl.item(index));
}

iface::mathml_dom::MathMLElement*
CDA_MathMLContainer::setArgument(iface::mathml_dom::MathMLElement* newArgument,
                                 u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_ARGUMENT);

  // XXX this isn't threadsafe, but the DOM provides no atomic approach.
  u_int32_t l = mfnl.length();
  if (index < 1 || index > l + 1)
    throw iface::dom::DOMException();
  if (index == l + 1)
  {
    return dynamic_cast<iface::mathml_dom::MathMLElement*>
      (appendChild(newArgument));
  }

  iface::dom::Node* old = mfnl.item(index - 1);
  iface::mathml_dom::MathMLElement* newA =
    dynamic_cast<iface::mathml_dom::MathMLElement*>(replaceChild(newArgument,
                                                                 old));
  old->release_ref();
  return newA;
}

iface::mathml_dom::MathMLElement*
CDA_MathMLContainer::insertArgument
(
 iface::mathml_dom::MathMLElement* newArgument,
 u_int32_t index
)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_ARGUMENT);

  // XXX this isn't threadsafe, but the DOM provides no atomic approach.
  u_int32_t l = mfnl.length();
  if (index > l + 1)
    throw iface::dom::DOMException();
  if (index == 0 || index == l + 1)
  {
    return dynamic_cast<iface::mathml_dom::MathMLElement*>
      (appendChild(newArgument));
  }

  // Get the child...
  iface::dom::Node* ref = mfnl.item(index - 1);
  iface::mathml_dom::MathMLElement* newA =
    dynamic_cast<iface::mathml_dom::MathMLElement*>
    (insertBefore(newArgument, ref));
  ref->release_ref();
  return newA;
}

void
CDA_MathMLContainer::deleteArgument(u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_ARGUMENT);
  iface::dom::Node* old = mfnl.item(index - 1);
  iface::dom::Node* old2 = removeChild(old);
  old->release_ref();
  old2->release_ref();
}

iface::mathml_dom::MathMLElement*
CDA_MathMLContainer::removeArgument(u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_ARGUMENT);
  iface::dom::Node* old = mfnl.item(index - 1);
  iface::dom::Node* old2 = removeChild(old);
  old->release_ref();
  return dynamic_cast<iface::mathml_dom::MathMLElement*>(old2);
}

iface::mathml_dom::MathMLDeclareElement*
CDA_MathMLContainer::getDeclaration(u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_DECLARATOR);
  if (index == 0)
    throw iface::dom::DOMException();
  return dynamic_cast<iface::mathml_dom::MathMLDeclareElement*>(mfnl.item(index));
}

iface::mathml_dom::MathMLDeclareElement*
CDA_MathMLContainer::setDeclaration
(
 iface::mathml_dom::MathMLDeclareElement* newDeclaration,
 u_int32_t index
)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_DECLARATOR);

  // XXX this isn't threadsafe, but the DOM provides no atomic approach.
  u_int32_t l = mfnl.length();
  if (index < 1 || index > l + 1)
    throw iface::dom::DOMException();
  if (index == l + 1)
  {
    return dynamic_cast<iface::mathml_dom::MathMLDeclareElement*>
      (appendChild(newDeclaration));
  }

  iface::dom::Node* old = mfnl.item(index - 1);
  iface::mathml_dom::MathMLDeclareElement* newA =
    dynamic_cast<iface::mathml_dom::MathMLDeclareElement*>(replaceChild(newDeclaration,
                                                                 old));
  old->release_ref();
  return newA;
}

iface::mathml_dom::MathMLDeclareElement*
CDA_MathMLContainer::insertDeclaration
(
 iface::mathml_dom::MathMLDeclareElement* newDeclaration,
 u_int32_t index
)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_DECLARATOR);

  // XXX this isn't threadsafe, but the DOM provides no atomic approach.
  u_int32_t l = mfnl.length();
  if (index > l + 1)
    throw iface::dom::DOMException();
  if (index == 0 || index == l + 1)
  {
    return dynamic_cast<iface::mathml_dom::MathMLDeclareElement*>
      (appendChild(newDeclaration));
  }

  // Get the child...
  iface::dom::Node* ref = mfnl.item(index - 1);
  iface::mathml_dom::MathMLDeclareElement* newA =
    dynamic_cast<iface::mathml_dom::MathMLDeclareElement*>
    (insertBefore(newDeclaration, ref));
  ref->release_ref();
  return newA;
}

iface::mathml_dom::MathMLDeclareElement*
CDA_MathMLContainer::removeDeclaration(u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_DECLARATOR);
  iface::dom::Node* old = mfnl.item(index - 1);
  iface::dom::Node* old2 = removeChild(old);
  old->release_ref();
  return dynamic_cast<iface::mathml_dom::MathMLDeclareElement*>(old2);
}

void
CDA_MathMLContainer::deleteDeclaration(u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_DECLARATOR);
  iface::dom::Node* old = mfnl.item(index - 1);
  iface::dom::Node* old2 = removeChild(old);
  old->release_ref();
  old2->release_ref();
}

CDA_MathMLMathElement::CDA_MathMLMathElement(GdomeElement* elem)
  : CDA_Element(elem), CDA_MathMLElement(elem)
{
}

iface::dom::DOMString
CDA_MathMLMathElement::macros()
  throw(std::exception&)
{
  return static_cast<iface::mathml_dom::MathMLMathElement*>(this)
    ->getAttributeNS(MATHML_NS, L"macros");
}

void
CDA_MathMLMathElement::macros(iface::dom::DOMString attr)
  throw(std::exception&)
{
  static_cast<iface::mathml_dom::MathMLMathElement*>(this)
    ->setAttributeNS(MATHML_NS, L"macros", attr);
}

iface::dom::DOMString
CDA_MathMLMathElement::display()
  throw(std::exception&)
{
  return static_cast<iface::mathml_dom::MathMLMathElement*>(this)
    ->getAttributeNS(MATHML_NS, L"display");
}

void
CDA_MathMLMathElement::display(iface::dom::DOMString attr)
  throw(std::exception&)
{
  static_cast<iface::mathml_dom::MathMLMathElement*>(this)
    ->setAttributeNS(MATHML_NS, L"display", attr);
}

CDA_MathMLContentElement::CDA_MathMLContentElement(GdomeElement* elem)
  : CDA_Element(elem), CDA_MathMLElement(elem)
{
}

CDA_MathMLContentToken::CDA_MathMLContentToken(GdomeElement* elem)
  : CDA_Element(elem), CDA_MathMLElement(elem), CDA_MathMLContentElement(elem)
{
}

iface::mathml_dom::MathMLNodeList*
CDA_MathMLContentToken::arguments()
  throw(std::exception&)
{
  return new CDA_MathMLFilteredNodeList(this,
                                        CDA_MathMLFilteredNodeList::
                                        FILTER_ARGUMENT);
}

iface::dom::DOMString
CDA_MathMLContentToken::definitionURL()
  throw(std::exception&)
{
  return static_cast<iface::mathml_dom::MathMLContentElement*>(this)
    ->getAttributeNS(MATHML_NS, L"definitionURL");
}

void
CDA_MathMLContentToken::definitionURL
(iface::dom::DOMString attr)
  throw(std::exception&)
{
  static_cast<iface::mathml_dom::MathMLContentElement*>(this)
    ->setAttributeNS(MATHML_NS, L"definitionURL", attr);
}

iface::dom::DOMString
CDA_MathMLContentToken::encoding()
  throw(std::exception&)
{
  return static_cast<iface::mathml_dom::MathMLContentElement*>(this)
    ->getAttributeNS(MATHML_NS, L"encoding");
}

void
CDA_MathMLContentToken::encoding(iface::dom::DOMString attr)
  throw(std::exception&)
{
  static_cast<iface::mathml_dom::MathMLContentElement*>(this)
    ->setAttributeNS(MATHML_NS, L"encoding", attr);
}

iface::dom::Node*
CDA_MathMLContentToken::getArgument(u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_ARGUMENT);
  if (index == 0)
    throw iface::dom::DOMException();
  return mfnl.item(index);
}

iface::dom::Node*
CDA_MathMLContentToken::insertArgument(iface::dom::Node* newArgument, u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_ARGUMENT);

  // XXX this isn't threadsafe, but the DOM provides no atomic approach.
  u_int32_t l = mfnl.length();
  if (index > l + 1)
    throw iface::dom::DOMException();
  if (index == 0 || index == l + 1)
  {
    return appendChild(newArgument);
  }

  // Get the child...
  iface::dom::Node* ref = mfnl.item(index - 1);
  iface::dom::Node* newA = insertBefore(newArgument, ref);
  ref->release_ref();
  return newA;
}

iface::dom::Node*
CDA_MathMLContentToken::setArgument(iface::dom::Node* newArgument, u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_ARGUMENT);

  // XXX this isn't threadsafe, but the DOM provides no atomic approach.
  u_int32_t l = mfnl.length();
  if (index < 1 || index > l + 1)
    throw iface::dom::DOMException();
  if (index == l + 1)
  {
    return appendChild(newArgument);
  }

  iface::dom::Node* old = mfnl.item(index - 1);
  iface::dom::Node* newA =
    replaceChild(newArgument, old);
  old->release_ref();
  return newA;
}

void
CDA_MathMLContentToken::deleteArgument(u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_ARGUMENT);
  iface::dom::Node* old = mfnl.item(index - 1);
  iface::dom::Node* old2 = removeChild(old);
  old->release_ref();
  old2->release_ref();
}

iface::dom::Node*
CDA_MathMLContentToken::removeArgument(u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_ARGUMENT);
  iface::dom::Node* old = mfnl.item(index - 1);
  iface::dom::Node* old2 = removeChild(old);
  old->release_ref();
  return old2;
}

CDA_MathMLCnElement::CDA_MathMLCnElement(GdomeElement* el)
  : CDA_Element(el), CDA_MathMLElement(el), CDA_MathMLContentElement(el),
    CDA_MathMLContentToken(el)
{
}

iface::dom::DOMString
CDA_MathMLCnElement::type()
  throw(std::exception&)
{
  return static_cast<iface::mathml_dom::MathMLContentElement*>(this)
    ->getAttributeNS(MATHML_NS, L"type");
}

void
CDA_MathMLCnElement::type(iface::dom::DOMString attr)
  throw(std::exception&)
{
  static_cast<iface::mathml_dom::MathMLContentElement*>(this)
    ->setAttributeNS(MATHML_NS, L"type", attr);
}

iface::dom::DOMString
CDA_MathMLCnElement::base()
  throw(std::exception&)
{
  return static_cast<iface::mathml_dom::MathMLContentElement*>(this)
    ->getAttributeNS(MATHML_NS, L"type");
}

void
CDA_MathMLCnElement::base(iface::dom::DOMString attr)
  throw(std::exception&)
{
  static_cast<iface::mathml_dom::MathMLContentElement*>(this)
    ->setAttributeNS(MATHML_NS, L"base", attr);
}

u_int32_t
CDA_MathMLCnElement::nargs()
  throw(std::exception&)
{
  // We count seps and add one...
  u_int32_t count = 1;
  iface::dom::NodeList* nl = this->childNodes();
  u_int32_t i, l = nl->length();
  for (i = 0; i < l; i++)
  {
    iface::dom::Node* n = nl->item(i);
    wchar_t* str = n->namespaceURI();
    if (!wcscmp(str, MATHML_NS))
    {
      free(str);
      str = n->localName();
      if (!wcscmp(str, L"sep"))
        count++;
    }
    free(str);
    n->release_ref();
  }
  nl->release_ref();

  return count;
}

CDA_MathMLCiElement::CDA_MathMLCiElement(GdomeElement* elem)
  : CDA_Element(elem), CDA_MathMLElement(elem), CDA_MathMLContentElement(elem),
    CDA_MathMLContentToken(elem)
{
}

iface::dom::DOMString
CDA_MathMLCiElement::type()
  throw(std::exception&)
{
  return static_cast<iface::mathml_dom::MathMLContentElement*>(this)
    ->getAttributeNS(MATHML_NS, L"type");
}

void
CDA_MathMLCiElement::type(iface::dom::DOMString attr)
  throw(std::exception&)
{
  static_cast<iface::mathml_dom::MathMLContentElement*>(this)
    ->setAttributeNS(MATHML_NS, L"type", attr);
}

CDA_MathMLCsymbolElement::CDA_MathMLCsymbolElement(GdomeElement* elem)
  : CDA_Element(elem), CDA_MathMLElement(elem), CDA_MathMLContentElement(elem),
    CDA_MathMLContentToken(elem)
{
}

CDA_MathMLContentContainer::CDA_MathMLContentContainer(GdomeElement* elem)
  : CDA_Element(elem), CDA_MathMLElement(elem), CDA_MathMLContentElement(elem)
{
}

u_int32_t
CDA_MathMLContentContainer::nBoundVariables()
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_BVAR);
  return mfnl.length();
}

iface::mathml_dom::MathMLConditionElement*
CDA_MathMLContentContainer::condition()
  throw(std::exception&)
{
  // See if we can find a condition...
  iface::dom::NodeList* nl = childNodes();
  iface::dom::Node* n;
  u_int32_t i, l = nl->length();
  for (i = 0; i < l; i++)
  {
    n = nl->item(i);
    if (IsCondition(n))
    {
      nl->release_ref();
      return dynamic_cast<iface::mathml_dom::MathMLConditionElement*>(n);
    }
    n->release_ref();
  }
  nl->release_ref();

  // There is no condition yet. Make one...
  iface::dom::Document* dd = ownerDocument();
  n = dd->createElementNS(MATHML_NS, L"condition");
  iface::dom::Node* n2 = appendChild(n);
  n2->release_ref();
  dd->release_ref();
  return dynamic_cast<iface::mathml_dom::MathMLConditionElement*>(n);
}

void
CDA_MathMLContentContainer::condition(iface::mathml_dom::MathMLConditionElement* attr)
  throw(std::exception&)
{
  // See if we can find a condition...
  iface::dom::NodeList* nl = childNodes();
  iface::dom::Node* n;
  u_int32_t i, l = nl->length();
  for (i = 0; i < l; i++)
  {
    n = nl->item(i);
    if (IsCondition(n))
    {
      nl->release_ref();
      // We have a condition. Replace it.
      iface::dom::Node* n2 = replaceChild(attr, n);
      n2->release_ref();
      n->release_ref();
      return;
    }
    n->release_ref();
  }
  nl->release_ref();

  // There is no condition yet. Append this one...
  n = appendChild(attr);
  n->release_ref();
}

iface::mathml_dom::MathMLElement*
CDA_MathMLContentContainer::opDegree()
  throw(std::exception&)
{
  // See if we can find a degree...
  iface::dom::NodeList* nl = childNodes();
  iface::dom::Node* n;
  u_int32_t i, l = nl->length();
  for (i = 0; i < l; i++)
  {
    n = nl->item(i);
    if (IsDegree(n))
    {
      nl->release_ref();
      return dynamic_cast<iface::mathml_dom::MathMLElement*>(n);
    }
    n->release_ref();
  }
  nl->release_ref();

  // There is no degree yet. Make one...
  iface::dom::Document* dd = ownerDocument();
  n = dd->createElementNS(MATHML_NS, L"degree");
  iface::dom::Node* n2 = appendChild(n);
  n2->release_ref();
  dd->release_ref();
  return dynamic_cast<iface::mathml_dom::MathMLConditionElement*>(n);
}

void
CDA_MathMLContentContainer::opDegree(iface::mathml_dom::MathMLElement* attr)
  throw(std::exception&)
{
  // See if we can find a degree...
  iface::dom::NodeList* nl = childNodes();
  iface::dom::Node* n;
  u_int32_t i, l = nl->length();
  for (i = 0; i < l; i++)
  {
    n = nl->item(i);
    if (IsDegree(n))
    {
      nl->release_ref();
      // We have a degree. Replace it.
      iface::dom::Node* n2 = replaceChild(attr, n);
      n2->release_ref();
      n->release_ref();
      return;
    }
    n->release_ref();
  }
  nl->release_ref();

  // There is no degree yet. Append this one...
  n = appendChild(attr);
  n->release_ref();
}

iface::mathml_dom::MathMLElement*
CDA_MathMLContentContainer::domainOfApplication()
  throw(std::exception&)
{
  // See if we can find a domainofapplication...
  iface::dom::NodeList* nl = childNodes();
  iface::dom::Node* n;
  u_int32_t i, l = nl->length();
  for (i = 0; i < l; i++)
  {
    n = nl->item(i);
    if (IsDomainOfApplication(n))
    {
      nl->release_ref();
      return dynamic_cast<iface::mathml_dom::MathMLElement*>(n);
    }
    n->release_ref();
  }
  nl->release_ref();

  // There is no domainofapplication yet. Make one...
  iface::dom::Document* dd = ownerDocument();
  n = dd->createElementNS(MATHML_NS, L"domainofapplication");
  iface::dom::Node* n2 = appendChild(n);
  n2->release_ref();
  dd->release_ref();
  return dynamic_cast<iface::mathml_dom::MathMLElement*>(n);
}

void
CDA_MathMLContentContainer::domainOfApplication(iface::mathml_dom::MathMLElement* attr)
  throw(std::exception&)
{
  // See if we can find a degree...
  iface::dom::NodeList* nl = childNodes();
  iface::dom::Node* n;
  u_int32_t i, l = nl->length();
  for (i = 0; i < l; i++)
  {
    n = nl->item(i);
    if (IsDomainOfApplication(n))
    {
      nl->release_ref();
      // We have a domainofapplication. Replace it.
      iface::dom::Node* n2 = replaceChild(attr, n);
      n2->release_ref();
      n->release_ref();
      return;
    }
    n->release_ref();
  }
  nl->release_ref();

  // There is no domainofapplication yet. Append this one...
  n = appendChild(attr);
  n->release_ref();
}

iface::mathml_dom::MathMLElement*
CDA_MathMLContentContainer::momentAbout()
  throw(std::exception&)
{
  // See if we can find a degree...
  iface::dom::NodeList* nl = childNodes();
  iface::dom::Node* n;
  u_int32_t i, l = nl->length();
  for (i = 0; i < l; i++)
  {
    n = nl->item(i);
    if (IsMomentAbout(n))
    {
      nl->release_ref();
      return dynamic_cast<iface::mathml_dom::MathMLElement*>(n);
    }
    n->release_ref();
  }
  nl->release_ref();

  // There is no degree yet. Make one...
  iface::dom::Document* dd = ownerDocument();
  n = dd->createElementNS(MATHML_NS, L"momentabout");
  iface::dom::Node* n2 = appendChild(n);
  n2->release_ref();
  dd->release_ref();
  return dynamic_cast<iface::mathml_dom::MathMLElement*>(n);
}

void
CDA_MathMLContentContainer::momentAbout(iface::mathml_dom::MathMLElement* attr)
  throw(std::exception&)
{
  // See if we can find a degree...
  iface::dom::NodeList* nl = childNodes();
  iface::dom::Node* n;
  u_int32_t i, l = nl->length();
  for (i = 0; i < l; i++)
  {
    n = nl->item(i);
    if (IsMomentAbout(n))
    {
      nl->release_ref();
      // We have a domainofapplication. Replace it.
      iface::dom::Node* n2 = replaceChild(attr, n);
      n2->release_ref();
      n->release_ref();
      return;
    }
    n->release_ref();
  }
  nl->release_ref();

  // There is no domainofapplication yet. Append this one...
  n = appendChild(attr);
  n->release_ref();
}

iface::mathml_dom::MathMLBvarElement*
CDA_MathMLContentContainer::getBoundVariable(u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_BVAR);
  if (index == 0)
    throw iface::dom::DOMException();
  return dynamic_cast<iface::mathml_dom::MathMLBvarElement*>(mfnl.item(index));
}

iface::mathml_dom::MathMLBvarElement*
CDA_MathMLContentContainer::insertBoundVariable(iface::mathml_dom::MathMLBvarElement* newBVar, u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_BVAR);

  // XXX this isn't threadsafe, but the DOM provides no atomic approach.
  u_int32_t l = mfnl.length();
  if (index > l + 1)
    throw iface::dom::DOMException();
  if (index == 0 || index == l + 1)
  {
    return dynamic_cast<iface::mathml_dom::MathMLBvarElement*>
      (appendChild(newBVar));
  }

  // Get the child...
  iface::dom::Node* ref = mfnl.item(index - 1);
  iface::mathml_dom::MathMLBvarElement* newA =
    dynamic_cast<iface::mathml_dom::MathMLBvarElement*>
    (insertBefore(newBVar, ref));
  ref->release_ref();
  return newA;
}

iface::mathml_dom::MathMLBvarElement*
CDA_MathMLContentContainer::setBoundVariable(iface::mathml_dom::MathMLBvarElement* newBVar, u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_BVAR);

  // XXX this isn't threadsafe, but the DOM provides no atomic approach.
  u_int32_t l = mfnl.length();
  if (index < 1 || index > l + 1)
    throw iface::dom::DOMException();
  if (index == l + 1)
  {
    return dynamic_cast<iface::mathml_dom::MathMLBvarElement*>
      (appendChild(newBVar));
  }

  iface::dom::Node* old = mfnl.item(index - 1);
  iface::mathml_dom::MathMLBvarElement* newA =
    dynamic_cast<iface::mathml_dom::MathMLBvarElement*>(replaceChild(newBVar,
                                                                     old));
  old->release_ref();
  return newA;
}

void
CDA_MathMLContentContainer::deleteBoundVariable(u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_BVAR);
  iface::dom::Node* old = mfnl.item(index - 1);
  iface::dom::Node* old2 = removeChild(old);
  old->release_ref();
  old2->release_ref();
}

iface::mathml_dom::MathMLBvarElement*
CDA_MathMLContentContainer::removeBoundVariable(u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_BVAR);
  iface::dom::Node* old = mfnl.item(index - 1);
  iface::dom::Node* old2 = removeChild(old);
  old->release_ref();
  return dynamic_cast<iface::mathml_dom::MathMLBvarElement*>(old2);
}

CDA_MathMLApplyElement::CDA_MathMLApplyElement(GdomeElement* el)
  : CDA_Element(el), CDA_MathMLElement(el),  CDA_MathMLContentElement(el),
    CDA_MathMLContentContainer(el)
{
}

iface::mathml_dom::MathMLElement*
CDA_MathMLApplyElement::_cxx_operator()
  throw(std::exception&)
{
  // If there is a first child, it is the operator...
  iface::dom::NodeList* nl = childNodes();
  u_int32_t i, l = nl->length();
  for (i = 0; i < l; i++)
  {
    iface::dom::Node* n = nl->item(i);
    wchar_t* str = n->namespaceURI();
    bool isMathML = !wcscmp(str, MATHML_NS);
    free(str);
    // Don't count lowlimit & uplimit as operators...
    if (isMathML && (IsLowLimit(n) || IsUpLimit(n)))
      isMathML = false;
    if (isMathML)
    {
      nl->release_ref();
      return dynamic_cast<iface::mathml_dom::MathMLElement*>(n);
    }
    n->release_ref();
  }
  nl->release_ref();
  // No operator defined, and we can't create one because we don't know what
  // type.
  throw iface::dom::DOMException();
}

void
CDA_MathMLApplyElement::_cxx_operator(iface::mathml_dom::MathMLElement* attr)
  throw(std::exception&)
{
  // If there is a first child, it is the operator...
  iface::dom::NodeList* nl = childNodes();
  u_int32_t i, l = nl->length();
  for (i = 0; i < l; i++)
  {
    iface::dom::Node* n = nl->item(i);
    wchar_t* str = n->namespaceURI();
    bool isMathML = !wcscmp(str, MATHML_NS);
    free(str);
    // Don't count lowlimit & uplimit as operators...
    if (isMathML && (IsLowLimit(n) || IsUpLimit(n)))
      isMathML = false;
    if (isMathML)
    {
      nl->release_ref();
      iface::dom::Node* n2 = replaceChild(attr, n);
      n->release_ref();
      n2->release_ref();
      return;
    }
    n->release_ref();
  }
  nl->release_ref();
  // We can now append the operator...
  iface::dom::Node* n = appendChild(attr);
  n->release_ref();
}

iface::mathml_dom::MathMLElement*
CDA_MathMLApplyElement::lowLimit()
  throw(std::exception&)
{
  // See if we can find a lowlimit...
  iface::dom::NodeList* nl = childNodes();
  iface::dom::Node* n;
  u_int32_t i, l = nl->length();
  for (i = 0; i < l; i++)
  {
    n = nl->item(i);
    if (IsLowLimit(n))
    {
      nl->release_ref();
      return dynamic_cast<iface::mathml_dom::MathMLElement*>(n);
    }
    n->release_ref();
  }
  nl->release_ref();

  // There is no lowlimit yet. Make one...
  iface::dom::Document* dd = ownerDocument();
  n = dd->createElementNS(MATHML_NS, L"lowlimit");
  iface::dom::Node* n2 = appendChild(n);
  n2->release_ref();
  dd->release_ref();
  return dynamic_cast<iface::mathml_dom::MathMLElement*>(n);
}

void
CDA_MathMLApplyElement::lowLimit(iface::mathml_dom::MathMLElement* attr)
  throw(std::exception&)
{
  // See if we can find a lowlimit...
  iface::dom::NodeList* nl = childNodes();
  iface::dom::Node* n;
  u_int32_t i, l = nl->length();
  for (i = 0; i < l; i++)
  {
    n = nl->item(i);
    if (IsLowLimit(n))
    {
      nl->release_ref();
      // We have a lowlimit. Replace it.
      iface::dom::Node* n2 = replaceChild(attr, n);
      n2->release_ref();
      n->release_ref();
      return;
    }
    n->release_ref();
  }
  nl->release_ref();

  // There is no lowlimit yet. Append this one...
  n = appendChild(attr);
  n->release_ref();
}

iface::mathml_dom::MathMLElement*
CDA_MathMLApplyElement::upLimit()
  throw(std::exception&)
{
  // See if we can find a uplimit...
  iface::dom::NodeList* nl = childNodes();
  iface::dom::Node* n;
  u_int32_t i, l = nl->length();
  for (i = 0; i < l; i++)
  {
    n = nl->item(i);
    if (IsUpLimit(n))
    {
      nl->release_ref();
      return dynamic_cast<iface::mathml_dom::MathMLElement*>(n);
    }
    n->release_ref();
  }
  nl->release_ref();

  // There is no lowlimit yet. Make one...
  iface::dom::Document* dd = ownerDocument();
  n = dd->createElementNS(MATHML_NS, L"lowlimit");
  iface::dom::Node* n2 = appendChild(n);
  n2->release_ref();
  dd->release_ref();
  return dynamic_cast<iface::mathml_dom::MathMLElement*>(n);
}

void
CDA_MathMLApplyElement::upLimit(iface::mathml_dom::MathMLElement* attr)
  throw(std::exception&)
{
  // See if we can find a lowlimit...
  iface::dom::NodeList* nl = childNodes();
  iface::dom::Node* n;
  u_int32_t i, l = nl->length();
  for (i = 0; i < l; i++)
  {
    n = nl->item(i);
    if (IsUpLimit(n))
    {
      nl->release_ref();
      // We have a lowlimit. Replace it.
      iface::dom::Node* n2 = replaceChild(attr, n);
      n2->release_ref();
      n->release_ref();
      return;
    }
    n->release_ref();
  }
  nl->release_ref();

  // There is no uplimit yet. Append this one...
  n = appendChild(attr);
  n->release_ref();
}

CDA_MathMLFnElement::CDA_MathMLFnElement(GdomeElement* el)
  : CDA_Element(el), CDA_MathMLElement(el),  CDA_MathMLContentElement(el),
    CDA_MathMLContentContainer(el)
{
}

iface::dom::DOMString
CDA_MathMLFnElement::definitionURL()
  throw(std::exception&)
{
  return static_cast<CDA_Element*>(this)
    ->getAttributeNS(MATHML_NS, L"definitionURL");
}

void
CDA_MathMLFnElement::definitionURL(iface::dom::DOMString attr)
  throw(std::exception&)
{
  static_cast<CDA_Element*>(this)
    ->setAttributeNS(MATHML_NS, L"definitionURL", attr);
}

iface::dom::DOMString
CDA_MathMLFnElement::encoding()
  throw(std::exception&)
{
  return static_cast<CDA_Element*>(this)
    ->getAttributeNS(MATHML_NS, L"encoding");
}

void
CDA_MathMLFnElement::encoding(iface::dom::DOMString attr)
  throw(std::exception&)
{
  static_cast<CDA_Element*>(this)
    ->setAttributeNS(MATHML_NS, L"encoding", attr);
}

CDA_MathMLLambdaElement::CDA_MathMLLambdaElement(GdomeElement* el)
  : CDA_Element(el), CDA_MathMLElement(el),  CDA_MathMLContentElement(el),
    CDA_MathMLContentContainer(el)
{
}

iface::mathml_dom::MathMLElement*
CDA_MathMLLambdaElement::expression()
  throw(std::exception&)
{
  return getArgument(1);
}

void
CDA_MathMLLambdaElement::expression(iface::mathml_dom::MathMLElement* attr)
  throw(std::exception&)
{
  setArgument(attr, 1);
}

CDA_MathMLSetElement::CDA_MathMLSetElement(GdomeElement* el)
  : CDA_Element(el), CDA_MathMLElement(el),  CDA_MathMLContentElement(el),
    CDA_MathMLContentContainer(el)
{
}

bool
CDA_MathMLSetElement::isExplicit()
  throw(std::exception&)
{
  // If it has a bvar it is not explicit, otherwise it is...
  return (nBoundVariables() == 0);
}

iface::dom::DOMString
CDA_MathMLSetElement::type()
  throw(std::exception&)
{
  return static_cast<CDA_Element*>(this)
    ->getAttributeNS(MATHML_NS, L"type");
}

void
CDA_MathMLSetElement::type(iface::dom::DOMString attr)
  throw(std::exception&)
{
  static_cast<CDA_Element*>(this)
    ->setAttributeNS(MATHML_NS, L"type", attr);
}

CDA_MathMLListElement::CDA_MathMLListElement(GdomeElement* el)
  : CDA_Element(el), CDA_MathMLElement(el),  CDA_MathMLContentElement(el),
    CDA_MathMLContentContainer(el)
{
}

bool
CDA_MathMLListElement::isExplicit()
  throw(std::exception&)
{
  // If it has a bvar it is not explicit, otherwise it is...
  return (nBoundVariables() == 0);
}

iface::dom::DOMString
CDA_MathMLListElement::ordering()
  throw(std::exception&)
{
  return static_cast<CDA_Element*>(this)
    ->getAttributeNS(MATHML_NS, L"order");
}

void
CDA_MathMLListElement::ordering(iface::dom::DOMString attr)
  throw(std::exception&)
{
  static_cast<CDA_Element*>(this)
    ->setAttributeNS(MATHML_NS, L"order", attr);
}

CDA_MathMLBvarElement::CDA_MathMLBvarElement(GdomeElement* el)
  : CDA_Element(el), CDA_MathMLElement(el),  CDA_MathMLContentElement(el),
    CDA_MathMLContentContainer(el)
{
}

CDA_MathMLPredefinedSymbol::CDA_MathMLPredefinedSymbol(GdomeElement* el)
  : CDA_Element(el), CDA_MathMLElement(el),  CDA_MathMLContentElement(el)
{
}

iface::dom::DOMString
CDA_MathMLPredefinedSymbol::definitionURL()
  throw(std::exception&)
{
  return static_cast<CDA_Element*>(this)
    ->getAttributeNS(MATHML_NS, L"definitionURL");
}

void
CDA_MathMLPredefinedSymbol::definitionURL(iface::dom::DOMString attr)
  throw(std::exception&)
{
  static_cast<CDA_Element*>(this)
    ->setAttributeNS(MATHML_NS, L"definitionURL", attr);
}

iface::dom::DOMString
CDA_MathMLPredefinedSymbol::encoding()
  throw(std::exception&)
{
  return static_cast<CDA_Element*>(this)
    ->getAttributeNS(MATHML_NS, L"encoding");
}

void 
CDA_MathMLPredefinedSymbol::encoding(iface::dom::DOMString attr)
  throw(std::exception&)
{
  static_cast<CDA_Element*>(this)
    ->setAttributeNS(MATHML_NS, L"encoding", attr);
}

iface::dom::DOMString
CDA_MathMLPredefinedSymbol::arity()
  throw(std::exception&)
{
  wchar_t* name = static_cast<CDA_Element*>(this)->tagName();
  iface::dom::DOMString ret = wcsdup(GetArity(name));
  free(name);
  return ret;
}

iface::dom::DOMString
CDA_MathMLPredefinedSymbol::symbolName()
  throw(std::exception&)
{
  return static_cast<CDA_Element*>(this)->tagName();
}

CDA_MathMLTendsToElement::CDA_MathMLTendsToElement(GdomeElement* el)
  : CDA_Element(el), CDA_MathMLElement(el),  CDA_MathMLContentElement(el),
    CDA_MathMLPredefinedSymbol(el)
{
}

iface::dom::DOMString
CDA_MathMLTendsToElement::type()
  throw(std::exception&)
{
  try
  {
    return
      static_cast<CDA_Element*>(this)
      ->getAttributeNS(MATHML_NS, L"type");
  }
  catch (iface::dom::DOMException& de)
  {
    return wcsdup(L"two-sided");
  }
}

void
CDA_MathMLTendsToElement::type(iface::dom::DOMString attr)
  throw(std::exception&)
{
  static_cast<CDA_Element*>(this)
    ->setAttributeNS(MATHML_NS, L"type", attr);
}

CDA_MathMLIntervalElement::CDA_MathMLIntervalElement(GdomeElement* el)
  : CDA_Element(el), CDA_MathMLElement(el),  CDA_MathMLContentElement(el)
{
}

iface::dom::DOMString
CDA_MathMLIntervalElement::closure()
  throw(std::exception&)
{
  try
  {
    return
      static_cast<CDA_Element*>(this)
      ->getAttributeNS(MATHML_NS, L"closure");
  }
  catch (iface::dom::DOMException& de)
  {
    return wcsdup(L"closed");
  }
}

void
CDA_MathMLIntervalElement::closure(iface::dom::DOMString attr)
  throw(std::exception&)
{
  static_cast<CDA_Element*>(this)
    ->setAttributeNS(MATHML_NS, L"closure", attr);
}

iface::mathml_dom::MathMLContentElement*
CDA_MathMLIntervalElement::start()
  throw(std::exception&)
{
  iface::dom::NodeList *cn = static_cast<CDA_Element*>(this)->childNodes();
  try
  {
    u_int32_t l = cn->length();
    u_int32_t i = 0;
    for (i = 0; i < l; i++)
    {
      iface::dom::Node* n = cn->item(i);
      iface::mathml_dom::MathMLContentElement* ce =
        dynamic_cast<iface::mathml_dom::MathMLContentElement*>(n);
      if (ce == NULL)
      {
        n->release_ref();
        continue;
      }
      else
      {
        cn->release_ref();
        return ce;
      }
    }
  }
  catch (std::exception& e)
  {
    cn->release_ref();
    throw e;
  }
  cn->release_ref();
  throw iface::dom::DOMException();
}

void
CDA_MathMLIntervalElement::start(iface::mathml_dom::MathMLContentElement* attr)
  throw(std::exception&)
{
  iface::dom::NodeList *cn = static_cast<CDA_Element*>(this)->childNodes();
  u_int32_t l = cn->length();
  iface::mathml_dom::MathMLContentElement* oldStart = NULL;
  iface::mathml_dom::MathMLContentElement* oldEnd = NULL;
  u_int32_t i = 0;
  for (i = 0; i < l; i++)
  {
    iface::dom::Node* n = cn->item(i);
    iface::mathml_dom::MathMLContentElement* ce =
      dynamic_cast<iface::mathml_dom::MathMLContentElement*>(n);
    if (ce != NULL)
    {
      if (oldStart)
      {
        if (oldEnd)
          break;
        oldEnd = ce;
      }
      else
        oldStart = ce;
    }
    else
      n->release_ref();
  }
  cn->release_ref();

  if (!oldStart)
  {
    appendChild(attr)->release_ref();
    iface::dom::Node* n = attr->cloneNode(true);
    appendChild(n)->release_ref();
    n->release_ref();
    return;
  }
  
  replaceChild(attr, oldStart)->release_ref();
  if (!oldEnd)
  {
    iface::dom::Node* n = attr->cloneNode(true);
    appendChild(n)->release_ref();
    n->release_ref();
  }
  else
    oldEnd->release_ref();
  oldStart->release_ref();
}

iface::mathml_dom::MathMLContentElement*
CDA_MathMLIntervalElement::end()
  throw(std::exception&)
{
  iface::dom::NodeList *cn = static_cast<CDA_Element*>(this)->childNodes();
  try
  {
    u_int32_t l = cn->length();
    u_int32_t i = 0;
    iface::mathml_dom::MathMLContentElement* firstCandidate = NULL;
    for (i = 0; i < l; i++)
    {
      iface::dom::Node* n = cn->item(i);
      iface::mathml_dom::MathMLContentElement* ce =
        dynamic_cast<iface::mathml_dom::MathMLContentElement*>(n);
      if (ce == NULL)
      {
        n->release_ref();
        continue;
      }
      else if (firstCandidate != NULL)
      {
        cn->release_ref();
        return ce;
      }
      else
        firstCandidate = ce;
    }
    cn->release_ref();
    cn = NULL;
    if (firstCandidate != NULL)
      return firstCandidate;
    throw iface::dom::DOMException();
  }
  catch (std::exception& e)
  {
    if (cn != NULL)
      cn->release_ref();
    throw e;
  }
}

void
CDA_MathMLIntervalElement::end(iface::mathml_dom::MathMLContentElement* attr)
  throw(std::exception&)
{
  iface::dom::NodeList *cn = static_cast<CDA_Element*>(this)->childNodes();
  u_int32_t l = cn->length();
  iface::mathml_dom::MathMLContentElement* oldStart = NULL;
  iface::mathml_dom::MathMLContentElement* oldEnd = NULL;
  u_int32_t i = 0;
  for (i = 0; i < l; i++)
  {
    iface::dom::Node* n = cn->item(i);
    iface::mathml_dom::MathMLContentElement* ce =
      dynamic_cast<iface::mathml_dom::MathMLContentElement*>(n);
    if (ce != NULL)
    {
      if (oldStart)
      {
        if (oldEnd)
          break;
        oldEnd = ce;
      }
      else
        oldStart = ce;
    }
    else
      n->release_ref();
  }
  cn->release_ref();

  if (!oldStart)
  {
    iface::dom::Node* n = attr->cloneNode(true);
    appendChild(n)->release_ref();
    n->release_ref();
    appendChild(attr)->release_ref();
    return;
  }

  if (!oldEnd)
  {
    appendChild(attr)->release_ref();
  }
  else
  {
    replaceChild(attr, oldEnd)->release_ref();
    oldEnd->release_ref();
  }
  oldStart->release_ref();
}

CDA_MathMLConditionElement::CDA_MathMLConditionElement(GdomeElement* el)
  : CDA_Element(el), CDA_MathMLElement(el),  CDA_MathMLContentElement(el)
{
}

iface::mathml_dom::MathMLApplyElement*
CDA_MathMLConditionElement::condition()
  throw(std::exception&)
{
  iface::dom::NodeList *cn = static_cast<CDA_Element*>(this)->childNodes();
  try
  {
    u_int32_t l = cn->length();
    u_int32_t i = 0;
    for (i = 0; i < l; i++)
    {
      iface::dom::Node* n = cn->item(i);
      iface::mathml_dom::MathMLApplyElement* ce =
        dynamic_cast<iface::mathml_dom::MathMLApplyElement*>(n);
      if (ce == NULL)
      {
        n->release_ref();
        continue;
      }
      else
      {
        cn->release_ref();
        return ce;
      }
    }
  }
  catch (std::exception& e)
  {
    cn->release_ref();
    throw e;
  }
  cn->release_ref();
  throw iface::dom::DOMException();
}

void
CDA_MathMLConditionElement::condition(iface::mathml_dom::MathMLApplyElement* attr)
  throw(std::exception&)
{
  iface::dom::NodeList *cn = static_cast<CDA_Element*>(this)->childNodes();
  try
  {
    u_int32_t l = cn->length();
    u_int32_t i = 0;
    for (i = 0; i < l; i++)
    {
      iface::dom::Node* n = cn->item(i);
      iface::mathml_dom::MathMLApplyElement* ce =
        dynamic_cast<iface::mathml_dom::MathMLApplyElement*>(n);
      if (ce == NULL)
      {
        n->release_ref();
        continue;
      }
      else
      {
        cn->release_ref();
        replaceChild(attr, n)->release_ref();
        return;
      }
    }
  }
  catch (std::exception& e)
  {
    cn->release_ref();
    throw e;
  }
  cn->release_ref();
  appendChild(attr)->release_ref();
}

CDA_MathMLDeclareElement::CDA_MathMLDeclareElement(GdomeElement* el)
  : CDA_Element(el), CDA_MathMLElement(el),  CDA_MathMLContentElement(el)
{
}

iface::dom::DOMString
CDA_MathMLDeclareElement::type()
  throw(std::exception&)
{
  // XXX need to infer the type from the constructor if no attribute present.
  return
    static_cast<CDA_Element*>(this)
    ->getAttributeNS(MATHML_NS, L"type");
}

void
CDA_MathMLDeclareElement::type(iface::dom::DOMString attr)
  throw(std::exception&)
{
  return
    static_cast<CDA_Element*>(this)
    ->setAttributeNS(MATHML_NS, L"type", attr);
}

u_int32_t
CDA_MathMLDeclareElement::nargs()
  throw(std::exception&)
{
  iface::dom::DOMString ds = static_cast<CDA_Element*>(this)->getAttributeNS
                             (MATHML_NS, L"nargs");
  u_int32_t ret = wcstoul(ds, NULL, 10);
  free(ds);
  return ret;
}

void
CDA_MathMLDeclareElement::nargs(u_int32_t attr)
  throw(std::exception&)
{
  wchar_t str[20];
  swprintf(str, 20, L"%u", attr);
  static_cast<CDA_Element*>(this)->setAttributeNS(MATHML_NS, L"nargs", str);
}

iface::dom::DOMString
CDA_MathMLDeclareElement::occurrence()
  throw(std::exception&)
{
  return static_cast<CDA_Element*>(this)->getAttributeNS
    (MATHML_NS, L"occurrence");
}

void
CDA_MathMLDeclareElement::occurrence(iface::dom::DOMString attr)
  throw(std::exception&)
{
  static_cast<CDA_Element*>(this)->setAttributeNS
    (MATHML_NS, L"occurrence", attr);
}

iface::dom::DOMString
CDA_MathMLDeclareElement::definitionURL()
  throw(std::exception&)
{
  return static_cast<CDA_Element*>(this)->getAttributeNS
    (MATHML_NS, L"definitionURL");
}

void
CDA_MathMLDeclareElement::definitionURL(iface::dom::DOMString attr)
  throw(std::exception&)
{
  static_cast<CDA_Element*>(this)->setAttributeNS
    (MATHML_NS, L"definitionURL", attr);
}

iface::dom::DOMString
CDA_MathMLDeclareElement::encoding()
  throw(std::exception&)
{
  return static_cast<CDA_Element*>(this)->getAttributeNS
    (MATHML_NS, L"encoding");
}

void
CDA_MathMLDeclareElement::encoding(iface::dom::DOMString attr)
  throw(std::exception&)
{
  static_cast<CDA_Element*>(this)->setAttributeNS
    (MATHML_NS, L"encoding", attr);
}

iface::mathml_dom::MathMLCiElement*
CDA_MathMLDeclareElement::identifier()
  throw(std::exception&)
{
  iface::dom::NodeList *cn = static_cast<CDA_Element*>(this)->childNodes();
  try
  {
    u_int32_t l = cn->length();
    u_int32_t i = 0;
    for (i = 0; i < l; i++)
    {
      iface::dom::Node* n = cn->item(i);
      iface::mathml_dom::MathMLCiElement* ce =
        dynamic_cast<iface::mathml_dom::MathMLCiElement*>(n);
      if (ce == NULL)
      {
        n->release_ref();
        continue;
      }
      else
      {
        cn->release_ref();
        return ce;
      }
    }
  }
  catch (std::exception& e)
  {
    cn->release_ref();
    throw e;
  }
  cn->release_ref();
  throw iface::dom::DOMException();
}

void
CDA_MathMLDeclareElement::identifier(iface::mathml_dom::MathMLCiElement* attr)
  throw(std::exception&)
{
  iface::dom::NodeList *cn = static_cast<CDA_Element*>(this)->childNodes();
  try
  {
    u_int32_t l = cn->length();
    u_int32_t i = 0;
    for (i = 0; i < l; i++)
    {
      iface::dom::Node* n = cn->item(i);
      iface::mathml_dom::MathMLCiElement* ce =
        dynamic_cast<iface::mathml_dom::MathMLCiElement*>(n);
      if (ce == NULL)
      {
        n->release_ref();
        continue;
      }
      else
      {
        cn->release_ref();
        replaceChild(attr, n)->release_ref();
        return;
      }
    }
  }
  catch (std::exception& e)
  {
    cn->release_ref();
    throw e;
  }
  cn->release_ref();
  appendChild(attr)->release_ref();
}

iface::mathml_dom::MathMLElement*
CDA_MathMLDeclareElement::constructor()
  throw(std::exception&)
{
  iface::dom::NodeList *cn = static_cast<CDA_Element*>(this)->childNodes();
  bool got_identifier = false;
  try
  {
    u_int32_t l = cn->length();
    u_int32_t i = 0;
    for (i = 0; i < l; i++)
    {
      iface::dom::Node* n = cn->item(i);
      iface::mathml_dom::MathMLElement* ce =
        dynamic_cast<iface::mathml_dom::MathMLElement*>(n);
      if (ce == NULL)
      {
        n->release_ref();
        continue;
      }
      else if (!got_identifier)
      {
        n->release_ref();
        got_identifier = true;
        continue;
      }
      else
      {
        cn->release_ref();
        return ce;
      }
    }
  }
  catch (std::exception& e)
  {
    cn->release_ref();
    throw e;
  }
  cn->release_ref();
  throw iface::dom::DOMException();
}

void
CDA_MathMLDeclareElement::constructor(iface::mathml_dom::MathMLElement* attr)
  throw(std::exception&)
{
  iface::dom::NodeList *cn = static_cast<CDA_Element*>(this)->childNodes();
  bool got_identifier = false;
  try
  {
    u_int32_t l = cn->length();
    u_int32_t i = 0;
    for (i = 0; i < l; i++)
    {
      iface::dom::Node* n = cn->item(i);
      iface::mathml_dom::MathMLElement* ce =
        dynamic_cast<iface::mathml_dom::MathMLElement*>(n);
      if (ce == NULL)
      {
        n->release_ref();
        continue;
      }
      else if (!got_identifier)
      {
        n->release_ref();
        got_identifier = true;
        continue;
      }
      else
      {
        cn->release_ref();
        replaceChild(attr, n)->release_ref();
        n->release_ref();
        return;
      }
    }
  }
  catch (std::exception& e)
  {
    cn->release_ref();
    throw e;
  }
  cn->release_ref();
  throw iface::dom::DOMException();
}

CDA_MathMLVectorElement::CDA_MathMLVectorElement(GdomeElement* el)
  : CDA_Element(el), CDA_MathMLElement(el),  CDA_MathMLContentElement(el)
{
}

u_int32_t
CDA_MathMLVectorElement::ncomponents()
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_CONTENT);
  return mfnl.length();
}

iface::mathml_dom::MathMLContentElement*
CDA_MathMLVectorElement::getComponent(u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_CONTENT);
  if (index == 0)
    throw iface::dom::DOMException();
  return dynamic_cast<iface::mathml_dom::MathMLContentElement*>(mfnl.item(index));
}

iface::mathml_dom::MathMLContentElement*
CDA_MathMLVectorElement::insertComponent(iface::mathml_dom::MathMLContentElement* newComponent, u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_CONTENT);

  // XXX this isn't threadsafe, but the DOM provides no atomic approach.
  u_int32_t l = mfnl.length();
  if (index > l + 1)
    throw iface::dom::DOMException();
  if (index == 0 || index == l + 1)
  {
    return dynamic_cast<iface::mathml_dom::MathMLContentElement*>
      (appendChild(newComponent));
  }

  // Get the child...
  iface::dom::Node* ref = mfnl.item(index - 1);
  iface::mathml_dom::MathMLContentElement* newA =
    dynamic_cast<iface::mathml_dom::MathMLContentElement*>
    (insertBefore(newComponent, ref));
  ref->release_ref();
  return newA;
}

iface::mathml_dom::MathMLContentElement*
CDA_MathMLVectorElement::setComponent(iface::mathml_dom::MathMLContentElement* newComponent, u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_CONTENT);

  // XXX this isn't threadsafe, but the DOM provides no atomic approach.
  u_int32_t l = mfnl.length();
  if (index < 1 || index > l + 1)
    throw iface::dom::DOMException();
  if (index == l + 1)
  {
    return dynamic_cast<iface::mathml_dom::MathMLContentElement*>
      (appendChild(newComponent));
  }

  iface::dom::Node* old = mfnl.item(index - 1);
  iface::mathml_dom::MathMLContentElement* newA =
    dynamic_cast<iface::mathml_dom::MathMLContentElement*>(replaceChild(newComponent,
                                                                 old));
  old->release_ref();
  return newA;
}

void CDA_MathMLVectorElement::deleteComponent(u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_CONTENT);
  iface::dom::Node* old = mfnl.item(index - 1);
  iface::dom::Node* old2 = removeChild(old);
  old->release_ref();
  old2->release_ref();
}

iface::mathml_dom::MathMLContentElement*
CDA_MathMLVectorElement::removeComponent(u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_CONTENT);
  iface::dom::Node* old = mfnl.item(index - 1);
  iface::dom::Node* old2 = removeChild(old);
  old->release_ref();
  return dynamic_cast<iface::mathml_dom::MathMLContentElement*>(old2);
}

CDA_MathMLMatrixElement::CDA_MathMLMatrixElement(GdomeElement* el)
  : CDA_Element(el), CDA_MathMLElement(el),  CDA_MathMLContentElement(el)
{
}

u_int32_t CDA_MathMLMatrixElement::nrows()
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_ROW);
  return mfnl.length();
}

u_int32_t
CDA_MathMLMatrixElement::ncols()
  throw(std::exception&)
{
  if (nrows() == 0)
    return 0;
  iface::mathml_dom::MathMLMatrixrowElement* mre = getRow(1);
  // Just return the number of entries in the first row, because matrices are
  // supposed to have the same number in each row.
  return mre->nEntries();
}

iface::mathml_dom::MathMLNodeList*
CDA_MathMLMatrixElement::rows()
  throw(std::exception&)
{
  return new CDA_MathMLFilteredNodeList(this,
                                        CDA_MathMLFilteredNodeList::
                                        FILTER_ROW);
}

iface::mathml_dom::MathMLMatrixrowElement*
CDA_MathMLMatrixElement::getRow(u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_ROW);
  if (index == 0)
    throw iface::dom::DOMException();
  return dynamic_cast<iface::mathml_dom::MathMLMatrixrowElement*>(mfnl.item(index));
}

iface::mathml_dom::MathMLMatrixrowElement*
CDA_MathMLMatrixElement::insertRow
(iface::mathml_dom::MathMLMatrixrowElement* newRow, u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_ROW);

  // XXX this isn't threadsafe, but the DOM provides no atomic approach.
  u_int32_t l = mfnl.length();
  if (index > l + 1)
    throw iface::dom::DOMException();
  if (index == 0 || index == l + 1)
  {
    return dynamic_cast<iface::mathml_dom::MathMLMatrixrowElement*>
      (appendChild(newRow));
  }

  // Get the child...
  iface::dom::Node* ref = mfnl.item(index - 1);
  iface::mathml_dom::MathMLMatrixrowElement* newA =
    dynamic_cast<iface::mathml_dom::MathMLMatrixrowElement*>
    (insertBefore(newRow, ref));
  ref->release_ref();
  return newA;
}

iface::mathml_dom::MathMLMatrixrowElement*
CDA_MathMLMatrixElement::setRow(iface::mathml_dom::MathMLMatrixrowElement* newRow, u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_ROW);

  // XXX this isn't threadsafe, but the DOM provides no atomic approach.
  u_int32_t l = mfnl.length();
  if (index < 1 || index > l + 1)
    throw iface::dom::DOMException();
  if (index == l + 1)
  {
    return dynamic_cast<iface::mathml_dom::MathMLMatrixrowElement*>
      (appendChild(newRow));
  }

  iface::dom::Node* old = mfnl.item(index - 1);
  iface::mathml_dom::MathMLMatrixrowElement* newA =
    dynamic_cast<iface::mathml_dom::MathMLMatrixrowElement*>(replaceChild(newRow,
                                                                          old));
  old->release_ref();
  return newA;
}

void
CDA_MathMLMatrixElement::deleteRow(u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_ROW);
  iface::dom::Node* old = mfnl.item(index - 1);
  iface::dom::Node* old2 = removeChild(old);
  old->release_ref();
  old2->release_ref();
}

iface::mathml_dom::MathMLMatrixrowElement*
CDA_MathMLMatrixElement::removeRow(u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_ROW);
  iface::dom::Node* old = mfnl.item(index - 1);
  iface::dom::Node* old2 = removeChild(old);
  old->release_ref();
  return dynamic_cast<iface::mathml_dom::MathMLMatrixrowElement*>(old2);
}

CDA_MathMLMatrixrowElement::CDA_MathMLMatrixrowElement(GdomeElement* el)
  : CDA_Element(el), CDA_MathMLElement(el),  CDA_MathMLContentElement(el)
{
}

u_int32_t
CDA_MathMLMatrixrowElement::nEntries()
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_CONTENT);
  return mfnl.length();  
}

iface::mathml_dom::MathMLContentElement*
CDA_MathMLMatrixrowElement::getEntry(u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_CONTENT);
  if (index == 0)
    throw iface::dom::DOMException();
  return dynamic_cast<iface::mathml_dom::MathMLContentElement*>(mfnl.item(index));
}

iface::mathml_dom::MathMLContentElement*
CDA_MathMLMatrixrowElement::insertEntry(iface::mathml_dom::MathMLContentElement* newEntry, u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_CONTENT);

  // XXX this isn't threadsafe, but the DOM provides no atomic approach.
  u_int32_t l = mfnl.length();
  if (index > l + 1)
    throw iface::dom::DOMException();
  if (index == 0 || index == l + 1)
  {
    return dynamic_cast<iface::mathml_dom::MathMLContentElement*>
      (appendChild(newEntry));
  }

  // Get the child...
  iface::dom::Node* ref = mfnl.item(index - 1);
  iface::mathml_dom::MathMLContentElement* newA =
    dynamic_cast<iface::mathml_dom::MathMLContentElement*>
    (insertBefore(newEntry, ref));
  ref->release_ref();
  return newA;
}

iface::mathml_dom::MathMLContentElement*
CDA_MathMLMatrixrowElement::setEntry(iface::mathml_dom::MathMLContentElement* newEntry, u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_CONTENT);

  // XXX this isn't threadsafe, but the DOM provides no atomic approach.
  u_int32_t l = mfnl.length();
  if (index < 1 || index > l + 1)
    throw iface::dom::DOMException();
  if (index == l + 1)
  {
    return dynamic_cast<iface::mathml_dom::MathMLContentElement*>
      (appendChild(newEntry));
  }

  iface::dom::Node* old = mfnl.item(index - 1);
  iface::mathml_dom::MathMLContentElement* newA =
    dynamic_cast<iface::mathml_dom::MathMLContentElement*>(replaceChild(newEntry,
                                                                 old));
  old->release_ref();
  return newA;
}

void
CDA_MathMLMatrixrowElement::deleteEntry(u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_CONTENT);
  iface::dom::Node* old = mfnl.item(index - 1);
  iface::dom::Node* old2 = removeChild(old);
  old->release_ref();
  old2->release_ref();
}

iface::mathml_dom::MathMLContentElement*
CDA_MathMLMatrixrowElement::removeEntry(u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_CONTENT);
  iface::dom::Node* old = mfnl.item(index - 1);
  iface::dom::Node* old2 = removeChild(old);
  old->release_ref();
  return dynamic_cast<iface::mathml_dom::MathMLContentElement*>(old2);
}

CDA_MathMLPiecewiseElement::CDA_MathMLPiecewiseElement(GdomeElement* el)
  : CDA_Element(el), CDA_MathMLElement(el),  CDA_MathMLContentElement(el)
{
}

iface::mathml_dom::MathMLNodeList*
CDA_MathMLPiecewiseElement::pieces()
  throw(std::exception&)
{
  return new CDA_MathMLFilteredNodeList(this,
                                        CDA_MathMLFilteredNodeList::
                                        FILTER_PIECE);
}

iface::mathml_dom::MathMLContentElement*
CDA_MathMLPiecewiseElement::otherwise()
  throw(std::exception&)
{
  // See if we can find an otherwise...
  iface::dom::NodeList* nl = childNodes();
  iface::dom::Node* n;
  u_int32_t i, l = nl->length();
  for (i = 0; i < l; i++)
  {
    n = nl->item(i);
    if (IsOtherwise(n))
    {
      nl->release_ref();
      return dynamic_cast<iface::mathml_dom::MathMLContentElement*>(n);
    }
    n->release_ref();
  }
  nl->release_ref();

  // There is no otherwise yet.
  throw iface::dom::DOMException();
}

void
CDA_MathMLPiecewiseElement::otherwise(iface::mathml_dom::MathMLContentElement* attr)
  throw(std::exception&)
{
  // See if we can find an otherwise...
  iface::dom::NodeList* nl = childNodes();
  iface::dom::Node* n;
  u_int32_t i, l = nl->length();
  for (i = 0; i < l; i++)
  {
    n = nl->item(i);
    if (IsOtherwise(n))
    {
      nl->release_ref();
      replaceChild(attr, n);
      n->release_ref();
      return;
    }
    n->release_ref();
  }
  nl->release_ref();

  // There is no otherwise yet.
  appendChild(attr);
}

iface::mathml_dom::MathMLCaseElement*
CDA_MathMLPiecewiseElement::getCase(u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_PIECE);
  if (index == 0)
    throw iface::dom::DOMException();
  return dynamic_cast<iface::mathml_dom::MathMLCaseElement*>(mfnl.item(index));
}

iface::mathml_dom::MathMLCaseElement*
CDA_MathMLPiecewiseElement::setCase(u_int32_t index, iface::mathml_dom::MathMLCaseElement* caseEl)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_PIECE);

  // XXX this isn't threadsafe, but the DOM provides no atomic approach.
  u_int32_t l = mfnl.length();
  if (index < 1 || index > l + 1)
    throw iface::dom::DOMException();
  if (index == l + 1)
  {
    return dynamic_cast<iface::mathml_dom::MathMLCaseElement*>
      (appendChild(caseEl));
  }

  iface::dom::Node* old = mfnl.item(index - 1);
  iface::mathml_dom::MathMLCaseElement* newA =
    dynamic_cast<iface::mathml_dom::MathMLCaseElement*>(replaceChild(caseEl,
                                                                     old));
  old->release_ref();
  return newA;
}

void
CDA_MathMLPiecewiseElement::deleteCase(u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_PIECE);
  iface::dom::Node* old = mfnl.item(index - 1);
  iface::dom::Node* old2 = removeChild(old);
  old->release_ref();
  old2->release_ref();
}

iface::mathml_dom::MathMLCaseElement*
CDA_MathMLPiecewiseElement::removeCase(u_int32_t index)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_PIECE);
  iface::dom::Node* old = mfnl.item(index - 1);
  iface::dom::Node* old2 = removeChild(old);
  old->release_ref();
  return dynamic_cast<iface::mathml_dom::MathMLCaseElement*>(old2);
}

iface::mathml_dom::MathMLCaseElement*
CDA_MathMLPiecewiseElement::insertCase(u_int32_t index, iface::mathml_dom::MathMLCaseElement* newCase)
  throw(std::exception&)
{
  CDA_MathMLFilteredNodeList mfnl(this,
                                  CDA_MathMLFilteredNodeList::
                                  FILTER_PIECE);

  // XXX this isn't threadsafe, but the DOM provides no atomic approach.
  u_int32_t l = mfnl.length();
  if (index > l + 1)
    throw iface::dom::DOMException();
  if (index == 0 || index == l + 1)
  {
    return dynamic_cast<iface::mathml_dom::MathMLCaseElement*>
      (appendChild(newCase));
  }

  // Get the child...
  iface::dom::Node* ref = mfnl.item(index - 1);
  iface::mathml_dom::MathMLCaseElement* newA =
    dynamic_cast<iface::mathml_dom::MathMLCaseElement*>
    (insertBefore(newCase, ref));
  ref->release_ref();
  return newA;
}

iface::mathml_dom::MathMLContentElement*
CDA_MathMLPiecewiseElement::getCaseValue(u_int32_t index)
  throw(std::exception&)
{
  iface::mathml_dom::MathMLCaseElement* ce = getCase(index);
  iface::mathml_dom::MathMLContentElement* ret = ce->caseValue();
  ce->release_ref();
  return ret;
}

iface::mathml_dom::MathMLContentElement*
CDA_MathMLPiecewiseElement::setCaseValue(u_int32_t index, iface::mathml_dom::MathMLContentElement* value)
  throw(std::exception&)
{
  iface::mathml_dom::MathMLCaseElement* ce = getCase(index);
  ce->caseValue(value);
  ce->release_ref();
  value->add_ref();
  return value;
}

iface::mathml_dom::MathMLContentElement*
CDA_MathMLPiecewiseElement::getCaseCondition(u_int32_t index)
  throw(std::exception&)
{
  iface::mathml_dom::MathMLCaseElement* ce = getCase(index);
  iface::mathml_dom::MathMLContentElement* ret = ce->caseCondition();
  ce->release_ref();
  return ret;
}

iface::mathml_dom::MathMLContentElement*
CDA_MathMLPiecewiseElement::setCaseCondition(u_int32_t index, iface::mathml_dom::MathMLContentElement* condition)
  throw(std::exception&)
{
  iface::mathml_dom::MathMLCaseElement* ce = getCase(index);
  ce->caseCondition(condition);
  ce->release_ref();
  condition->add_ref();
  return condition;
}

CDA_MathMLCaseElement::CDA_MathMLCaseElement(GdomeElement* el)
  : CDA_Element(el), CDA_MathMLElement(el),  CDA_MathMLContentElement(el)
{
}

iface::mathml_dom::MathMLContentElement*
CDA_MathMLCaseElement::caseValue    ()
  throw(std::exception&)
{
  iface::dom::NodeList *cn = static_cast<CDA_Element*>(this)->childNodes();
  try
  {
    u_int32_t l = cn->length();
    u_int32_t i = 0;
    for (i = 0; i < l; i++)
    {
      iface::dom::Node* n = cn->item(i);
      iface::mathml_dom::MathMLContentElement* ce =
        dynamic_cast<iface::mathml_dom::MathMLContentElement*>(n);
      if (ce == NULL)
      {
        n->release_ref();
        continue;
      }
      else
      {
        cn->release_ref();
        return ce;
      }
    }
  }
  catch (std::exception& e)
  {
    cn->release_ref();
    throw e;
  }
  cn->release_ref();
  throw iface::dom::DOMException();
}

void
CDA_MathMLCaseElement::caseValue(iface::mathml_dom::MathMLContentElement* attr)
  throw(std::exception&)
{
  iface::dom::NodeList *cn = static_cast<CDA_Element*>(this)->childNodes();
  try
  {
    u_int32_t l = cn->length();
    u_int32_t i = 0;
    for (i = 0; i < l; i++)
    {
      iface::dom::Node* n = cn->item(i);
      iface::mathml_dom::MathMLContentElement* ce =
        dynamic_cast<iface::mathml_dom::MathMLContentElement*>(n);
      if (ce == NULL)
      {
        n->release_ref();
        continue;
      }
      else
      {
        cn->release_ref();
        replaceChild(attr, n)->release_ref();
        return;
      }
    }
  }
  catch (std::exception& e)
  {
    cn->release_ref();
    throw e;
  }
  cn->release_ref();
  appendChild(attr)->release_ref();
}

iface::mathml_dom::MathMLContentElement*
CDA_MathMLCaseElement::caseCondition()
  throw(std::exception&)
{
  iface::dom::NodeList *cn = static_cast<CDA_Element*>(this)->childNodes();
  bool got_condition = false;
  try
  {
    u_int32_t l = cn->length();
    u_int32_t i = 0;
    for (i = 0; i < l; i++)
    {
      iface::dom::Node* n = cn->item(i);
      iface::mathml_dom::MathMLContentElement* ce =
        dynamic_cast<iface::mathml_dom::MathMLContentElement*>(n);
      if (ce == NULL)
      {
        n->release_ref();
        continue;
      }
      else if (!got_condition)
      {
        n->release_ref();
        got_condition = true;
        continue;
      }
      else
      {
        cn->release_ref();
        return ce;
      }
    }
  }
  catch (std::exception& e)
  {
    cn->release_ref();
    throw e;
  }
  cn->release_ref();
  throw iface::dom::DOMException();
}

void
CDA_MathMLCaseElement::caseCondition(iface::mathml_dom::MathMLContentElement* attr)
  throw(std::exception&)
{
  iface::dom::NodeList *cn = static_cast<CDA_Element*>(this)->childNodes();
  bool got_condition = false;
  try
  {
    u_int32_t l = cn->length();
    u_int32_t i = 0;
    for (i = 0; i < l; i++)
    {
      iface::dom::Node* n = cn->item(i);
      iface::mathml_dom::MathMLContentElement* ce =
        dynamic_cast<iface::mathml_dom::MathMLContentElement*>(n);
      if (ce == NULL)
      {
        n->release_ref();
        continue;
      }
      else if (!got_condition)
      {
        n->release_ref();
        got_condition = true;
        continue;
      }
      else
      {
        cn->release_ref();
        replaceChild(attr, n)->release_ref();
        n->release_ref();
        return;
      }
    }
  }
  catch (std::exception& e)
  {
    cn->release_ref();
    throw e;
  }
  cn->release_ref();
  throw iface::dom::DOMException();
}

#define C(x) static iface::dom::Element*\
ConstructMathML##x(GdomeElement* el) \
{ \
  return new CDA_MathML##x(el); \
}
C(ApplyElement);
C(PredefinedSymbol);
C(BvarElement);
C(CiElement);
C(CnElement);
C(ConditionElement);
C(CsymbolElement);
C(DeclareElement);
C(ContentContainer);
C(FnElement);
C(IntervalElement);
C(LambdaElement);
C(ListElement);
C(MathElement);
C(MatrixElement);
C(MatrixrowElement);
C(CaseElement);
C(PiecewiseElement);
C(SetElement);
C(TendsToElement);
C(VectorElement);
C(Element);
#undef C

iface::dom::Element*
WrapMathMLElement(GdomeElement* el)
{
  struct
  {
    const wchar_t* name;
    iface::dom::Element* (*factory)(GdomeElement* el);
  } MathMLConstructors[] =
  { // Note: The presentation API isn't implemented.
    {L"abs", ConstructMathMLPredefinedSymbol},
    {L"and", ConstructMathMLPredefinedSymbol},
#ifdef IMPL_PRESMATHML
    {L"annotation", ConstructMathMLAnnotationElement},
    {L"annotation-xml", ConstructMathMLXMLAnnotationElement},
#endif
    {L"apply", ConstructMathMLApplyElement},
    {L"approx", ConstructMathMLPredefinedSymbol},
    {L"arccos", ConstructMathMLPredefinedSymbol},
    {L"arccosh", ConstructMathMLPredefinedSymbol},
    {L"arccot", ConstructMathMLPredefinedSymbol},
    {L"arccoth", ConstructMathMLPredefinedSymbol},
    {L"arccsc", ConstructMathMLPredefinedSymbol},
    {L"arccsch", ConstructMathMLPredefinedSymbol},
    {L"arcsec", ConstructMathMLPredefinedSymbol},
    {L"arcsech", ConstructMathMLPredefinedSymbol},
    {L"arcsin", ConstructMathMLPredefinedSymbol},
    {L"arcsinh", ConstructMathMLPredefinedSymbol},
    {L"arctan", ConstructMathMLPredefinedSymbol},
    {L"arctanh", ConstructMathMLPredefinedSymbol},
    {L"arg", ConstructMathMLPredefinedSymbol},
    {L"bvar", ConstructMathMLBvarElement},
    {L"card", ConstructMathMLPredefinedSymbol},
    {L"cartesianproduct", ConstructMathMLPredefinedSymbol},
    {L"ceiling", ConstructMathMLPredefinedSymbol},
    {L"ci", ConstructMathMLCiElement},
    {L"cn", ConstructMathMLCnElement},
    {L"codomain", ConstructMathMLPredefinedSymbol},
    {L"complexes", ConstructMathMLPredefinedSymbol},
    {L"compose", ConstructMathMLPredefinedSymbol},
    {L"condition", ConstructMathMLConditionElement},
    {L"conjugate", ConstructMathMLPredefinedSymbol},
    {L"cos", ConstructMathMLPredefinedSymbol},
    {L"cosh", ConstructMathMLPredefinedSymbol},
    {L"cot", ConstructMathMLPredefinedSymbol},
    {L"coth", ConstructMathMLPredefinedSymbol},
    {L"csc", ConstructMathMLPredefinedSymbol},
    {L"csch", ConstructMathMLPredefinedSymbol},
    {L"csymbol", ConstructMathMLCsymbolElement},
    {L"curl", ConstructMathMLPredefinedSymbol},
    {L"declare", ConstructMathMLDeclareElement},
    {L"degree", ConstructMathMLContentContainer},
    {L"determinant", ConstructMathMLPredefinedSymbol},
    {L"diff", ConstructMathMLPredefinedSymbol},
    {L"divergence", ConstructMathMLPredefinedSymbol},
    {L"divide", ConstructMathMLPredefinedSymbol},
    {L"domain", ConstructMathMLPredefinedSymbol},
    {L"domainofapplication", ConstructMathMLContentContainer},
    {L"emptyset", ConstructMathMLPredefinedSymbol},
    {L"eq", ConstructMathMLPredefinedSymbol},
    {L"equivalent", ConstructMathMLPredefinedSymbol},
    {L"eulergamma", ConstructMathMLPredefinedSymbol},
    {L"exists", ConstructMathMLPredefinedSymbol},
    {L"exp", ConstructMathMLPredefinedSymbol},
    {L"exp", ConstructMathMLPredefinedSymbol},
    {L"exponentiale", ConstructMathMLPredefinedSymbol},
    {L"factorial", ConstructMathMLPredefinedSymbol},
    {L"factorof", ConstructMathMLPredefinedSymbol},
    {L"false", ConstructMathMLPredefinedSymbol},
    {L"floor", ConstructMathMLPredefinedSymbol},
    {L"fn", ConstructMathMLFnElement},
    {L"forall", ConstructMathMLPredefinedSymbol},
    {L"gcd", ConstructMathMLPredefinedSymbol},
    {L"geq", ConstructMathMLPredefinedSymbol},
    {L"grad", ConstructMathMLPredefinedSymbol},
    {L"gt", ConstructMathMLPredefinedSymbol},
    {L"ident", ConstructMathMLPredefinedSymbol},
    {L"image", ConstructMathMLPredefinedSymbol},
    {L"imaginary", ConstructMathMLPredefinedSymbol},
    {L"imaginaryi", ConstructMathMLPredefinedSymbol},
    {L"implies", ConstructMathMLPredefinedSymbol},
    {L"in", ConstructMathMLPredefinedSymbol},
    {L"infinity", ConstructMathMLPredefinedSymbol},
    {L"int", ConstructMathMLPredefinedSymbol},
    {L"integers", ConstructMathMLPredefinedSymbol},
    {L"intersect", ConstructMathMLPredefinedSymbol},
    {L"interval", ConstructMathMLIntervalElement},
    {L"inverse", ConstructMathMLPredefinedSymbol},
    {L"lambda", ConstructMathMLLambdaElement},
    {L"laplacian", ConstructMathMLPredefinedSymbol},
    {L"lcm", ConstructMathMLPredefinedSymbol},
    {L"leq", ConstructMathMLPredefinedSymbol},
    {L"limit", ConstructMathMLPredefinedSymbol},
    {L"list", ConstructMathMLListElement},
    {L"ln", ConstructMathMLPredefinedSymbol},
    {L"log", ConstructMathMLPredefinedSymbol},
    {L"lowlimit", ConstructMathMLContentContainer},
    {L"lt", ConstructMathMLPredefinedSymbol},
#ifdef IMPL_PRESMATHML
    {L"maction", ConstructMathMLActionElement},
    {L"maligngroup", ConstructMathMLAlignGroupElement},
    {L"malignmark", ConstructMathMLAlignMarkElement},
#endif
    {L"math", ConstructMathMLMathElement},
    {L"matrix", ConstructMathMLMatrixElement},
    {L"matrixrow", ConstructMathMLMatrixrowElement},
    {L"max", ConstructMathMLPredefinedSymbol},
    {L"mean", ConstructMathMLPredefinedSymbol},
    {L"median", ConstructMathMLPredefinedSymbol},
#ifdef IMPL_PRESMATHML
    {L"menclose", ConstructMathMLEncloseElement},
    {L"merror", ConstructMathMLPresentationContainer},
    {L"mfenced", ConstructMathMLFencedElement},
    {L"mfrac", ConstructMathMLFractionElement},
    {L"mglyph", ConstructMathMLGlyphElement},
    {L"mi", ConstructMathMLPresentationToken},
#endif
    {L"min", ConstructMathMLPredefinedSymbol},
    {L"minus", ConstructMathMLPredefinedSymbol},
#ifdef IMPL_PRESMATHML
    {L"mlabeledtr", ConstructMathMLLabeledRowElement},
    {L"mmultiscripts", ConstructMathMLMultiScriptsElement},
    {L"mn", ConstructMathMLPresentationToken},
    {L"mo", ConstructMathMLOperatorElement},
#endif
    {L"mode", ConstructMathMLPredefinedSymbol},
    {L"moment", ConstructMathMLPredefinedSymbol},
    {L"momentabout", ConstructMathMLContentContainer},
#ifdef IMPL_PRESMATHML
    {L"mover", ConstructMathMLUnderOverElement},
    {L"mpadded", ConstructMathMLPaddedElement},
    {L"mphantom", ConstructMathMLPresentationContainer},
    {L"mroot", ConstructMathMLRadicalElement},
    {L"mrow", ConstructMathMLPresentationContainer},
    {L"ms", ConstructMathMLStringLitElement},
    {L"mspace", ConstructMathMLSpaceElement},
    {L"msqrt", ConstructMathMLRadicalElement},
    {L"mstyle", ConstructMathMLStyleElement},
    {L"msub", ConstructMathMLScriptElement},
    {L"msubsup", ConstructMathMLScriptElement},
    {L"msup", ConstructMathMLScriptElement},
    {L"mtable", ConstructMathMLTableElement},
    {L"mtd", ConstructMathMLTableCellElement},
    {L"mtext", ConstructMathMLPresentationToken},
    {L"mtr", ConstructMathMLTableRowElement},
    {L"munder", ConstructMathMLUnderOverElement},
    {L"munderover", ConstructMathMLUnderOverElement},
#endif
    {L"naturalnumbers", ConstructMathMLPredefinedSymbol},
    {L"neq", ConstructMathMLPredefinedSymbol},
    {L"not", ConstructMathMLPredefinedSymbol},
    {L"notanumber", ConstructMathMLPredefinedSymbol},
    {L"notin", ConstructMathMLPredefinedSymbol},
    {L"notprsubset", ConstructMathMLPredefinedSymbol},
    {L"notsubset", ConstructMathMLPredefinedSymbol},
    {L"or", ConstructMathMLPredefinedSymbol},
    {L"otherwise", ConstructMathMLContentContainer},
    {L"outerproduct", ConstructMathMLPredefinedSymbol},
    {L"partialdiff", ConstructMathMLPredefinedSymbol},
    {L"pi", ConstructMathMLPredefinedSymbol},
    {L"piece", ConstructMathMLCaseElement},
    {L"piecewise", ConstructMathMLPiecewiseElement},
    {L"plus", ConstructMathMLPredefinedSymbol},
    {L"power", ConstructMathMLPredefinedSymbol},
    {L"primes", ConstructMathMLPredefinedSymbol},
    {L"product", ConstructMathMLPredefinedSymbol},
    {L"prsubset", ConstructMathMLPredefinedSymbol},
    {L"quotient", ConstructMathMLPredefinedSymbol},
    {L"rationals", ConstructMathMLPredefinedSymbol},
    {L"real", ConstructMathMLPredefinedSymbol},
    {L"reals", ConstructMathMLPredefinedSymbol},
    {L"reln", ConstructMathMLContentContainer},
    {L"rem", ConstructMathMLPredefinedSymbol},
    {L"root", ConstructMathMLPredefinedSymbol},
    {L"scalarproduct", ConstructMathMLPredefinedSymbol},
    {L"sdev", ConstructMathMLPredefinedSymbol},
    {L"sec", ConstructMathMLPredefinedSymbol},
    {L"sech", ConstructMathMLPredefinedSymbol},
    {L"selector", ConstructMathMLPredefinedSymbol},
#ifdef IMPL_PRESMATHML
    {L"semantics", ConstructMathMLSemanticsElement},
#endif
    {L"set", ConstructMathMLSetElement},
    {L"setdiff", ConstructMathMLPredefinedSymbol},
    {L"sin", ConstructMathMLPredefinedSymbol},
    {L"sinh", ConstructMathMLPredefinedSymbol},
    {L"subset", ConstructMathMLPredefinedSymbol},
    {L"sum", ConstructMathMLPredefinedSymbol},
    {L"tan", ConstructMathMLPredefinedSymbol},
    {L"tanh", ConstructMathMLPredefinedSymbol},
    {L"tendsto", ConstructMathMLTendsToElement},
    {L"times", ConstructMathMLPredefinedSymbol},
    {L"transpose", ConstructMathMLPredefinedSymbol},
    {L"true", ConstructMathMLPredefinedSymbol},
    {L"union", ConstructMathMLPredefinedSymbol},
    {L"uplimit", ConstructMathMLContentContainer},
    {L"variance", ConstructMathMLPredefinedSymbol},
    {L"vector", ConstructMathMLVectorElement},
    {L"vectorproduct", ConstructMathMLPredefinedSymbol},
    {L"xor", ConstructMathMLPredefinedSymbol}
  };

  // It is guaranteed to be in the MathML namespace. Get the local name...
  EXCEPTION_TRY;
  GdomeDOMString* str = gdome_el_localName(el, &exc);
  EXCEPTION_CATCH;

  TRGDOMSTRING(str);
  u_int32_t min_entry = 0;
  u_int32_t max_entry = sizeof(MathMLConstructors)/sizeof(*MathMLConstructors) - 1;
  while (min_entry <= max_entry)
  {
    u_int32_t selentry = (max_entry + min_entry) / 2;
    int x = wcscmp(cxxstr, MathMLConstructors[selentry].name);
    if (x == 0)
    {
      return MathMLConstructors[selentry].factory(el);
      free(cxxstr);
    }
    else if (x > 0)
    {
      if (selentry == sizeof(MathMLConstructors)/sizeof(*MathMLConstructors) - 1)
        break;
      min_entry = selentry + 1;
    }
    else
    {
      if (selentry == 0)
        break;
      max_entry = selentry - 1;
    }
  }
  free(cxxstr);
  return ConstructMathMLElement(el);
}

iface::dom::Document*
WrapMathMLDocument(GdomeDocument* doc)
{
  return new CDA_MathMLDocument(doc);
}
