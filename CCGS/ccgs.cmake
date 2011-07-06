DECLARE_EXTENSION(ccgs)
DECLARE_IDL(CCGS)
DECLARE_EXTENSION_END(ccgs)

INCLUDE_DIRECTORIES(CCGS/sources)

ADD_LIBRARY(ccgs
  CCGS/sources/CCGSImplementation.cpp
  CCGS/sources/CCGSGenerator.cpp)

DECLARE_BOOTSTRAP("CCGSBootstrap" "CCGS" "CodeGeneratorBootstrap" "cellml_services" "createCodeGeneratorBootstrap" "CreateCodeGeneratorBootstrap" "CCGSBootstrap.hpp")

IF (BUILD_TESTING)
  ADD_EXECUTABLE(CellML2C CCGS/tests/CellML2C.cpp)
  TARGET_LINK_LIBRARIES(CellML2C cellml ccgs xml2 cuses cevas malaes annotools)
  ADD_TEST(CheckCodeGenerator tests/CheckCodeGenerator)
  DECLARE_TEST_LIB(ccgs)
ENDIF()
