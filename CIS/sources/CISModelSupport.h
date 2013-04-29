#ifdef __cplusplus
extern "C"
{
#endif
#ifndef CDA_EXPORT_PRE
#define CDA_EXPORT_PRE extern
#define CDA_EXPORT_POST
struct fail_info;
#endif
#if 0
}
#endif

CDA_EXPORT_PRE void clearFailure(struct fail_info*) CDA_EXPORT_POST;
CDA_EXPORT_PRE void setFailure(struct fail_info*, const char*, int) CDA_EXPORT_POST;
CDA_EXPORT_PRE void failAddCause(struct fail_info*, const char*) CDA_EXPORT_POST;
CDA_EXPORT_PRE int getFailType(struct fail_info* aFail) CDA_EXPORT_POST;

struct Override;
struct EDoubleStruct;
typedef struct EDoubleStruct* EDouble;

CDA_EXPORT_PRE EDouble CreateEDouble(double aValue) CDA_EXPORT_POST;
CDA_EXPORT_PRE double UseEDouble(EDouble aValue, struct fail_info* aFail, const char* aContext) CDA_EXPORT_POST;
CDA_EXPORT_PRE void TryAssign(double* aDest, EDouble aValue, const char* aContext, struct fail_info* aFail) CDA_EXPORT_POST;
CDA_EXPORT_PRE void TryOverrideAssign(double* aDest, EDouble aValue, const char* aContext, struct Override* aOverrides, struct fail_info* aFail) CDA_EXPORT_POST;
CDA_EXPORT_PRE void OverrideAssign(double* aDest, double aValue, struct Override* aOverrides) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryAbs(double aInput, struct fail_info* aFail) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryAnd(int count, ...) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryACos(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryACosh(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryACot(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryACoth(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryACsc(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryACsch(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryASec(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryASech(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryASin(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryASinh(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryATan(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryATanh(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryCeil(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryCos(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryCosh(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryCot(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryCoth(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryCsc(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryCsch(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryDivide(EDouble aInput1, EDouble aInput2) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryEq(int count, ...) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryExp(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryFactorial(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryFactorOf(EDouble aInput1, EDouble aInput2) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryFloor(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryGCD(int count, ...) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryGeq(int count, ...) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryGt(int count, ...) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryImplies(EDouble aInput1, EDouble aInput2) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryLCM(int count, ...) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryLeq(int count, ...) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryLn(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryLog(EDouble aInput1, EDouble aInput2) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryLt(int count, ...) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryMax(int count, ...) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryMin(int count, ...) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryMinus(EDouble aInput1, EDouble aInput2) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryNeq(EDouble aInput1, EDouble aInput2) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryNot(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryOr(int count, ...) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryPlus(int count, ...) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryPower(EDouble aInput1, EDouble aInput2) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryQuotient(EDouble aInput1, EDouble aInput2) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryRem(EDouble aInput1, EDouble aInput2) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryRoot(EDouble aInput1, EDouble aInput2) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TrySec(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TrySech(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TrySin(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TrySinh(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryTan(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryTanh(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryTimes(int count, ...) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryUnaryMinus(EDouble aInput) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryUnitsConversion(EDouble value, EDouble mup, EDouble offset) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryXor(EDouble aInput1, EDouble aInput2) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryPiecewise(EDouble firstCond, EDouble firstValue, ...) CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryInfinity() CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryNaN() CDA_EXPORT_POST;
CDA_EXPORT_PRE EDouble TryDefint(EDouble (*f)(double VOI,double *C,double *R,double *S,double *A, struct fail_info*),
                                double VOI,double *C,double *R,double *S,double *A,double *V,
                                EDouble lowV, EDouble highV,
                                struct fail_info*) CDA_EXPORT_POST;

CDA_EXPORT_PRE double factorial(double x) CDA_EXPORT_POST;
CDA_EXPORT_PRE double arbitrary_log(double value, double logbase) CDA_EXPORT_POST;
CDA_EXPORT_PRE double gcd_pair(double a, double b) CDA_EXPORT_POST;
CDA_EXPORT_PRE double lcm_pair(double a, double b) CDA_EXPORT_POST;
CDA_EXPORT_PRE double gcd_multi(unsigned int size, ...) CDA_EXPORT_POST;
CDA_EXPORT_PRE double lcm_multi(unsigned int size, ...) CDA_EXPORT_POST;
CDA_EXPORT_PRE double multi_min(unsigned int size, ...) CDA_EXPORT_POST;
CDA_EXPORT_PRE double multi_max(unsigned int size, ...) CDA_EXPORT_POST;
CDA_EXPORT_PRE double safe_quotient(double num, double den) CDA_EXPORT_POST;
CDA_EXPORT_PRE double safe_remainder(double num, double den) CDA_EXPORT_POST;
CDA_EXPORT_PRE double safe_factorof(double num, double den) CDA_EXPORT_POST;
CDA_EXPORT_PRE void do_nonlinearsolve(void (*)(double *p, double *hx, 
                                                 void *adata),
                                      double* params, struct fail_info*,
                                      unsigned int size, void* adata) CDA_EXPORT_POST;
CDA_EXPORT_PRE double defint(double (*f)(double VOI,double *C,double *R,double *S,double *A, struct fail_info*),
                             double VOI,double *C,double *R,double *S,double *A,double *V,
                             double lowV, double highV,
                             struct fail_info*) CDA_EXPORT_POST;
CDA_EXPORT_PRE double SampleUsingPDF(double (*pdf)(double bvar, double* CONSTANTS, double* ALGEBRAIC,
                                                   struct fail_info*),
                                     int nroots, double (**rootFuncs)(double bvar, double* CONSTANTS,
                                                                      double* ALGEBRAIC, struct fail_info*),
                                     double* CONSTANTS, double* ALGEBRAIC, struct fail_info*) CDA_EXPORT_POST;

#ifdef __cplusplus
}
#endif
