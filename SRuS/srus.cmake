DECLARE_EXTENSION(xpath)
DECLARE_IDL(xpath)
DECLARE_EXTENSION_END(xpath)
DECLARE_EXTENSION(srus)
DECLARE_IDL(SRuS)
DECLARE_EXTENSION_END(srus)

INCLUDE_DIRECTORIES(SRuS)
INCLUDE_DIRECTORIES(xpath)

ADD_LIBRARY(xpath xpath/XPathImpl.cpp)
ADD_LIBRARY(srus SRuS/SRuSImpl.cpp)
TARGET_LINK_LIBRARIES(srus xpath cis)

DECLARE_BOOTSTRAP("SRuSBootstrap" "SRuS" "Bootstrap" "SRuS" "createSRuSBootstrap" "CreateSRuSBootstrap" "SRuSBootstrap.hpp")

IF (BUILD_TESTING)
  ADD_EXECUTABLE(RunSEDML SRuS/tests/RunSEDML.cpp)
  TARGET_LINK_LIBRARIES(RunSEDML cellml ccgs xml2 cuses cevas malaes annotools spros srus xpath)
  ADD_TEST(CheckSRuS tests/CheckSRuS)
  DECLARE_TEST_LIB(srus)
  DECLARE_TEST_LIB(xpath)
  DECLARE_CPPUNIT_FILE(XPath)
ENDIF()
