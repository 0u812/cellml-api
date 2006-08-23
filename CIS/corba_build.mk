lib_LTLIBRARIES += libcis_corba.la libCISService.la

libcis_corba_la_SOURCES := \
  $(top_builddir)/interfaces/CCICIS.cxx \
  $(top_builddir)/interfaces/SCICIS.cxx \
  $(top_builddir)/interfaces/CISSK.cc

libcis_corba_la_LIDADD := \
  $(top_builddir)/libcellml.la \
  $(top_builddir)/libcellml_corba_bridge.la \
  $(top_builddir)/libcis.la \
  $(top_builddir)/libccgs.la \
  $(top_builddir)/libccgs_corba.la

libCISService_la_SOURCES := \
  $(top_srcdir)/CIS/sources/CISServiceRegistration.cpp

libCISService_la_LIBADD := \
  $(top_builddir)/libcellml.la \
  $(top_builddir)/libcellml_corba_bridge.la \
  $(top_builddir)/libcis.la \
  $(top_builddir)/libcis_corba.la \
  $(top_builddir)/libccgs.la \
  $(top_builddir)/libccgs_corba.la

libCISService_la_LDFLAGS := -shared -module $(OMNILINK) -lgsl -lgslcblas

libcis_corba_la_CXXFLAGS := \
	-I$(top_builddir)/interfaces \
	-I$(top_srcdir) \
	-I$(top_srcdir)/sources/cellml_corba_server \
        -I$(top_srcdir)/sources/ \
	-I$(top_srcdir)/simple_interface_generators/glue $(AM_CXXFLAGS)

libCISService_la_CXXFLAGS := \
	-I$(top_builddir)/interfaces \
	-I$(top_srcdir) \
	-I$(top_srcdir)/sources/cellml_corba_server \
        -I$(top_srcdir)/sources/ \
	-I$(top_srcdir)/simple_interface_generators/glue $(AM_CXXFLAGS)

BUILT_SOURCES += \
  $(top_builddir)/interfaces/CCICIS.cxx \
  $(top_builddir)/interfaces/SCICIS.cxx \
  $(top_builddir)/interfaces/CISSK.cc
