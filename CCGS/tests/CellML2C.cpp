/*
 * Please note that CellML2C is included as a testing program to help test the
 * API, and not as a general purpose tool. Please carefully consider whether
 * adding a new CellML2* program is justified on the grounds of testing the API
 * more extensively before doing so; if your only reason is you want to provide
 * a command line tool for another language, please do so in a separate package
 * instead. See also:
 *  [1] https://tracker.physiomeproject.org/show_bug.cgi?id=1279
 *  [2] https://tracker.physiomeproject.org/show_bug.cgi?id=1514#c16
 *  [3] http://www.cellml.org/pipermail/cellml-tools-developers/2009-January/000140.html
 */
#include "Utilities.hxx"
#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif
#include <exception>
#include "cda_compiler_support.h"
#include "IfaceCellML_APISPEC.hxx"
#include "IfaceCCGS.hxx"
#include "IfaceAnnoTools.hxx"
#include "CCGSBootstrap.hpp"
#include "CellMLBootstrap.hpp"
#include "AnnoToolsBootstrap.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <vector>
#include <algorithm>
#include <string>

#ifdef _WIN32
#define swprintf _snwprintf
#endif

wchar_t*
TypeToString(iface::cellml_services::VariableEvaluationType vet)
{
  switch (vet)
  {
  case iface::cellml_services::VARIABLE_OF_INTEGRATION:
    return L"variable of integration";
  case iface::cellml_services::CONSTANT:
    return L"constant";
  case iface::cellml_services::STATE_VARIABLE:
    return L"state variable";
  case iface::cellml_services::ALGEBRAIC:
    return L"algebraic variable";
  case iface::cellml_services::FLOATING:
    return L"uncomputed";
  }

  return L"invalid type";
}

void
WriteCode(iface::cellml_services::CodeInformation* cci)
{
  // Scoped locale change.
  CNumericLocale locobj;

  iface::cellml_services::ModelConstraintLevel mcl =
    cci->constraintLevel();
  if (mcl == iface::cellml_services::UNDERCONSTRAINED)
  {
    printf("/* Model is underconstrained.\n"
           " * List of undefined targets follows...\n");
    iface::cellml_services::ComputationTargetIterator* cti = cci->iterateTargets();
    iface::cellml_services::ComputationTarget* ct;
    std::vector<std::wstring> messages;
    while (true)
    {
      ct = cti->nextComputationTarget();
      if (ct == NULL)
        break;
      if (ct->type() != iface::cellml_services::FLOATING)
      {
        ct->release_ref();
        continue;
      }
      iface::cellml_api::CellMLVariable* v = ct->variable();
      wchar_t* n = v->name();
      wchar_t* c = v->componentName();
      std::wstring str = L" * * ";
      uint32_t deg = ct->degree();
      if (deg != 0)
      {
        str += L"d^";
        wchar_t buf[20];
        swprintf(buf, 20, L"%u", deg);
        str += buf;
        str += L"/dt^";
        str += buf;
        str += L" ";
      }
      str += n;
      str += L" (in ";
      str += c;
      str += L")\n";
      free(n);
      free(c);
      messages.push_back(str);
      v->release_ref();
      ct->release_ref();
    }
    cti->release_ref();
    // Sort the messages...
    std::sort(messages.begin(), messages.end());
    std::vector<std::wstring>::iterator msgi;
    for (msgi = messages.begin(); msgi != messages.end(); msgi++)
      printf("%S", (*msgi).c_str());
    printf(" */\n");
    return;
  }
  else if (mcl == iface::cellml_services::OVERCONSTRAINED)
  {
    printf("/* Model is overconstrained.\n"
           " * List variables defined at time of error follows...\n");
    iface::cellml_services::ComputationTargetIterator* cti = cci->iterateTargets();
    iface::cellml_services::ComputationTarget* ct;
    std::vector<std::wstring> messages;
    while (true)
    {
      ct = cti->nextComputationTarget();
      if (ct == NULL)
        break;
      if (ct->type() == iface::cellml_services::FLOATING)
      {
        ct->release_ref();
        continue;
      }
      iface::cellml_api::CellMLVariable* v = ct->variable();
      wchar_t* n = v->name();
      std::wstring str = L" * * ";
      uint32_t deg = ct->degree();
      if (deg != 0)
      {
        str += L"d^";
        wchar_t buf[20];
        swprintf(buf, 20, L"%u", deg);
        str += buf;
        str += L"/dt^";
        str += buf;
        str += L" ";
      }
      str += n;
      free(n);
      str += L"\n";
      messages.push_back(str);
      v->release_ref();
      ct->release_ref();
    }
    cti->release_ref();

    // Sort the messages...
    std::sort(messages.begin(), messages.end());
    std::vector<std::wstring>::iterator msgi;
    for (msgi = messages.begin(); msgi != messages.end(); msgi++)
      printf("%S", (*msgi).c_str());

    // Get flagged equations...
    iface::mathml_dom::MathMLNodeList* mnl = cci->flaggedEquations();
    printf(" * Extraneous equation was:\n");
    iface::dom::Node* n = mnl->item(0);
    mnl->release_ref();
    iface::dom::Element* el =
      reinterpret_cast<iface::dom::Element*>(n->query_interface("dom::Element"));
    n->release_ref();

    wchar_t* cmeta = el->getAttribute(L"id");
    if (!wcscmp(cmeta, L""))
      printf(" *   <equation with no cmeta ID>\n");
    else
      printf(" *   %S\n", cmeta);
    free(cmeta);

    n = el->parentNode();
    el->release_ref();

    if (n != NULL)
    {
      el = reinterpret_cast<iface::dom::Element*>
        (n->query_interface("dom::Element"));
      n->release_ref();

      cmeta = el->getAttribute(L"id");
      if (!wcscmp(cmeta, L""))
        printf(" *   in <math with no cmeta ID>\n");
      else
        printf(" *   in math with cmeta:id %S\n", cmeta);
      free(cmeta);

      el->release_ref();
    }

    printf(" */\n");
    return;
  }
  else if (mcl == iface::cellml_services::UNSUITABLY_CONSTRAINED)
  {
    printf("/* Model is unsuitably constrained (i.e. would need capabilities"
           " beyond those of the CCGS to solve).\n"
           " * The status of variables at time of error follows...\n");
    iface::cellml_services::ComputationTargetIterator* cti = cci->iterateTargets();
    iface::cellml_services::ComputationTarget* ct;
    std::vector<std::wstring> messages;
    while (true)
    {
      ct = cti->nextComputationTarget();
      if (ct == NULL)
        break;
      std::wstring str = L" * * ";
      if (ct->type() == iface::cellml_services::FLOATING)
        str += L" Undefined: ";
      else
        str += L" Defined: ";

      uint32_t deg = ct->degree();
      if (deg != 0)
      {
        str += L"d^";
        wchar_t buf[20];
        swprintf(buf, 20, L"%u", deg);
        str += buf;
        str += L"/dt^";
        str += buf;
        str += L" ";
      }
      iface::cellml_api::CellMLVariable* v = ct->variable();
      wchar_t* n = v->name();
      str += n;
      free(n);
      str += L"\n";
      messages.push_back(str);
      v->release_ref();
      ct->release_ref();
    }
    cti->release_ref();

    // Sort the messages...
    std::sort(messages.begin(), messages.end());
    std::vector<std::wstring>::iterator msgi;
    for (msgi = messages.begin(); msgi != messages.end(); msgi++)
      printf("%S", (*msgi).c_str());

    printf(" */\n");
    return;
  }

  printf("/* Model is correctly constrained.\n");
  iface::mathml_dom::MathMLNodeList* mnl = cci->flaggedEquations();
  uint32_t i, l = mnl->length();
  if (l == 0)
    printf(" * No equations needed Newton-Raphson evaluation.\n");
  else
    printf(" * The following equations needed Newton-Raphson evaluation:\n");

  std::vector<std::wstring> messages;
  for (i = 0; i < l; i++)
  {
    iface::dom::Node* n = mnl->item(i);
    iface::dom::Element* el =
      reinterpret_cast<iface::dom::Element*>(n->query_interface("dom::Element"));
    n->release_ref();

    wchar_t* cmeta = el->getAttribute(L"id");
    std::wstring str;
    if (!wcscmp(cmeta, L""))
      str += L" *   <equation with no cmeta ID>\n";
    else
    {
      str += L" *   ";
      str += cmeta;
      str += L"\n";
    }
    free(cmeta);

    n = el->parentNode();
    el->release_ref();

    el = reinterpret_cast<iface::dom::Element*>
      (n->query_interface("dom::Element"));
    n->release_ref();

    cmeta = el->getAttribute(L"id");
    if (!wcscmp(cmeta, L""))
      str += L" *   in <math with no cmeta ID>\n";
    else
    {
      str += L" *   in math with cmeta:id ";
      str += cmeta;
      str += L"\n";
    }
    free(cmeta);
    el->release_ref();

    messages.push_back(str);
  }
  mnl->release_ref();

  // Sort the messages...
  std::sort(messages.begin(), messages.end());
  std::vector<std::wstring>::iterator msgi;
  for (msgi = messages.begin(); msgi != messages.end(); msgi++)
    printf("%S", (*msgi).c_str());
  
  printf(" * The rate and state arrays need %u entries.\n", cci->rateIndexCount());
  printf(" * The algebraic variables array needs %u entries.\n", cci->algebraicIndexCount());
  printf(" * The constant array needs %u entries.\n", cci->constantIndexCount());
  printf(" * Variable storage is as follows:\n");
  
  messages.clear();
  iface::cellml_services::ComputationTargetIterator* cti = cci->iterateTargets();
  while (true)
  {
    iface::cellml_services::ComputationTarget* ct = cti->nextComputationTarget();
    if (ct == NULL)
      break;
    iface::cellml_api::CellMLVariable* v = ct->variable();
    iface::cellml_api::CellMLElement* el = v->parentElement();
    iface::cellml_api::CellMLComponent* c =
      reinterpret_cast<iface::cellml_api::CellMLComponent*>
      (el->query_interface("cellml_api::CellMLComponent"));
    el->release_ref();

    std::wstring str;
    wchar_t* vn = v->name(), * cn = c->name();
    str += L" * * Target ";
    uint32_t deg = ct->degree();
    if (deg != 0)
    {
      str += L"d^";
      wchar_t buf[20];
      swprintf(buf, 20, L"%u", deg);
      str += buf;
      str += L"/dt^";
      str += buf;
      str += L" ";
    }
    str += vn;
    str += L" in component ";
    str += cn;
    str += L"\n";
    free(vn);
    free(cn);

    c->release_ref();
    v->release_ref();

    str += L" * * * Variable type: ";
    str += TypeToString(ct->type());
    str += L"\n * * * Variable index: ";
    wchar_t buf[40];
    swprintf(buf, 40, L"%u\n", ct->assignedIndex());
    str += buf;

    str += L" * * * Variable storage: ";
    wchar_t * vsn;
    vsn = ct->name();
    str += vsn;
    free(vsn);
    str += '\n';

    ct->release_ref();

    messages.push_back(str);
  }
  cti->release_ref();

  // Sort the messages...
  std::sort(messages.begin(), messages.end());
  for (msgi = messages.begin(); msgi != messages.end(); msgi++)
    printf("%S", (*msgi).c_str());

  printf(" */\n");

  wchar_t* frag = cci->functionsString();
  printf("%S", frag);
  free(frag);

  // Now start the code...
  frag = cci->initConstsString();
  printf("void SetupFixedConstants(double* CONSTANTS, double* RATES, double* STATES)\n{\n%S}\n", frag);
  free(frag);

  frag = cci->variablesString();
  printf("void EvaluateVariables(double VOI, double* CONSTANTS, double* RATES, double* STATES, double* ALGEBRAIC)\n"
         "{\n%S}\n", frag);
  free(frag);

  frag = cci->ratesString();
  printf("void ComputeRates(double VOI, double* STATES, double* RATES, double* CONSTANTS, "
         "double* ALGEBRAIC)\n"
         "{\n%S}\n", frag);
  free(frag);
}

void
doNameAnnotations(iface::cellml_api::Model* aModel,
                  iface::cellml_services::CodeGenerator* aCG)
{
  // Make an annotation set...
  iface::cellml_services::AnnotationToolService* ats
    (CreateAnnotationToolService());
  iface::cellml_services::AnnotationSet* as(ats->createAnnotationSet());
  ats->release_ref();

  aCG->useAnnoSet(as);

  // Now we go through all variables in the model and set their annotations...
  iface::cellml_api::CellMLComponentSet* ccs = aModel->allComponents();

  iface::cellml_api::CellMLComponentIterator* cci = ccs->iterateComponents();
  ccs->release_ref();

  iface::cellml_api::CellMLComponent* comp;
  while ((comp = cci->nextComponent()) != NULL)
  {
    iface::cellml_api::CellMLVariableSet* vs(comp->variables());
    wchar_t* compname = comp->name();
    comp->release_ref();
    iface::cellml_api::CellMLVariableIterator* vi(vs->iterateVariables());
    vs->release_ref();

    iface::cellml_api::CellMLVariable* v;
    while ((v = vi->nextVariable()) != NULL)
    {
      wchar_t* name = v->name();
      std::wstring varn = compname;
      varn += L"_";
      varn += name;
      free(name);
      std::wstring raten = L"rate_";
      raten += varn;
      as->setStringAnnotation(v, L"expression", varn.c_str());
      as->setStringAnnotation(v, L"expression_d1", raten.c_str());
      v->release_ref();
    }

    vi->release_ref();

    free(compname);
  }

  cci->release_ref();

  as->release_ref();
}

int
main(int argc, char** argv)
{
  // Get the URL from which to load the model...
  if (argc < 2)
  {
    printf("Usage: CellML2C modelURL\n");
    return -1;
  }

  uint32_t usenames = 0;

  if (argc > 2)
  {
    if (!strcmp(argv[2], "usenames"))
      usenames = 1;
  }

  wchar_t* URL;
  size_t l = strlen(argv[1]);
  URL = new wchar_t[l + 1];
  memset(URL, 0, (l + 1) * sizeof(wchar_t));
  const char* mbrurl = argv[1];
  mbsrtowcs(URL, &mbrurl, l, NULL);

  iface::cellml_api::CellMLBootstrap* cb =
    CreateCellMLBootstrap();

  iface::cellml_api::ModelLoader* ml =
    cb->modelLoader();
  cb->release_ref();

  iface::cellml_api::Model* mod;
  try
  {
    mod = ml->loadFromURL(URL);
  }
  catch (...)
  {
    printf("Error loading model URL.\n");
    // Well, a leak on exit wouldn't be so bad, but someone might reuse this
    // code, so...
    delete [] URL;
    ml->release_ref();
    return -1;
  }

  ml->release_ref();
  delete [] URL;

  iface::cellml_services::CodeGeneratorBootstrap* cgb =
    CreateCodeGeneratorBootstrap();
  iface::cellml_services::CodeGenerator* cg =
    cgb->createCodeGenerator();
  cgb->release_ref();

  if (usenames)
    doNameAnnotations(mod, cg);

  iface::cellml_services::CodeInformation* cci = NULL;
  try
  {
    cci = cg->generateCode(mod);
  }
  catch (iface::cellml_api::CellMLException& ce)
  {
    printf("Caught a CellMLException while generating code.\n");
    cg->release_ref();
    mod->release_ref();
    return -1;
  }
  catch (...)
  {
    printf("Unexpected exception calling generateCode!\n");
    // this is a leak, but it should also never happen :)
    return -1;
  }
  mod->release_ref();
  cg->release_ref();

  wchar_t* m = cci->errorMessage();
  if (wcscmp(m, L""))
  {
    printf("Error generating code: %S\n", m);
    cci->release_ref();
    free(m);
    return -1;
  }
  free(m);

  // We now have the code information...
  WriteCode(cci);
  cci->release_ref();

  return 0;
}
