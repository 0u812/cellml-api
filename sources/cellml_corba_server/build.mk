# CellML CORBA Server...
bin_PROGRAMS := cellml_corba_server
cellml_corba_server_SOURCES := sources/cellml_corba_server/Main.cpp
cellml_corba_server_CXXFLAGS := \
  -I$(top_srcdir) -I$(top_srcdir)/sources -I$(top_srcdir)/sources/cellml \
  -I $(top_srcdir)/sources/dom -I $(top_srcdir)/sources/dom_direct \
  -I $(top_srcdir)/sources/mathml -I$(top_builddir)/interfaces
cellml_corba_server_LDADD := \
  $(top_builddir)/sources/libcellml.la \
  $(top_builddir)/interfaces/libcellml_corba.la
cellml_corba_server_LDFLAGS := -lomniORB4 -lxml2
