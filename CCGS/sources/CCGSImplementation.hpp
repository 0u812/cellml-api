#ifndef _CCGSImplementation_hpp
#define _CCGSImplementation_hpp
#include "cda_compiler_support.h"
#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif
#include <exception>
#include "IfaceCellML_APISPEC.hxx"
#include "IfaceMathML_content_APISPEC.hxx"
#include "IfaceCCGS.hxx"
#include "Utilities.hxx"
#include <sstream>
#include <vector>
#include <list>

#ifdef ENABLE_CONTEXT
#include "IfaceCellML_Context.hxx"
#endif

class CDA_CodeInformation;

class CDA_ComputationTarget
  : public iface::cellml_services::ComputationTarget
{
  ptr_tag<CDA_ComputationTarget> self;
public:
  CDA_IMPL_ID;
  CDA_IMPL_REFCOUNT;
  CDA_IMPL_QI1(cellml_services::ComputationTarget);

  CDA_ComputationTarget() : self(this), _cda_refcount(1), mDegree(0), mAssignedIndex(0),
                            mIsReset(false),
                            mEvaluationType(iface::cellml_services::FLOATING),
                            mInfDelayedAssignedIndex(-1)
  {};
  ~CDA_ComputationTarget() {};

  iface::cellml_api::CellMLVariable* variable() throw();
  uint32_t degree() throw();
  iface::cellml_services::VariableEvaluationType type() throw();
  wchar_t* name() throw();
  uint32_t assignedIndex() throw();

  ptr_tag<CDA_ComputationTarget>& getSelf() { return self; }

  // CCGS implementation access only...
  void setNameAndIndex(uint32_t aIndex, const wchar_t* aName) throw();
  void setDelayedName(const wchar_t* aSetTo) throw();
  ObjRef<iface::cellml_api::CellMLVariable> mVariable;
  ObjRef<iface::cellml_services::AnnotationSet> mAnnoSet;
  uint32_t mDegree, mAssignedIndex;
  bool mIsReset;
  iface::cellml_services::VariableEvaluationType mEvaluationType;
  // Only available during code generation...
  ptr_tag<CDA_ComputationTarget> mUpDegree, mDownDegree;
  uint32_t mHighestDegree;
  // -1 if there is no index for the infinitesimally delayed version.
  int32_t mInfDelayedAssignedIndex;
  bool mStateHasIV;

  // Disjoint set utilities...
  uint32_t rank;
  ptr_tag<CDA_ComputationTarget> parent;
  void resetSetMembership()
  {
    rank = 0;
    parent = self;
  }

  ptr_tag<CDA_ComputationTarget> findRoot()
  {
    if (parent == this)
      return self;

    parent = parent->findRoot();
    return parent;
  }

  void unionWith(ptr_tag<CDA_ComputationTarget> b)
  {
    findRoot();
    b->findRoot();

    if (parent == b->parent)
      return;

    if (parent->rank > b->parent->rank)
      b->parent->parent = parent;
    else if (b->parent->rank > parent->rank)
      parent->parent = b->parent;
    else
    {
      b->parent->parent = parent;
      parent->rank++;
    }
  }
};

class CDA_ComputationTargetIterator
  : public iface::cellml_services::ComputationTargetIterator
{
public:
  CDA_IMPL_ID;
  CDA_IMPL_REFCOUNT;
  CDA_IMPL_QI1(cellml_services::ComputationTargetIterator);

  CDA_ComputationTargetIterator
  (
   std::list<ptr_tag<CDA_ComputationTarget> >& aTargets,
   CDA_CodeInformation* aOwner
  ) : _cda_refcount(1), mTargets(aTargets), mTargetsIt(aTargets.begin()),
      mOwner(aOwner) {};
  ~CDA_ComputationTargetIterator() {};

  iface::cellml_services::ComputationTarget* nextComputationTarget() throw();

private:
  // These will not go away before mOwner, so we don't ref them.
  std::list<ptr_tag<CDA_ComputationTarget> >& mTargets;
  std::list<ptr_tag<CDA_ComputationTarget> >::iterator mTargetsIt;
  ObjRef<CDA_CodeInformation> mOwner;
};

class CDA_CodeInformation
  : public iface::cellml_services::CodeInformation
{
public:
  CDA_IMPL_ID;
  CDA_IMPL_REFCOUNT;
  CDA_IMPL_QI1(cellml_services::CodeInformation);

  CDA_CodeInformation() : _cda_refcount(1) {};
  ~CDA_CodeInformation();

  wchar_t* errorMessage() throw();
  iface::cellml_services::ModelConstraintLevel constraintLevel() throw();
  uint32_t algebraicIndexCount() throw();
  uint32_t rateIndexCount() throw();
  uint32_t constantIndexCount() throw();
  wchar_t* initConstsString() throw();
  wchar_t* ratesString() throw();
  wchar_t* variablesString() throw();
  wchar_t* functionsString() throw();
  iface::cellml_services::ComputationTargetIterator* iterateTargets()
    throw();
  iface::mathml_dom::MathMLNodeList* flaggedEquations()
    throw();

  // CCGS implementation access only...
  std::wstring mErrorMessage;
  iface::cellml_services::ModelConstraintLevel mConstraintLevel;
  uint32_t mAlgebraicIndexCount, mRateIndexCount, mConstantIndexCount;
  std::wstring mInitConstsStr, mRatesStr, mVarsStr, mFuncsStr;
  std::list<ptr_tag<CDA_ComputationTarget> > mTargets;
  std::vector<iface::dom::Element*> mFlaggedEquations;
};

class CDA_CodeGenerator
  : public iface::cellml_services::CodeGenerator
{
public:
  CDA_IMPL_ID;
  CDA_IMPL_REFCOUNT;
  CDA_IMPL_QI1(cellml_services::CodeGenerator);

  CDA_CodeGenerator();
  ~CDA_CodeGenerator() {};

  wchar_t* constantPattern() throw();
  void constantPattern(const wchar_t* aPattern) throw();
  wchar_t* stateVariableNamePattern() throw();
  void stateVariableNamePattern(const wchar_t* aPattern) throw();
  wchar_t* algebraicVariableNamePattern() throw();
  void algebraicVariableNamePattern(const wchar_t* aPattern) throw();
  wchar_t* rateNamePattern() throw();
  void rateNamePattern(const wchar_t* aPattern) throw();
  wchar_t* voiPattern(void) throw();
  void voiPattern(const wchar_t* aPattern) throw();
  uint32_t arrayOffset() throw();
  void arrayOffset(uint32_t offset) throw();
  wchar_t* assignPattern() throw();
  void assignPattern(const wchar_t* aPattern) throw();
  wchar_t* solvePattern() throw();
  void solvePattern(const wchar_t* aPattern) throw();
  wchar_t* solveNLSystemPattern() throw();
  void solveNLSystemPattern(const wchar_t* aPattern) throw();
  wchar_t* temporaryVariablePattern() throw();
  void temporaryVariablePattern(const wchar_t* aPattern) throw();
  wchar_t* declareTemporaryPattern() throw();
  void declareTemporaryPattern(const wchar_t* aPattern) throw();
  wchar_t* conditionalAssignmentPattern() throw();
  void conditionalAssignmentPattern(const wchar_t* aPattern) throw();

  iface::cellml_services::MaLaESTransform* transform() throw();
  void transform(iface::cellml_services::MaLaESTransform* aTransform)
     throw();
  iface::cellml_services::CeVAS* useCeVAS()
     throw();
  void useCeVAS(iface::cellml_services::CeVAS* aCeVAS)
     throw();
  iface::cellml_services::CUSES* useCUSES() throw();
  void useCUSES(iface::cellml_services::CUSES* aCUSES) throw();
  iface::cellml_services::AnnotationSet* useAnnoSet() throw();
  void useAnnoSet(iface::cellml_services::AnnotationSet* aAnnoSet) throw();
  iface::cellml_services::CodeInformation* generateCode
    (iface::cellml_api::Model* aSourceModel) throw();
private:
  std::wstring mConstantPattern, mStateVariableNamePattern,
    mAlgebraicVariableNamePattern,
    mRateNamePattern, mVOIPattern, mAssignPattern, mSolvePattern,
    mSolveNLSystemPattern, mTemporaryVariablePattern, mDeclareTemporaryPattern,
    mConditionalAssignmentPattern;
  uint32_t mArrayOffset;
  ObjRef<iface::cellml_services::MaLaESTransform> mTransform;
  ObjRef<iface::cellml_services::CeVAS> mCeVAS;
  ObjRef<iface::cellml_services::CUSES> mCUSES;
  ObjRef<iface::cellml_services::AnnotationSet> mAnnoSet;
};

class CDA_CodeGeneratorBootstrap
  : public iface::cellml_services::CodeGeneratorBootstrap
#ifdef ENABLE_CONTEXT 
  , public iface::cellml_context::CellMLModule
#endif 
{
public:
  CDA_IMPL_ID;
  CDA_IMPL_REFCOUNT;
#ifdef ENABLE_CONTEXT
  CDA_IMPL_QI2(cellml_services::CodeGeneratorBootstrap, cellml_context::CellMLModule);
#else
  CDA_IMPL_QI1(cellml_services::CodeGeneratorBootstrap);
#endif

  CDA_CodeGeneratorBootstrap() : _cda_refcount(1)
#ifdef ENABLE_CONTEXT
                               , mUnload(NULL)
#endif
  {}
  ~CDA_CodeGeneratorBootstrap() {}

  iface::cellml_services::CodeGenerator*
  createCodeGenerator() throw(std::exception&)
  {
    return new CDA_CodeGenerator();
  }

#ifdef ENABLE_CONTEXT
  iface::cellml_context::CellMLModule::ModuleTypes moduleType()
    throw(std::exception&)
  {
    return iface::cellml_context::CellMLModule::SERVICE;
  }

  wchar_t* moduleName() throw (std::exception&)
  {
    return CDA_wcsdup(L"CCCGS");
  }

  wchar_t* moduleDescription() throw (std::exception&)
  {
    return CDA_wcsdup(L"The CellML C Code Generation Service");
  }

  wchar_t* moduleVersion() throw (std::exception&)
  {
    return CDA_wcsdup(L"0.0");
  }

  wchar_t* moduleIconURL() throw (std::exception&)
  {
    return CDA_wcsdup(L"");
  }

  void unload() throw (std::exception&)
  {
    if (mUnload != NULL)
      mUnload();
  }

  void SetUnloadCCGS(void (*unload)(void))
  {
    mUnload = unload;
  }

private:
  void (*mUnload)();
#endif
};

#endif // _CCGSImplementation_hpp
