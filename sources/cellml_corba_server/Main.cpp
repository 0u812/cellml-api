#include <omniORB4/CORBA.h>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <inttypes.h>
#include "CellML-APISPEC.hh"
#include "IfaceCellML-APISPEC.hxx"
#include "CellMLBootstrap.hpp"
#include "SCICellML-APISPEC.hxx"

CORBA::ORB_var gBroker;

FILE*
PrepareCellMLHome(void)
{
  // This use of environment variables becomes a security risk if used with
  // setuid/setgid programs, so don't allow it...
#ifndef _WIN32
  if ((getuid() != geteuid()) || (getgid() != getegid()))
  {
    printf("Do not run this program setuid or setgid.\n");
    exit(-1);
  }
#endif

  // Try to load the IOR file. This is very much platform specific, and we need
  // to add support for this on other platforms too...
  const char* chome = getenv("CELLML_HOME");
  std::string cellml_home;
  if (chome == NULL)
    chome = getenv("HOME");
  else
    cellml_home = chome;
  if (chome == NULL)
    cellml_home = "/.cellml";
  else
  {
    cellml_home = chome;
    cellml_home += "/.cellml";
  }

  // Grant everyone permission, umask will restrict if needed.
  int ret = mkdir(cellml_home.c_str(), 0777);
  if (ret != 0 && errno != EEXIST)
  {
    perror("Creating CellML home directory");
    exit(-1);
  }

  std::string ior_file = cellml_home + "/corba_server";
  FILE* f = fopen(ior_file.c_str(), "r");
  if (f == NULL)
  {
    if (errno != ENOENT)
    {
      perror("Cannot open corba_server IOR file");
      exit(-1);
    }
    // No server, so open the file to write the new IOR.
    return fopen(ior_file.c_str(), "w");
  }

  // We have found an IOR. It could either be new or stale.
  char buf[4096];
  size_t len = fread(buf, 1, 4095, f);
  if (len == 0)
  {
    fclose(f);
    // Invalid IOR, so return...
    return fopen(ior_file.c_str(), "w");
  }
  buf[len] = 0;
  try
  {
    CORBA::Object_var op = gBroker->string_to_object(buf);
    cellml_api::CellMLBootstrap_var cbv =
      cellml_api::CellMLBootstrap::_narrow(op);
    if (cbv->_non_existent())
      throw std::exception();
  }
  catch (...)
  {
    fclose(f);
    // Invalid IOR, so return...
    return fopen(ior_file.c_str(), "w");
  }
  fclose(f);
  printf("There is a valid CORBA server already running.\n");
  exit(-2);
}

int
main(int argc, char** argv)
{
  gBroker = CORBA::ORB_init(argc, argv);

  FILE* fIOR = PrepareCellMLHome();

  CORBA::Object_var rootPoaObj = gBroker->resolve_initial_references("RootPOA");
  PortableServer::POA_var rootPoa = PortableServer::POA::_narrow(rootPoaObj);
  if (CORBA::is_nil(rootPoa))
  {
    printf("Problem with ORB: Could not resolve the root POA.\n");
    return -1;
  }

  // Just use the root POA for now, but perhaps later we could create a sub-POA
  // with different policies?

  // Activate the root POA...
  PortableServer::POAManager_var pm = rootPoa->the_POAManager();
  pm->activate();

  // Next, create the bootstrap object...
  iface::cellml_api::CellMLBootstrap* cbs = CreateCellMLBootstrap();

  // Now wrap it...
  SCI::cellml_api::_final_CellMLBootstrap* fcb = 
    new SCI::cellml_api::_final_CellMLBootstrap(cbs, rootPoa);
  delete rootPoa->activate_object(fcb);
  cbs->release_ref();

  cellml_api::CellMLBootstrap_var cbsV = fcb->_this();
  char* iorStr = gBroker->object_to_string(cbsV);
  fprintf(fIOR, "%s", iorStr);
  CORBA::string_free(iorStr);

  // Finally, clean up...
  fcb->release_ref();
  fcb->_remove_ref();
}
