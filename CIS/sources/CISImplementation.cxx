#include <stdlib.h>
#include <string>
#include "Utilities.hxx"
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/errno.h>
#include "CISImplementation.hxx"
#include <dlfcn.h>
#include <fstream>
#include "CISBootstrap.hpp"
#include <dirent.h>

char*
attempt_make_tempdir(const char* parentDir)
{
  std::string basename = parentDir;
  basename += "/";
  while (true)
  {
#define DICT "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_."
    std::string fn = basename;
    uint32_t i;
    for (i = 0; i < 5; i++)
    {
      uint32_t r = mersenne_genrand_int32();
      fn += DICT[r & 0x3F];
      fn += DICT[(r >> 6) & 0x3F];
      fn += DICT[(r >> 12) & 0x3F];
      fn += DICT[(r >> 18) & 0x3F];
      fn += DICT[(r >> 24) & 0x3F];
    }
    if (mkdir(fn.c_str(), 0700) == 0)
      return strdup(fn.c_str());
    // If the error is EEXIST, we can try again with another name. If not, we
    // should fail.
    if (errno != EEXIST)
      return NULL;
  }
}

CompiledModelFunctions*
SetupCompiledModelFunctions(void* module)
{
  CompiledModelFunctions* cmf = new CompiledModelFunctions;
  cmf->SetupFixedConstants = (void (*)(double*))
    dlsym(module, "SetupFixedConstants");
  cmf->SetupComputedConstants = (void (*)(double*,double*))
    dlsym(module, "SetupComputedConstants");
  cmf->ComputeRates = (void (*)(double*,double*,double*,double*))
    dlsym(module, "ComputeRates");
  cmf->ComputeVariables = (void (*)(double*,double*,double*,double*))
    dlsym(module, "ComputeVariables");
  return cmf;
}

void*
CompileSource(std::string& destDir, std::string& sourceFile,
              std::wstring& lastError)
{
  std::string targ = destDir;
  targ += "/generated.so";
  std::string cmd = "gcc -O3 -shared -o";
  cmd += targ;
  cmd += " ";
  cmd += sourceFile;
  int ret = system(cmd.c_str());
  if (ret != 0)
  {
    lastError = L"Could not compile the model code.";
    throw iface::cellml_api::CellMLException();
  }

  void* t = dlopen(targ.c_str(), RTLD_NOW);
  if (t == NULL)
  {
    lastError = L"Cannot load the model code module.";
    throw iface::cellml_api::CellMLException();
  }

  return t;
}

CDA_CellMLCompiledModel::CDA_CellMLCompiledModel
(
 void* aModule, CompiledModelFunctions* aCMF,
 iface::cellml_api::Model* aModel,
 iface::cellml_services::CCodeInformation* aCCI,
 std::string& aDirname
)
  : _cda_refcount(1), mModule(aModule), mCMF(aCMF), mModel(aModel), mCCI(aCCI),
    mDirname(aDirname)
{
  mModel->add_ref();
  mCCI->add_ref();
}

CDA_CellMLCompiledModel::~CDA_CellMLCompiledModel()
{
  dlclose(mModule);
  delete mCMF;
  mModel->release_ref();
  mCCI->release_ref();
  DIR* d = opendir(mDirname.c_str());
  struct dirent* de;
  while ((de = readdir(d)))
  {
    std::string n = mDirname;
    n += "/";
    n += de->d_name;
    unlink(n.c_str());
  }
  closedir(d);
  rmdir(mDirname.c_str());
}

CDA_CellMLIntegrationRun::CDA_CellMLIntegrationRun
(
 iface::cellml_services::CellMLCompiledModel* aModel
)
  : _cda_refcount(1),
    mStepType(iface::cellml_services::RUNGE_KUTTA_FEHLBERG_4_5),
    mEpsAbs(1E-6), mEpsRel(1E-6), mScalVar(1.0), mScalRate(0.0),
    mStepSizeMax(1.0), mStartBvar(0.0), mStopBvar(10.0), mIncrementBvar(1.0),
    mObserver(NULL), mCancelIntegration(false)
{
  mModel = dynamic_cast<CDA_CellMLCompiledModel*>(aModel);
  if (mModel == NULL)
    throw iface::cellml_api::CellMLException();
  mModel->add_ref();
}

CDA_CellMLIntegrationRun::~CDA_CellMLIntegrationRun()
{
  mModel->release_ref();
  if (mObserver != NULL)
    mObserver->release_ref();
}

iface::cellml_services::IntegrationStepType
CDA_CellMLIntegrationRun::stepType
(
)
  throw (std::exception&)
{
  return mStepType;
}

void
CDA_CellMLIntegrationRun::stepType
(
 iface::cellml_services::IntegrationStepType aStepType
)
  throw(std::exception&)
{
  mStepType = aStepType;
}

void
CDA_CellMLIntegrationRun::setStepSizeControl
(
 double epsAbs, double epsRel, double scalVar,
 double scalRate
)
  throw (std::exception&)
{
  mEpsAbs = epsAbs;
  mEpsRel = epsRel;
  mScalVar = scalVar;
  mScalRate = scalRate;
}

void
CDA_CellMLIntegrationRun::setResultRange
(
 double startBvar, double stopBvar, double incrementBvar
)
  throw (std::exception&)
{
  mStartBvar = startBvar;
  mStopBvar = stopBvar;
  mIncrementBvar = incrementBvar;
}

void
CDA_CellMLIntegrationRun::setProgressObserver
(
 iface::cellml_services::IntegrationProgressObserver* aIpo
)
  throw (std::exception&)
{
  if (mObserver != NULL)
    mObserver->release_ref();
  mObserver = aIpo;
  if (mObserver != NULL)
    mObserver->add_ref();
}

void
CDA_CellMLIntegrationRun::setOverride
(
 iface::cellml_services::VariableEvaluationType aType,
 uint32_t variableIndex, double newValue
)
  throw (std::exception&)
{
  if (aType == iface::cellml_services::CONSTANT)
  {
    mConstantOverrides.push_back(std::pair<uint32_t,double>
                                 (variableIndex, newValue));
  }
  else if (aType == iface::cellml_services::DIFFERENTIAL)
  {
    mIVOverrides.push_back(std::pair<uint32_t,double>
                           (variableIndex, newValue));
  }
  else
    throw iface::cellml_api::CellMLException();
}

void
CDA_CellMLIntegrationRun::start()
  throw (std::exception&)
{
  // The new thread accesses this, so must add_ref. Thread will release itself
  // before returning.
  add_ref();
  startthread();
}

void
CDA_CellMLIntegrationRun::stop()
  throw (std::exception&)
{
}

void
CDA_CellMLIntegrationRun::runthread()
{
  std::string emsg = "Unknown error";
  double* constants = NULL, * variables = NULL, * rates = NULL;
  try
  {
    CompiledModelFunctions* f = mModel->mCMF;
    uint32_t varSize = mModel->mCCI->variableCount();
    uint32_t constSize = mModel->mCCI->constantCount();
    uint32_t boundSize = mModel->mCCI->boundCount();
    uint32_t rateSize = mModel->mCCI->rateVariableCount();
    if (boundSize > 1)
    {
      emsg = "This integrator only supports systems with one bound variable "
        "shared across all ODEs.";
      throw std::exception();
    }
    constants = new double[constSize];
    variables = new double[varSize];
    rates = new double[rateSize];
    f->SetupFixedConstants(constants);
    // Now apply constant overrides...
    OverrideList::iterator oli;
    for (oli = mConstantOverrides.begin(); oli != mConstantOverrides.end();
         oli++)
      if ((*oli).first < constSize)
        constants[(*oli).first] = (*oli).second;

    f->SetupComputedConstants(constants, variables);
    // Now apply IV overrides...
    for (oli = mIVOverrides.begin(); oli != mIVOverrides.end();
         oli++)
      if ((*oli).first < rateSize)
        variables[(*oli).first] = (*oli).second;

    double bound = 0.0;
    if (boundSize == 1)
      bound = mStartBvar;
    f->ComputeVariables(&bound, rates, constants, variables);

    if (mObserver != NULL)
      mObserver->results(varSize, variables);

    if (boundSize == 1)
      SolveODEProblem(f, constSize, constants, varSize, variables, rateSize,
                      rates);
  }
  catch (...)
  {
    try
    {
      if (mObserver != NULL)
        mObserver->failed(emsg.c_str());
    }
    catch (...)
    {
    }
  }

  if (constants != NULL)
    delete [] constants;
  if (variables != NULL)
    delete [] variables;
  if (rates != NULL)
    delete [] rates;
  release_ref();
}

iface::cellml_services::CellMLCompiledModel*
CDA_CellMLIntegrationService::compileModel
(
 iface::cellml_services::CGenerator* aCG,
 iface::cellml_api::Model* aModel
)
  throw(std::exception&)
{
  // Generate code information...
  ObjRef<iface::cellml_services::CCodeInformation> cci;
  try
  {
    cci = already_AddRefd<iface::cellml_services::CCodeInformation>
      (aCG->generateCode(aModel));
  }
  catch (...)
  {
    wchar_t* le = aCG->lastError();
    mLastError = le;
    free(le);
    throw iface::cellml_api::CellMLException();
  }

  // Create a temporary directory...
  const char* tmpenvs[] = {"TMPDIR", "TEMP", "TMP", NULL};
  const char** p = tmpenvs;
  char* fn = NULL;
  while (!fn && *p)
  {
    char* env = getenv(*p);
    if (env != NULL)
      fn = attempt_make_tempdir(env);
  }
  if (fn == NULL)
  {
#ifdef WIN32
    const char* tmpdirs[] = {"c:\\temp", "c:\\tmp", "\\temp", "\\tmp", NULL};
#else
    const char* tmpdirs[] = {"/tmp", "/var/tmp", "/usr/tmp", NULL};
#endif
    p = tmpdirs;
    while (!fn && *p)
      fn = attempt_make_tempdir(*p);
  }
  if (fn == NULL)
    throw iface::cellml_api::CellMLException();
  std::string dirname = fn;
  free(fn);

  // We now have a temporary directory. Make the source file...
  std::string sourcename = dirname + "/generated.c";
  std::ofstream ss(sourcename.c_str());
  ss << "/* This file is automatically generated and will be automatically"
     << std::endl
     << " * deleted. Don't edit it or changes will be lost. */" << std::endl;
  ss << "#include <math.h>" << std::endl;
  char* frag = cci->functionsFragment();
  ss << frag << std::endl;
  free(frag);
  ss << "void SetupFixedConstants(double* CONSTANTS)" << std::endl;
  frag = cci->fixedConstantFragment();
  ss << "{" << std::endl << frag << std::endl << "}" << std::endl;
  free(frag);
  ss << "void SetupComputedConstants(double* CONSTANTS, double* VARIABLES)"
     << std::endl;
  frag = cci->computedConstantFragment();
  ss << "{" << std::endl << frag << std::endl << "}" << std::endl;
  free(frag);
  ss << "void ComputeRates(double* BOUND, double* RATES, double* CONSTANTS, "
    "double* VARIABLES)" << std::endl;
  frag = cci->rateCodeFragment();
  ss << "{" << std::endl << frag << std::endl << "}" << std::endl;
  free(frag);
  ss << "void ComputeVariables(double* BOUND, double* RATES, double* CONSTANTS, "
    "double* VARIABLES)" << std::endl;
  frag = cci->variableCodeFragment();
  ss << "{" << std::endl << frag << std::endl << "}" << std::endl;
  free(frag);
  ss.close();

  void* mod = CompileSource(dirname, sourcename, mLastError);
  CompiledModelFunctions* cmf = SetupCompiledModelFunctions(mod);
  
  return new CDA_CellMLCompiledModel(mod, cmf, aModel, cci, dirname);
}

iface::cellml_services::CellMLIntegrationRun*
CDA_CellMLIntegrationService::createIntegrationRun
(
 iface::cellml_services::CellMLCompiledModel* aModel
)
  throw (std::exception&)
{
  return new CDA_CellMLIntegrationRun(aModel);
}

iface::cellml_services::CellMLIntegrationService*
CreateIntegrationService()
{
  return new CDA_CellMLIntegrationService();
}
