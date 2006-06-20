#include "IfaceCCGS.hxx"
#include "IfaceCIS.hxx"
#include "ThreadWrapper.hxx"
#include <string>

struct CompiledModelFunctions
{
  void (*SetupFixedConstants)(double* CONSTANTS);
  void (*SetupComputedConstants)(double* CONSTANTS, double* VARIABLES);
  void (*ComputeRates)(double* BOUND, double* RATES, double* CONSTANTS,
                       double* VARIABLES);
  void (*ComputeVariables)(double* BOUND, double* RATES, double* CONSTANTS,
                           double* VARIABLES);
};

class CDA_CellMLCompiledModel
  : public iface::cellml_services::CellMLCompiledModel
{
public:
  CDA_CellMLCompiledModel(
                           void* aModule, CompiledModelFunctions* aCMF,
                           iface::cellml_api::Model* aModel,
                           iface::cellml_services::CCodeInformation* aCCI,
                           std::string& aDirname
                          );
  ~CDA_CellMLCompiledModel();

  CDA_IMPL_REFCOUNT;
  CDA_IMPL_ID;
  CDA_IMPL_QI1(cellml_services::CellMLCompiledModel);

  iface::cellml_api::Model* model()
    throw(std::exception&)
  {
    mModel->add_ref();
    return mModel;
  }

  iface::cellml_services::CCodeInformation* codeInformation()
    throw(std::exception&)
  {
    mCCI->add_ref();
    return mCCI;
  }

  // These are not available directly across CORBA, but read-only access is
  // allowed within the same module...
  void* mModule;
  CompiledModelFunctions* mCMF;
  iface::cellml_api::Model* mModel;
  iface::cellml_services::CCodeInformation* mCCI;
  std::string mDirname;
};

class CDA_CellMLIntegrationRun
  : public iface::cellml_services::CellMLIntegrationRun,
    public CDAThread
{
public:
  CDA_CellMLIntegrationRun(
                           iface::cellml_services::CellMLCompiledModel* aModel
                          );
  ~CDA_CellMLIntegrationRun();

  CDA_IMPL_REFCOUNT;
  CDA_IMPL_ID;
  CDA_IMPL_QI1(cellml_services::CellMLIntegrationRun);

  iface::cellml_services::IntegrationStepType stepType()
    throw (std::exception&);

  void stepType(iface::cellml_services::IntegrationStepType ist)
    throw (std::exception&);

  void setStepSizeControl(double epsAbs, double epsRel, double scalVar,
                          double scalRate) throw (std::exception&);
  void setResultRange(double startBvar, double stopBvar, double incrementBvar)
    throw (std::exception&);
  void setProgressObserver(iface::cellml_services::IntegrationProgressObserver*
                           aIpo)
    throw (std::exception&);
  void setOverride(iface::cellml_services::VariableEvaluationType aType,
                   uint32_t variableIndex, double newValue)
    throw (std::exception&);
  void start() throw (std::exception&);
  void stop() throw (std::exception&);

protected:
  void runthread();

private:
  void SolveODEProblem(CompiledModelFunctions* f, uint32_t constSize,
                       double* constants, uint32_t varSize, double* variables,
                       uint32_t rateSize, double* rates);

  CDA_CellMLCompiledModel* mModel;
  iface::cellml_services::IntegrationStepType mStepType;
  double mEpsAbs, mEpsRel, mScalVar, mScalRate, mStepSizeMax;
  double mStartBvar, mStopBvar, mIncrementBvar;
  iface::cellml_services::IntegrationProgressObserver* mObserver;
  typedef std::list<std::pair<uint32_t,double> > OverrideList;
  OverrideList mConstantOverrides, mIVOverrides;
  bool mCancelIntegration;
};

class CDA_CellMLIntegrationService
  : public iface::cellml_services::CellMLIntegrationService
{
public:
  CDA_CellMLIntegrationService()
    : _cda_refcount(1)
  {
  }

  ~CDA_CellMLIntegrationService()
  {
  }

  CDA_IMPL_REFCOUNT;
  CDA_IMPL_ID;
  CDA_IMPL_QI1(cellml_services::CellMLIntegrationService);
  
  iface::cellml_services::CellMLCompiledModel*
  compileModel(iface::cellml_services::CGenerator* aCG,
               iface::cellml_api::Model* aModel)
    throw(std::exception&);

  iface::cellml_services::CellMLIntegrationRun*
  createIntegrationRun(iface::cellml_services::CellMLCompiledModel* aModel)
    throw(std::exception&);
  
  wchar_t* lastError() throw(std::exception&)
  {
    return CDA_wcsdup(mLastError.c_str());
  }
private:
  std::wstring mLastError;
};
