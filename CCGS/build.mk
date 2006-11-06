lib_LTLIBRARIES += libccgs.la

libccgs_la_LIBADD := \
  libcellml.la \
  $(STLLINK)

libccgs_la_SOURCES := \
  $(top_srcdir)/CCGS/sources/CCGSImplementation.cpp \
  $(top_srcdir)/CCGS/sources/CCGSGenerator.cpp \
  $(top_srcdir)/CCGS/sources/Units.cxx \
  $(top_srcdir)/CCGS/sources/Equality.cxx \
  $(top_srcdir)/CCGS/sources/VariableConnections.cpp

libccgs_la_CXXFLAGS := \
  -Wall -ggdb -I$(top_srcdir)/sources -I$(top_builddir)/interfaces $(AM_CXXFLAGS)

include_HEADERS += \
  $(top_builddir)/interfaces/IfaceCCGS.hxx \
  $(top_srcdir)/CCGS/sources/CCGSBootstrap.hpp

BUILT_SOURCES += $(top_builddir)/interfaces/IfaceCCGS.hxx

include $(top_srcdir)/CCGS/tests/build.mk
