#define IN_MALAES_MODULE
#include "MaLaESImpl.hpp"
#include "MaLaESBootstrap.hpp"
#include <math.h>
#include <sstream>

#ifdef _WIN32
#undef __STDC__
#include <stdio.h>
#define swprintf _snwprintf
#endif

#define MATHML_NS L"http://www.w3.org/1998/Math/MathML"

class MaLaESError
{
public:
  MaLaESError(const std::wstring& aMessage)
    : message(aMessage) {}

  std::wstring message;
};

CDAMaLaESResult::CDAMaLaESResult
(
 CDAMaLaESTransform* aTransform,
 iface::cellml_services::CeVAS* aCeVAS,
 iface::cellml_services::CUSES* aCUSES,
 iface::cellml_services::AnnotationSet* aAnnos,
 iface::cellml_api::CellMLElement* aContext,
 bool aVariablesFromSource
)
  : _cda_refcount(1), mTransform(aTransform), mCeVAS(aCeVAS), mCUSES(aCUSES),
    mAnnos(aAnnos), mLastUnique(0), boundVariable(false), degree(0),
    mVariablesFromSource(aVariablesFromSource), mInvolvesExternalCode(false)
{
  mPrec.push_back(std::pair<uint32_t, bool>(0, true));

  QUERY_INTERFACE(mContext, aContext, cellml_api::CellMLComponent);
  ObjRef<iface::cellml_api::CellMLElement> el(aContext);
  while (mContext == NULL)
  {
    RETURN_INTO_OBJREF(p, iface::cellml_api::CellMLElement, el->parentElement());
    el = p;
    if (el == NULL)
      throw MaLaESError(L"Invalid context element supplied.");
    QUERY_INTERFACE(mContext, el, cellml_api::CellMLComponent);
  }
}

CDAMaLaESResult::~CDAMaLaESResult()
{
}

wchar_t*
CDAMaLaESResult::compileErrors()
  throw(std::exception&)
{
  return CDA_wcsdup(mError.c_str());
}

wchar_t*
CDAMaLaESResult::expression()
  throw(std::exception&)
{
  return CDA_wcsdup(mActive.c_str());
}

uint32_t
CDAMaLaESResult::supplementariesLength()
  throw(std::exception&)
{
  return mSupplementaries.size();
}

wchar_t*
CDAMaLaESResult::getSupplementary(uint32_t aIndex)
  throw(std::exception&)
{
  if (aIndex >= mSupplementaries.size())
    return CDA_wcsdup(L"");
  return CDA_wcsdup(mSupplementaries[aIndex].c_str());
}

bool
CDAMaLaESResult::involvesExternalCode()
  throw(std::exception&)
{
  return mInvolvesExternalCode;
}

class CDAMaLaESInvolvedVariableIterator
  : public iface::cellml_api::CellMLVariableIterator
{
public:
  CDA_IMPL_ID;
  CDA_IMPL_REFCOUNT;
  CDA_IMPL_QI2(cellml_api::CellMLVariableIterator,
               cellml_api::CellMLElementIterator);

  CDAMaLaESInvolvedVariableIterator
  (
   CDAMaLaESResult* aResult,
   std::vector<iface::cellml_api::CellMLVariable*>& aVars
  )
    : _cda_refcount(1), mResult(aResult), mVars(aVars)
  {
    mIt = mVars.begin();
  }

  ~CDAMaLaESInvolvedVariableIterator()
  {
  }

  iface::cellml_api::CellMLVariable*
  nextVariable()
    throw(std::exception&)
  {
    if (mIt == mVars.end())
      return NULL;

    iface::cellml_api::CellMLVariable* v = (*mIt);
    mIt++;
    v->add_ref();
    return v;
  }

  iface::cellml_api::CellMLElement*
  next()
    throw(std::exception&)
  {
    return static_cast<iface::cellml_api::CellMLElement*>(nextVariable());
  }

private:
  ObjRef<CDAMaLaESResult> mResult;
  std::vector<iface::cellml_api::CellMLVariable*>& mVars;
  std::vector<iface::cellml_api::CellMLVariable*>::iterator mIt;
};

class CDAMaLaESDegreeVariable
  : public iface::cellml_services::DegreeVariable
{
public:
  CDA_IMPL_ID;
  CDA_IMPL_REFCOUNT;
  CDA_IMPL_QI1(cellml_services::DegreeVariable);

  CDAMaLaESDegreeVariable(uint32_t aDeg, iface::cellml_api::CellMLVariable* aVar)
    : _cda_refcount(1), mDeg(aDeg), mVar(aVar)
  {
  }

  iface::cellml_api::CellMLVariable*
  variable() throw(std::exception&)
  {
    mVar->add_ref();
    return mVar;
  }

  uint32_t
  degree() throw(std::exception&)
  {
    return mDeg;
  }

private:
  uint32_t mDeg;
  ObjRef<iface::cellml_api::CellMLVariable> mVar;
};

class CDAMaLaESInvolvedVariableDegIterator
  : public iface::cellml_services::DegreeVariableIterator
{
public:
  CDA_IMPL_ID;
  CDA_IMPL_REFCOUNT;
  CDA_IMPL_QI1(cellml_services::DegreeVariableIterator);

  CDAMaLaESInvolvedVariableDegIterator
  (
   CDAMaLaESResult* aResult,
   std::vector<std::pair<uint32_t, iface::cellml_api::CellMLVariable*> >& aVars
  )
    : _cda_refcount(1), mResult(aResult), mVars(aVars)
  {
    mIt = mVars.begin();
  }

  ~CDAMaLaESInvolvedVariableDegIterator()
  {
  }

  iface::cellml_services::DegreeVariable*
  nextDegreeVariable()
    throw(std::exception&)
  {
    if (mIt == mVars.end())
      return NULL;

    uint32_t deg = (*mIt).first;
    iface::cellml_api::CellMLVariable* var = (*mIt).second;
    
    mIt++;

    return new CDAMaLaESDegreeVariable(deg, var);
  }

private:
  ObjRef<CDAMaLaESResult> mResult;
  std::vector<std::pair<uint32_t, iface::cellml_api::CellMLVariable*> >& mVars;
  std::vector<std::pair<uint32_t, iface::cellml_api::CellMLVariable*> >::
    iterator mIt;
};

iface::cellml_api::CellMLVariableIterator*
CDAMaLaESResult::iterateInvolvedVariables()
  throw(std::exception&)
{
  return new CDAMaLaESInvolvedVariableIterator(this, mInvolved);
}

iface::cellml_api::CellMLVariableIterator*
CDAMaLaESResult::iterateBoundVariables()
  throw(std::exception&)
{
  return new CDAMaLaESInvolvedVariableIterator(this, mBoundVars);
}

iface::cellml_services::DegreeVariableIterator*
CDAMaLaESResult::iterateInvolvedVariablesByDegree()
  throw(std::exception&)
{
  return new CDAMaLaESInvolvedVariableDegIterator(this, mInvolvedDeg);
}

void
CDAMaLaESResult::finishTransform()
{
  // Clean this up now to save memory...
  mTransform = NULL;
  mCeVAS = NULL;
  mCUSES = NULL;
  mAnnos = NULL;
  mContext = NULL;
}

double
CDAMaLaESResult::startConversionMode(iface::mathml_dom::MathMLCiElement* aCI,
                                     double& aOffset, bool aIsBound)
{
  try
  {
    RETURN_INTO_OBJREF(n, iface::dom::Node, aCI->getArgument(1));
    DECLARE_QUERY_INTERFACE_OBJREF(t, n, dom::Text);
    RETURN_INTO_WSTRING(txt, t->data());
    int i = 0, j = txt.length() - 1;
    wchar_t c;
    while ((c = txt[i]) == ' ' || c == '\t' || c == '\r' || c == '\n')
      i++;
    while ((c = txt[j]) == ' ' || c == '\t' || c == '\r' || c == '\n')
      j--;
    if (j < i)
      throw MaLaESError(L"CI element with only spaces inside.");
    txt = txt.substr(i, j - i + 1);
    
    RETURN_INTO_OBJREF(vs, iface::cellml_api::CellMLVariableSet,
                       mContext->variables());
    RETURN_INTO_OBJREF(v, iface::cellml_api::CellMLVariable,
                       vs->getVariable(txt.c_str()));
    if (v == NULL)
    {
      std::wstring msg = L"Cannot find variable ";
      msg += txt;
      msg += L" named in ci element.";
      throw MaLaESError(msg);
    }
    RETURN_INTO_OBJREF(cvs, iface::cellml_services::ConnectedVariableSet,
                       mCeVAS->findVariableSet(v));
    RETURN_INTO_OBJREF(sv, iface::cellml_api::CellMLVariable,
                       cvs->sourceVariable());
    RETURN_INTO_WSTRING(unameDest, v->unitsName());
    RETURN_INTO_WSTRING(unameSrc, sv->unitsName());
    RETURN_INTO_OBJREF(curDest, iface::cellml_services::CanonicalUnitRepresentation,
                       mCUSES->getUnitsByName(mContext, unameDest.c_str()));
    if (curDest == NULL)
    {
      std::wstring msg = L"Invalid units ";
      msg += unameDest;
      msg += L" on variable.";
      throw MaLaESError(msg);
    }
    RETURN_INTO_OBJREF(curSrc, iface::cellml_services::CanonicalUnitRepresentation,
                       mCUSES->getUnitsByName(sv, unameSrc.c_str()));
    if (curSrc == NULL)
    {
      std::wstring msg = L"Invalid units ";
      msg += unameSrc;
      msg += L" on variable.";
      throw MaLaESError(msg);
    }

    double mup = curSrc->convertUnits(curDest, &aOffset);

    if (mInvolvedSet.count(sv) == 0)
    {
      sv->add_ref();
      mInvolved.push_back(sv);
      mInvolvedSet.insert(sv);

      if (aIsBound)
        mBoundVars.push_back(sv);
    }

    if (mVariablesFromSource)
      processingVariable = sv;
    else
      processingVariable = v;

    if (!aIsBound)
    {
      std::pair<uint32_t, iface::cellml_api::CellMLVariable*>
        dvp(degree, sv);
      if (mInvolvedDegSet.count(dvp) == 0)
      {
        mInvolvedDegSet.insert(dvp);
        mInvolvedDeg.push_back(dvp);
      }

      if (degree > 0)
      {
        mup /= boundMup;
        boundMup = 1.0;
      }
    }

    return mup;
  }
  catch (iface::dom::DOMException& de)
  {
    throw MaLaESError(L"CI Element with no text inside.");
  }
}

bool
CDAMaLaESResult::writeConvertedVariable()
{
  if (processingVariable == NULL)
    return false;

  if (boundVariable)
  {
    RETURN_INTO_WSTRING(expr,
                        mAnnos->getStringAnnotation(processingVariable,
                                                    L"bvarIndex"));
    mActive += expr;
  }
  else if (degree > 0)
  {
    std::map<iface::cellml_api::CellMLVariable*, uint32_t>::iterator i =
      mHighestDegree.find(processingVariable);
    if (i == mHighestDegree.end())
      mHighestDegree.insert(std::pair<iface::cellml_api::CellMLVariable*, uint32_t>
                            (processingVariable, degree));
    else if ((*i).second < degree)
      (*i).second = degree;

    wchar_t buf[30];
    swprintf(buf, 40, L"expression_d%d", degree);
    RETURN_INTO_WSTRING(expr,
                        mAnnos->getStringAnnotation(processingVariable, buf));
    mActive += expr;
    degree = 0;
  }
  else
  {
    RETURN_INTO_WSTRING(expr,
                        mAnnos->getStringAnnotation(processingVariable,
                                                    L"expression"));
    mActive += expr;
  }

  return true;
}

void
CDAMaLaESResult::endConversionMode()
{
  processingVariable = NULL;
}

bool
CDAMaLaESResult::pushPrecedence(uint32_t outer, uint32_t inner)
{
  bool eqNeed = outer > mPrec.back().second;
  if (!eqNeed)
    mPrec.back().second = true;
  bool ret;
  if (eqNeed)
    ret = (outer <= mPrec.back().first);
  else
    ret = (outer < mPrec.back().first);

  mPrec.push_back(std::pair<uint32_t, bool>(inner, false));

  return ret;
}

void
CDAMaLaESResult::popPrecedence()
{
  mPrec.pop_back();
}

void
CDAMaLaESResult::appendString
(
 const std::wstring& aArg,
 std::vector<iface::mathml_dom::MathMLElement*>& aArgs,
 std::vector<iface::mathml_dom::MathMLBvarElement*>& aBvars,
 iface::mathml_dom::MathMLElement* degree,
 iface::mathml_dom::MathMLElement* logbase
)
{
  mActive += aArg;
}

void
CDAMaLaESResult::appendCount
(
 const std::wstring& aArg,
 std::vector<iface::mathml_dom::MathMLElement*>& aArgs,
 std::vector<iface::mathml_dom::MathMLBvarElement*>& aBvars,
 iface::mathml_dom::MathMLElement* degree,
 iface::mathml_dom::MathMLElement* logbase
)
{
  wchar_t buf[30];
  swprintf(buf, 30, L"%lu", aArgs.size());
  mActive += buf;
}

void
CDAMaLaESResult::appendExprs
(
 const std::wstring& aArg,
 std::vector<iface::mathml_dom::MathMLElement*>& aArgs,
 std::vector<iface::mathml_dom::MathMLBvarElement*>& aBvars,
 iface::mathml_dom::MathMLElement* degree,
 iface::mathml_dom::MathMLElement* logbase
)
{
  std::vector<iface::mathml_dom::MathMLElement*>::iterator i;
  bool first = true;
  for (i = aArgs.begin(); i != aArgs.end(); i++)
  {
    if (first)
      first = false;
    else
      mActive += aArg;
    mTransform->RunTransformOnOperator(this, *i);
  }
}

void
CDAMaLaESResult::appendExpr
(
 const std::wstring& aArg,
 std::vector<iface::mathml_dom::MathMLElement*>& aArgs,
 std::vector<iface::mathml_dom::MathMLBvarElement*>& aBvars,
 iface::mathml_dom::MathMLElement* degree,
 iface::mathml_dom::MathMLElement* logbase
)
{
  uint32_t idx = wcstoul(aArg.c_str(), NULL, 10);
  mTransform->RunTransformOnOperator(this, aArgs[idx - 1]);
}

void
CDAMaLaESResult::appendDegree
(
 const std::wstring& aArg,
 std::vector<iface::mathml_dom::MathMLElement*>& aArgs,
 std::vector<iface::mathml_dom::MathMLBvarElement*>& aBvars,
 iface::mathml_dom::MathMLElement* degree,
 iface::mathml_dom::MathMLElement* logbase
)
{
  if (degree == NULL)
    mActive += L"1";
  else
    mTransform->RunTransformOnOperator(this, degree);
}

void
CDAMaLaESResult::appendLogbase
(
 const std::wstring& aArg,
 std::vector<iface::mathml_dom::MathMLElement*>& aArgs,
 std::vector<iface::mathml_dom::MathMLBvarElement*>& aBvars,
 iface::mathml_dom::MathMLElement* degree,
 iface::mathml_dom::MathMLElement* logbase
)
{
  if (logbase == NULL)
    mActive += L"10";
  else
    mTransform->RunTransformOnOperator(this, logbase);
}

void
CDAMaLaESResult::appendBvarIndex
(
 const std::wstring& aArg,
 std::vector<iface::mathml_dom::MathMLElement*>& aArgs,
 std::vector<iface::mathml_dom::MathMLBvarElement*>& aBvars,
 iface::mathml_dom::MathMLElement* degree,
 iface::mathml_dom::MathMLElement* logbase
)
{
  // XXX what about multivariate bvars?
  if (aBvars.size() < 1)
    throw MaLaESError(L"No bound variable found, but language transform requires it.");

  if (aBvars[0]->nArguments() < 1)
    throw MaLaESError(L"Bvar element has no arguments (invalid).");
  
  RETURN_INTO_OBJREF(arg, iface::mathml_dom::MathMLElement,
                     aBvars[0]->getArgument(1));

  DECLARE_QUERY_INTERFACE_OBJREF(ci, arg, mathml_dom::MathMLCiElement);
  if (ci == NULL)
    throw MaLaESError(L"Bvar argument was not a ci (invalid).");

  // We don't want an actual conversion here, so we just ignore the factors.
  boundVariable = true;
  double offset;
  startConversionMode(ci, offset);
  writeConvertedVariable();
  endConversionMode();

  RETURN_INTO_WSTRING(expr,
                      mAnnos->getStringAnnotation(processingVariable,
                                                  L"bvarIndex"));
  mActive += expr;
}

void
CDAMaLaESResult::appendDiffVariable
(
 const std::wstring& aArg,
 std::vector<iface::mathml_dom::MathMLElement*>& aArgs,
 std::vector<iface::mathml_dom::MathMLBvarElement*>& aBvars,
 iface::mathml_dom::MathMLElement* degree,
 iface::mathml_dom::MathMLElement* logbase
)
{
  if (aBvars.size() < 1)
    throw MaLaESError(L"No bound variable found, but language transform requires it.");
  if (aArgs.size() < 1)
    throw MaLaESError(L"No argument found when taking the derivative, but "
                      L"language transform requires it.");

  if (aBvars[0]->nArguments() < 1)
    throw MaLaESError(L"Bvar element has no arguments (invalid).");
  
  RETURN_INTO_OBJREF(arg, iface::mathml_dom::MathMLElement,
                     aBvars[0]->getArgument(1));

  DECLARE_QUERY_INTERFACE_OBJREF(bvci, arg, mathml_dom::MathMLCiElement);
  if (bvci == NULL)
    throw MaLaESError(L"Bvar argument was not a ci (invalid).");

  // XXX what about things like:
  /*
    <apply><diff/>
      <bvar><ci>x</ci></bvar>
      <apply><power/><ci>y</ci><cn cellml:units="dimensionless">2</cn></apply>
    </apply> ?
  */
  DECLARE_QUERY_INTERFACE_OBJREF(ci, aArgs[0], mathml_dom::MathMLCiElement);
  if (ci == NULL)
    throw MaLaESError(L"Sorry, MaLaES currently can only take the "
                      L"derivative of variables.");

  int deg = 1;
  if (degree != NULL)
  {
    DECLARE_QUERY_INTERFACE_OBJREF(dcn, degree, mathml_dom::MathMLCnElement);
    if (dcn == NULL)
      throw MaLaESError(L"Sorry, only constant diff degrees are supported.");
    deg = (uint32_t)parseConstant(dcn);
  }

  double offset;
  boundMup = pow(startConversionMode(bvci, offset, true), deg);
  this->degree = deg;
  if (offset != 0.0)
    throw MaLaESError(L"You can't use a variable with offset units "
                         L"as the bound variable.");

  mTransform->WriteConversion(this, ci);
}

void
CDAMaLaESResult::pushSupplement
(
 const std::wstring& aArg,
 std::vector<iface::mathml_dom::MathMLElement*>& aArgs,
 std::vector<iface::mathml_dom::MathMLBvarElement*>& aBvars,
 iface::mathml_dom::MathMLElement* degree,
 iface::mathml_dom::MathMLElement* logbase
)
{
  mInactive.push_back(mActive);
  mActive = L"";
}

void
CDAMaLaESResult::popSupplement
(
 const std::wstring& aArg,
 std::vector<iface::mathml_dom::MathMLElement*>& aArgs,
 std::vector<iface::mathml_dom::MathMLBvarElement*>& aBvars,
 iface::mathml_dom::MathMLElement* degree,
 iface::mathml_dom::MathMLElement* logbase
)
{
  mSupplementaries.push_back(mActive);
  mActive = mInactive.back();
  mInactive.pop_back();
}

void
CDAMaLaESResult::appendUnique
(
 const std::wstring& aArg,
 std::vector<iface::mathml_dom::MathMLElement*>& aArgs,
 std::vector<iface::mathml_dom::MathMLBvarElement*>& aBvars,
 iface::mathml_dom::MathMLElement* degree,
 iface::mathml_dom::MathMLElement* logbase
)
{
  // See if the unique has been assigned already for the arg...
  uint32_t unique;
  std::map<std::wstring, uint32_t>::iterator i = mUniqueAssignments.find(aArg);
  
  if (i == mUniqueAssignments.end())
  {
    unique = ++mLastUnique;
    mUniqueAssignments.insert(std::pair<std::wstring, uint32_t>(aArg, unique));
  }
  else
  {
    unique = (*i).second;
  }

  wchar_t buf[30];
  swprintf(buf, 30, L"%lu", unique);
  mActive += buf;
}

double
CDAMaLaESResult::parseConstant
(
 iface::mathml_dom::MathMLCnElement* cnEl
)
{
  RETURN_INTO_OBJREF(n, iface::dom::Node, cnEl->getArgument(1));
  DECLARE_QUERY_INTERFACE_OBJREF(t, n, dom::Text);
  RETURN_INTO_WSTRING(txt, t->data());
  int i = 0, j = txt.length() - 1;
  wchar_t c;
  while ((c = txt[i]) == ' ' || c == '\t' || c == '\r' || c == '\n')
    i++;
  while ((c = txt[j]) == ' ' || c == '\t' || c == '\r' || c == '\n')
    j--;
  if (j < i)
    throw MaLaESError(L"CN element with only spaces inside.");
  txt = txt.substr(i, j - i + 1);

  return wcstod(txt.c_str(), NULL);
}

void
CDAMaLaESResult::appendConstant
(
 iface::mathml_dom::MathMLCnElement* cnEl
)
{
  double v = parseConstant(cnEl);
  wchar_t buf[30];
  swprintf(buf, 30, L"%g", v);
  mActive += buf;
}

uint32_t
CDAMaLaESResult::getDiffDegree(iface::cellml_api::CellMLVariable* aVar)
  throw(std::exception&)
{
  std::map<iface::cellml_api::CellMLVariable*, uint32_t>::iterator i = 
    mHighestDegree.find(aVar);
  if (i == mHighestDegree.end())
    return 0;
  return (*i).second;
}

CDAMaLaESTransform::CDAMaLaESTransform(const wchar_t* aSpec)
  : _cda_refcount(1), mVariablesFromSource(true)
{
  stringpairlist tags;
  GetTagsForSpec(aSpec, tags);

  // Go through the tags and process them...
  stringpairlist::iterator i;
  for (i = tags.begin(); i != tags.end(); i++)
  {
    if ((*i).first == L"opengroup")
      openGroup = (*i).second;
    else if ((*i).first == L"closegroup")
      closeGroup = (*i).second;
    else if ((*i).first == L"locally_annotated")
      mVariablesFromSource = true;
    else
    {
      try
      {
        AddOperator((*i).first, (*i).second);
      }
      catch (MaLaESError& e)
      {
        OpError(e.message.c_str(), (*i).first);
      }
    }
  }
}

CDAMaLaESTransform::~CDAMaLaESTransform()
{
}

wchar_t*
CDAMaLaESTransform::compileErrors()
  throw(std::exception&)
{
  return CDA_wcsdup(mMessage.c_str());
}

iface::cellml_services::MaLaESResult*
CDAMaLaESTransform::transform
(
 iface::cellml_services::CeVAS* aCeVAS,
 iface::cellml_services::CUSES* aCUSES,
 iface::cellml_services::AnnotationSet* aAnnos,
 iface::mathml_dom::MathMLElement* aMathML,
 iface::cellml_api::CellMLElement* aContext,
 iface::cellml_api::CellMLVariable* aUnitsOf,
 iface::cellml_api::CellMLVariable* aBoundUnitsOf,
 uint32_t aUnitsDiffDegree
)
  throw(std::exception&)
{
  RETURN_INTO_OBJREF(r, CDAMaLaESResult,
                     new CDAMaLaESResult(this, aCeVAS, aCUSES, aAnnos,
                                         aContext, mVariablesFromSource));
  try
  {
    double mup = 1.0;
    double offset = 0.0;

    if (aUnitsOf != NULL)
    {
      RETURN_INTO_WSTRING(unLocal, aUnitsOf->unitsName());
      RETURN_INTO_OBJREF(compLocal, iface::cellml_api::CellMLElement,
                         aUnitsOf->parentElement());
      RETURN_INTO_OBJREF(curLocal,
                         iface::cellml_services::CanonicalUnitRepresentation,
                         aCUSES->getUnitsByName(compLocal, unLocal.c_str()));
      RETURN_INTO_OBJREF(cvs, iface::cellml_services::ConnectedVariableSet,
                         aCeVAS->findVariableSet(aUnitsOf));
      RETURN_INTO_OBJREF(sv, iface::cellml_api::CellMLVariable,
                         cvs->sourceVariable());
      if (sv == NULL)
        throw MaLaESError(L"Can't find source variable of units conversion target");

      RETURN_INTO_WSTRING(unTarg, sv->unitsName());
      RETURN_INTO_OBJREF(compTarg, iface::cellml_api::CellMLElement,
                         sv->parentElement());
      RETURN_INTO_OBJREF(curTarg,
                         iface::cellml_services::CanonicalUnitRepresentation,
                         aCUSES->getUnitsByName(compTarg, unTarg.c_str()));
      mup = curLocal->convertUnits(curTarg, &offset);
    }

    if (aUnitsDiffDegree != 0 && aBoundUnitsOf != NULL)
    {
      RETURN_INTO_WSTRING(unLocal, aBoundUnitsOf->unitsName());
      RETURN_INTO_OBJREF(compLocal, iface::cellml_api::CellMLElement,
                         aBoundUnitsOf->parentElement());
      RETURN_INTO_OBJREF(curLocal,
                         iface::cellml_services::CanonicalUnitRepresentation,
                         aCUSES->getUnitsByName(compLocal, unLocal.c_str()));
      RETURN_INTO_OBJREF(cvs, iface::cellml_services::ConnectedVariableSet,
                         aCeVAS->findVariableSet(aBoundUnitsOf));
      RETURN_INTO_OBJREF(sv, iface::cellml_api::CellMLVariable,
                         cvs->sourceVariable());
      if (sv == NULL)
        throw MaLaESError(L"Can't find source variable of units conversion target");

      RETURN_INTO_WSTRING(unTarg, sv->unitsName());
      RETURN_INTO_OBJREF(compTarg, iface::cellml_api::CellMLElement,
                         sv->parentElement());
      RETURN_INTO_OBJREF(curTarg,
                         iface::cellml_services::CanonicalUnitRepresentation,
                         aCUSES->getUnitsByName(compTarg, unTarg.c_str()));
      double tmp;
      mup /= pow(curLocal->convertUnits(curTarg, &tmp), (double)aUnitsDiffDegree);
    }

    if (mup == 1.0 && offset == 0.0)
      RunTransformOnOperator(r, aMathML);
    else
    {
      CleanupVector<iface::mathml_dom::MathMLElement*> args;
      CleanupVector<iface::mathml_dom::MathMLBvarElement*> bvars;
      aMathML->add_ref();
      args.push_back(aMathML);
      RETURN_INTO_OBJREF(doc, iface::dom::Document, aMathML->ownerDocument());
      if (mup != 1.0)
      {
        RETURN_INTO_OBJREF(cn, iface::dom::Element,
                           doc->createElementNS(MATHML_NS, L"cn"));
        wchar_t buf[30];
        swprintf(buf, 30, L"%g", mup);
        RETURN_INTO_OBJREF(t, iface::dom::Text,
                           doc->createTextNode(buf));
        cn->appendChild(t)->release_ref();
        DECLARE_QUERY_INTERFACE(mcn, cn,
                                mathml_dom::MathMLCnElement);
        args.push_back(mcn);
      }
      if (offset != 0.0)
      {
        RETURN_INTO_OBJREF(cn, iface::dom::Element,
                           doc->createElementNS(MATHML_NS, L"cn"));
        wchar_t buf[30];
        swprintf(buf, 30, L"%g", offset);
        RETURN_INTO_OBJREF(t, iface::dom::Text,
                           doc->createTextNode(buf));
        cn->appendChild(t)->release_ref();
        DECLARE_QUERY_INTERFACE(mcn, cn,
                                mathml_dom::MathMLCnElement);
        args.push_back(mcn);
      }
      const wchar_t* pseudo;
      if (mup == 1.0)
        pseudo = L"units_conversion_offset";
      else if (offset == 0.0)
        pseudo = L"units_conversion_factor";
      else
        pseudo = L"units_conversion";
      // Apply the pseudo-operator...
      ExecuteTransform(r, pseudo, args, bvars, NULL, NULL);
    }
  }
  catch (MaLaESError& mError)
  {
    r->gotError(mError.message);
  }

  r->finishTransform();
  r->add_ref();
  return r;
}

void
CDAMaLaESTransform::AddOperator
(
 const std::wstring& aOpName,
 const std::wstring& aOpValue
)
{
  // Casting away constness is unfortunate, but wcstoul needs it.
  wchar_t* p = const_cast<wchar_t*>(aOpValue.c_str());
  uint32_t precInner = 0, precOuter = 1000;
  if (p[0] == L'#' && p[1] == L'p' && p[2] == L'r' && p[3] == L'e' &&
      p[4] == L'c' && p[5] == L'[')
  {
    p += 6;
    if (*p == L'H')
    {
      precOuter = 0;
      precInner = 1000;
      p++;
    }
    else
    {
      precOuter = wcstoul(p, &p, 10);
      if (*p == L'(')
      {
        precInner = wcstoul(p + 1, &p, 10);
        if (*p++ != L')')
        {
          OpError(L"Expected ) after #prec definition.", aOpName);
          return;
        }
      }
      else
        precInner = precOuter;
    }
    if (*p != L']')
    {
      OpError(L"Expected ] after #prec definition.", aOpName);
      return;
    }
    p++;
  }
  
  commandlist program;
  wchar_t* p2;
  bool needPopSupplement = false;
  int maxarg = 0;
  while (*p != 0)
  {
    if (p[0] == L'#')
    {
      p++;
      for (p2 = p; ; p2++)
      {
        wchar_t c = *p2;
        if ((c >= L'A' && c <= L'Z') ||
            (c >= L'a' && c <= L'z') ||
            (c >= L'0' && c <= L'9'))
          ;
        else
          break;
      }
      std::wstring cmd(p, p2 - p);
      std::wstring arg;
      p = p2;
      if (*p == L'[')
      {
        p++;
        p2 = wcschr(p, L']');
        if (p2 == NULL)
        {
          OpError(L"Expected ] after # command operand.", aOpName);
          return;
        }
        arg = std::wstring(p, p2 - p);
        p = p2 + 1;
      }

      if (cmd == L"popsupplement")
      {
        OpError(L"popsupplement is an internal command only.", aOpName);
        return;
      }

      if (cmd == L"supplement")
      {
        if (needPopSupplement)
          AppendCommandToProgram(program, L"popsupplement", L"", maxarg);
        needPopSupplement = true;
      }
      AppendCommandToProgram(program, cmd, arg, maxarg);
    }

    p2 = wcschr(p, L'#');
    if (p2 == NULL)
    {
      program.push_back(command(&CDAMaLaESResult::appendString, p));
      break;
    }
    else
    {
      std::wstring t(p, p2 - p);
      program.push_back(command(&CDAMaLaESResult::appendString, t));
      p = p2;
    }
  }
  if (needPopSupplement)
    AppendCommandToProgram(program, L"popsupplement", L"", maxarg);
  operMap.insert(std::pair<std::wstring, Operator>
                 (aOpName, Operator(program, precInner, precOuter, maxarg)));
}

void
CDAMaLaESTransform::AppendCommandToProgram
(
 commandlist& aProgram,
 const std::wstring& aCmd,
 const std::wstring& aArg,
 int& maxarg
)
{
  wchar_t c = aCmd[0];
  if (c <= L'e')
  {
    if (c == L'e')
    {
      if (aCmd == L"exprs")
      {
        aProgram.push_back(command(&CDAMaLaESResult::appendExprs, aArg));
        if (maxarg > 0)
          throw MaLaESError(L"MAL file contains #exprs and #expri in the same tag.");
      
        maxarg = -1;
        return;
      }
      else if (aCmd.substr(0, 4) == L"expr")
      {
        std::wstring arg = aCmd.substr(4);
        aProgram.push_back(command(&CDAMaLaESResult::appendExpr, arg));
        uint32_t iarg = wcstoul(arg.c_str(), NULL, 10);
        if (maxarg == -1)
          throw MaLaESError(L"MAL file contains #exprs and #expri in the same tag.");
        if ((int)iarg > maxarg)
          maxarg = iarg;
        return;
      }
    }
    else if (c == L'd')
    {
      if (aCmd == L"degree")
      {
        aProgram.push_back(command(&CDAMaLaESResult::appendDegree, aArg));
        return;
      }
    }
    else if (c == L'c')
    {
      if (aCmd == L"count")
      {
        aProgram.push_back(command(&CDAMaLaESResult::appendCount, aArg));
        return;
      }
    }
    else if (c == L'b')
    {
      if (aCmd == L"bvarIndex")
      {
        aProgram.push_back(command(&CDAMaLaESResult::appendBvarIndex, aArg));
        return;
      }
    }
  }
  else if (c == L'l')
  {
    if (aCmd == L"logbase")
    {
      aProgram.push_back(command(&CDAMaLaESResult::appendLogbase, aArg));
      return;
    }
    else if (aCmd == L"lookupDiffVariable")
    {
      maxarg = 1;
      aProgram.push_back(command(&CDAMaLaESResult::appendDiffVariable, aArg));
      return;
    }
  }
  else if (c == L'p')
  {
    if (aCmd == L"popsupplement")
    {
      aProgram.push_back(command(&CDAMaLaESResult::popSupplement, aArg));
      return;
    }
  }
  else if (c == L's')
  {
    if (aCmd == L"supplement")
    {
      aProgram.push_back(command(&CDAMaLaESResult::pushSupplement, aArg));
      return;
    }
  }
  else if (aCmd.substr(0, 6) == L"unique")
  {
    std::wstring arg = aCmd.substr(6);
    aProgram.push_back(command(&CDAMaLaESResult::appendUnique, arg));
    return;
  }

  std::wstring msg = L"Unknown or unsupported # command: ";
  msg += aCmd;
  throw MaLaESError(msg);
}

void
CDAMaLaESTransform::GetTagsForSpec(const wchar_t* aSpec, stringpairlist& aTags)
{
  const wchar_t* idx = aSpec;
  enum
  {
    EXPECT_TAG,
    IN_COMMENT,
    IN_TAG,
    EXPECT_VALUE_SPACE,
    IN_VALUE,
    MAYBE_CONT,
    MAYBE_CONT_2
  } state = EXPECT_TAG;

  std::wstring tmpn, tmpv, contv;
  size_t lineno = 1;
  for (; *idx != 0; idx++)
  {
    wchar_t c = *idx;
    if (c == L'\n')
      lineno++;
    switch (state)
    {
    case EXPECT_TAG:
      if (c == L'\n' || c == L'\r')
        continue;
      if (c == L'#')
      {
        state = IN_COMMENT;
        continue;
      }
      if ((c >= L'A' && c <= L'Z') || (c >= L'a' && c <= L'z') ||
          (c >= L'0' && c <= L'9') || (c == L'_'))
      {
        state = IN_TAG;
        tmpn += c;
        continue;
      }
      ParseError(L"Invalid character at beginning of line.", lineno);
      return;

    case IN_COMMENT:
      if (c == L'\n' || c == L'\r')
        state = EXPECT_TAG;
      continue;

    case IN_TAG:
      if (c == L'\r' || c == L'\n')
      {
        ParseError(L"Expecting a : before the end of the line.", lineno);
        return;
      }
      if (c == L':')
      {
        state = EXPECT_VALUE_SPACE;
        continue;
      }
      if ((c >= L'A' && c <= L'Z') || (c >= L'a' && c <= L'z') ||
          (c >= L'0' && c <= L'9') || (c == L'_') || (c == L'.') ||
          (c == L'-') || (c == L'/'))
      {
        tmpn += c;
        continue;
      }
      ParseError(L"Invalid character in tag name.", lineno);
      return;

    case EXPECT_VALUE_SPACE:
      if (c == L'\r' || c == L'\n')
      {
        contv = L"";
        state = MAYBE_CONT;
        contv += c;
        continue;
      }

      if (c == L' ')
      {
        state = IN_VALUE;
        continue;
      }

      tmpn += L':';
      tmpn += c;
      state = IN_TAG;
      continue;

    case IN_VALUE:
      if (c == L'\r' || c == L'\n')
      {
        contv = L"";
        state = MAYBE_CONT;
        contv += c;
        continue;
      }

      tmpv += c;
      continue;

    case MAYBE_CONT:
      if (c == L' ')
      {
        state = MAYBE_CONT_2;
        continue;
      }

      aTags.push_back(std::pair<std::wstring,std::wstring>(tmpn, tmpv));
      tmpn = L"";
      tmpv = L"";

      // Retry in EXPECT_TAG instead.
      state = EXPECT_TAG;
      idx--;
      continue;

    case MAYBE_CONT_2:
      if (c != L' ')
      {
        ParseError(L"Continuations need two spaces.", lineno);
        return;
      }
      tmpv += contv;
      state = IN_VALUE;
      continue;
    }
  }

  switch (state)
  {
  case EXPECT_TAG:
    break;
  case IN_COMMENT:
    break;
  case IN_TAG:
    ParseError(L"Unexpected end of format specification.", lineno);
    return;
  case EXPECT_VALUE_SPACE:
    ParseError(L"Unexpected end of format specification.", lineno);
    return;
  case IN_VALUE:
    aTags.push_back(std::pair<std::wstring, std::wstring>(tmpn, tmpv));
    break;
  case MAYBE_CONT:
    break;
  case MAYBE_CONT_2:
    ParseError(L"Unexpected space at end of format specification.", lineno);
    return;
  }
}

void
CDAMaLaESTransform::ParseError(const wchar_t* msg, size_t lineno)
{
  std::wstringstream wss;
  wss << L"Parse error in MAL specification line " << lineno
      << L": " << msg << L"; ";
  mMessage += wss.str();
}

void
CDAMaLaESTransform::OpError(const wchar_t* msg, const std::wstring& op)
{
  mMessage += L"Invalid MAL operator definition ";
  mMessage += op;
  mMessage += L": ";
  mMessage += msg;
  mMessage += L"; ";
}

void
CDAMaLaESTransform::WriteConversion
(
 CDAMaLaESResult* aResult,
 iface::mathml_dom::MathMLCiElement* ci
)
{
  CleanupVector<iface::mathml_dom::MathMLBvarElement*> bvars;
  CleanupVector<iface::mathml_dom::MathMLElement*> args;

  double mup, offset;
  mup = aResult->startConversionMode(ci, offset);
  if (mup == 1.0 && offset == 0.0)
  {
    aResult->writeConvertedVariable();
    aResult->endConversionMode();
    return;
  }

  ci->add_ref();
  
  // Next time we will go to the writeConvertedVariable path instead.
  args.push_back(ci);
  
  RETURN_INTO_OBJREF(doc, iface::dom::Document, ci->ownerDocument());
  if (mup != 1.0)
  {
    RETURN_INTO_OBJREF(cn, iface::dom::Element,
                       doc->createElementNS(MATHML_NS, L"cn"));
    wchar_t buf[30];
    swprintf(buf, 30, L"%g", mup);
    RETURN_INTO_OBJREF(t, iface::dom::Text,
                       doc->createTextNode(buf));
    cn->appendChild(t)->release_ref();
    DECLARE_QUERY_INTERFACE(mcn, cn,
                            mathml_dom::MathMLCnElement);
    args.push_back(mcn);
  }
  if (offset != 0.0)
  {
    RETURN_INTO_OBJREF(cn, iface::dom::Element,
                       doc->createElementNS(MATHML_NS, L"cn"));
    wchar_t buf[30];
    swprintf(buf, 30, L"%g", offset);
    RETURN_INTO_OBJREF(t, iface::dom::Text,
                       doc->createTextNode(buf));
    cn->appendChild(t)->release_ref();
    DECLARE_QUERY_INTERFACE(mcn, cn,
                            mathml_dom::MathMLCnElement);
    args.push_back(mcn);
  }
  
  const wchar_t* pseudo;
  if (mup == 1.0)
    pseudo = L"units_conversion_offset";
  else if (offset == 0.0)
    pseudo = L"units_conversion_factor";
  else
    pseudo = L"units_conversion";
  
  // Apply the pseudo-operator...
  ExecuteTransform(aResult, pseudo, args, bvars, NULL, NULL);
  
  aResult->endConversionMode();
}

void
CDAMaLaESTransform::RunTransformOnOperator
(
 CDAMaLaESResult* aResult, iface::mathml_dom::MathMLElement* aEl
)
{
  CleanupVector<iface::mathml_dom::MathMLBvarElement*> bvars;
  CleanupVector<iface::mathml_dom::MathMLElement*> args;

  DECLARE_QUERY_INTERFACE_OBJREF(ci, aEl, mathml_dom::MathMLCiElement);
  if (ci != NULL)
  {
    if (aResult->writeConvertedVariable())
      return;

    WriteConversion(aResult, ci);
    return;
  }

  DECLARE_QUERY_INTERFACE_OBJREF(cn, aEl, mathml_dom::MathMLCnElement);
  if (cn != NULL)
  {
    aResult->appendConstant(cn);
    return;
  }

  DECLARE_QUERY_INTERFACE_OBJREF(pw, aEl, mathml_dom::MathMLPiecewiseElement);
  if (pw != NULL)
  {
    uint32_t i;
    std::wstring sn = L"piecewise_first_case";
    for (i = 1; ; i++)
    {
      RETURN_INTO_OBJREF(pwc, iface::mathml_dom::MathMLCaseElement,
                         pw->getCase(i));
      if (pwc == NULL)
        break;
      
      CleanupVector<iface::mathml_dom::MathMLElement*> tmpargs;
      tmpargs.push_back(pwc->caseCondition());
      tmpargs.push_back(pwc->caseValue());
      ExecuteTransform(aResult, sn, tmpargs, bvars, NULL, NULL);
      if (i == 1)
        sn = L"piecewise_extra_case";
    }

    try
    {
      RETURN_INTO_OBJREF(pwo, iface::mathml_dom::MathMLContentElement,
                         pw->otherwise());

      args.push_back(pwo);
      pwo->add_ref();

      std::wstring sn = L"piecewise_otherwise";
      ExecuteTransform(aResult, sn, args, bvars, NULL, NULL);
    }
    catch (iface::dom::DOMException& de)
    {
      std::wstring sn = L"piecewise_no_otherwise";
      ExecuteTransform(aResult, sn, args, bvars, NULL, NULL);
    }

    return;
  }

  DECLARE_QUERY_INTERFACE_OBJREF(apply, aEl, mathml_dom::MathMLApplyElement);

  if (apply == NULL)
  {
    DECLARE_QUERY_INTERFACE_OBJREF(pds, aEl, mathml_dom::MathMLPredefinedSymbol);
    if (pds == NULL)
      throw MaLaESError(L"Only apply, cn, ci, constants, and piecewise are supported.");
    RETURN_INTO_WSTRING(sn, pds->symbolName());
    ExecuteTransform(aResult, sn, args, bvars, NULL, NULL);
    return;
  }

  RETURN_INTO_OBJREF(op, iface::mathml_dom::MathMLElement,
                     apply->_cxx_operator());
  DECLARE_QUERY_INTERFACE_OBJREF(csym, op, mathml_dom::MathMLCsymbolElement);
  std::wstring opName;

  ObjRef<iface::mathml_dom::MathMLElement> degree, logbase;

  RETURN_INTO_OBJREF(nl, iface::dom::NodeList, apply->childNodes());
  uint32_t l = nl->length(), i;
  for (i = 0; i < l; i++)
  {
    RETURN_INTO_OBJREF(node, iface::dom::Node, nl->item(i));
    DECLARE_QUERY_INTERFACE_OBJREF(el, node, dom::Element);
    if (el == NULL)
      continue;
    RETURN_INTO_WSTRING(nsURI, el->namespaceURI());
    if (nsURI != MATHML_NS)
      continue;

    RETURN_INTO_WSTRING(ln, el->localName());
    if (ln == L"bvar")
    {
      DECLARE_QUERY_INTERFACE_OBJREF(bv, el, mathml_dom::MathMLBvarElement);
      bv->add_ref();
      bvars.push_back(bv);
    }
    else if (ln == L"degree")
    {
      DECLARE_QUERY_INTERFACE_OBJREF(degreeC, el, mathml_dom::MathMLContainer);
      if (degreeC->nArguments() < 1)
        throw MaLaESError(L"Found a degree with no argument (invalid)");
      degree = already_AddRefd<iface::mathml_dom::MathMLElement>
        (degreeC->getArgument(1));
    }
    else if (ln == L"logbase")
    {
      DECLARE_QUERY_INTERFACE_OBJREF(logbaseC, el,
                                     mathml_dom::MathMLContentContainer);
      if (logbaseC->nArguments() < 1)
        throw MaLaESError(L"Found a logbase with no argument (invalid)");
      logbase = already_AddRefd<iface::mathml_dom::MathMLElement>
        (logbaseC->getArgument(1));
    }
  }

  if (csym != NULL)
  {
    wchar_t* str = csym->definitionURL();
    opName = str;
    free(str);
    aResult->setInvolvesExternalCode();
  }
  else
  {
    DECLARE_QUERY_INTERFACE_OBJREF(pds, op, mathml_dom::MathMLPredefinedSymbol);
    if (pds == NULL)
      throw MaLaESError(L"Found a MathML apply with an invalid operator.");
    wchar_t* str = pds->symbolName();
    opName = str;
    free(str);
  }

  l = apply->nArguments();
  for (i = 2; i <= l; i++)
    args.push_back(apply->getArgument(i));

  ExecuteTransform(aResult, opName, args, bvars, degree, logbase);
}

void
CDAMaLaESTransform::ExecuteTransform
(
 CDAMaLaESResult* aResult,
 const std::wstring& aOpName,
 std::vector<iface::mathml_dom::MathMLElement*>& args,
 std::vector<iface::mathml_dom::MathMLBvarElement*>& bvars,
 iface::mathml_dom::MathMLElement* degree,
 iface::mathml_dom::MathMLElement* logbase
)
{
  std::wstring opName(aOpName);

  // This special case is ugly, but it is rooted in the fact that MathML has
  // both unary and binary minus as a single operator.
  if (aOpName == L"minus" && args.size() == 1)
    opName = L"unary_minus";

  size_t idx;
  if ((idx = aOpName.find(L'#')) != std::wstring::npos)
  {
    opName = aOpName.substr(0, idx);
  }

  // Look up the operator...
  std::map<std::wstring, Operator>::iterator omi = operMap.find(opName);
  if (omi == operMap.end())
  {
    std::wstring msg = L"Language description has no rules to deal with "
      L"MathML operator ";
    msg += opName;
    throw MaLaESError(msg);
  }

  Operator& o = (*omi).second;

  if (o.maxarg != -1 && o.maxarg != (int)args.size())
  {
    std::wstring msg = L"Language description requires that MathML operator ";
    msg += opName;
    msg += L" has ";
    wchar_t buf[30];
    swprintf(buf, 30, L"%u", o.maxarg);
    msg += buf;
    msg += L" arguments, but it actually has ";
    swprintf(buf, 30, L"%u", args.size());
    msg += buf;
    throw MaLaESError(msg);
  }

  bool needGroup = aResult->pushPrecedence(o.precOuter, o.precInner);

  // Open group...
  if (needGroup)
    aResult->appendString(openGroup, args, bvars, degree, logbase);

  // Now run the append program we compiled earlier...
  commandlist::iterator pri;
  for (pri = o.program.begin(); pri != o.program.end(); pri++)
    (aResult->* ((*pri).first)) ((*pri).second, args, bvars, degree, logbase);

  // Close group...
  if (needGroup)
    aResult->appendString(closeGroup, args, bvars, degree, logbase);
  aResult->popPrecedence();
}

CDAMaLaESBootstrap::CDAMaLaESBootstrap()
  : _cda_refcount(1)
{
}

CDAMaLaESBootstrap::~CDAMaLaESBootstrap()
{
}

iface::cellml_services::MaLaESTransform*
CDAMaLaESBootstrap::compileTransformer(const wchar_t* aSpec)
  throw(std::exception&)
{
  return new CDAMaLaESTransform(aSpec);
}

iface::cellml_services::MaLaESBootstrap*
CreateMaLaESBootstrap()
{
  return new CDAMaLaESBootstrap();
}
