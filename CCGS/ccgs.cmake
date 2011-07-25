DECLARE_EXTENSION(ccgs)
DECLARE_IDL(CCGS)
DECLARE_IDL_DEPENDENCY(CellML_APISPEC)
DECLARE_IDL_DEPENDENCY(MaLaES)
DECLARE_IDL_DEPENDENCY(CUSES)
DECLARE_IDL_DEPENDENCY(CeVAS)
DECLARE_IDL_DEPENDENCY(AnnoTools)
DECLARE_EXTENSION_END(ccgs)

INCLUDE_DIRECTORIES(CCGS/sources)

ADD_LIBRARY(ccgs
  CCGS/sources/CCGSImplementation.cpp
  CCGS/sources/CCGSGenerator.cpp)
TARGET_LINK_LIBRARIES(ccgs cuses cevas malaes annotools cellml ${CMAKE_DL_LIBS})
INSTALL(TARGETS ccgs DESTINATION lib)

DECLARE_BOOTSTRAP("CCGSBootstrap" "CCGS" "CodeGeneratorBootstrap" "cellml_services" "createCodeGeneratorBootstrap" "CreateCodeGeneratorBootstrap" "CCGSBootstrap.hpp" "CCGS/sources" "ccgs")

IF (BUILD_TESTING)
  ADD_EXECUTABLE(CellML2C CCGS/tests/CellML2C.cpp)
  TARGET_LINK_LIBRARIES(CellML2C cellml ccgs xml2 cuses cevas malaes annotools)
  ADD_TEST(CheckCodeGenerator tests/CheckCodeGenerator)
  DECLARE_TEST_LIB(ccgs)
ENDIF()
