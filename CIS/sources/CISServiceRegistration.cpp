#include "ServiceRegistration.hxx"
#include "Utilities.hxx"
#include "CISImplementation.hxx"
#include "CISBootstrap.hpp"

// This is a hack to force linking...
#include "SCICIS.hxx"
#include "corba/support/WrapperRepository.hxx"
#include "CCICIS.hxx"

iface::cellml_context::CellMLModule* gIntegrationService;

int
do_registration(void* aContext, void* aModuleManager, void (*UnloadService)())
{
  RETURN_INTO_OBJREF(cis, iface::cellml_services::CellMLIntegrationService,
                     CreateIntegrationService());
  QUERY_INTERFACE(gIntegrationService, cis, cellml_context::CellMLModule);

  // All unload requests are silently ignored until we find a better way to
  // sort out the linking issues on all platforms.
  // gIntegrationService->SetUnloadCIS(UnloadService);

  reinterpret_cast<iface::cellml_context::CellMLModuleManager*>(aModuleManager)
    ->registerModule(gIntegrationService);

  // Ugly hack to force linking...
  SCI::cellml_services::prodCellMLIntegrationRun();
  CCI::cellml_services::prodCellMLIntegrationRun();
}

void
do_deregistration(void* aModuleManager)
{
  reinterpret_cast<iface::cellml_context::CellMLModuleManager*>(aModuleManager)
    ->deregisterModule(gIntegrationService);
  gIntegrationService->release_ref();
}
