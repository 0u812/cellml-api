DECLARE_EXTENSION(xpath)
DECLARE_IDL(xpath)
DECLARE_IDL_DEPENDENCY(MathML_content_APISPEC)
DECLARE_IDL_DEPENDENCY(SProS)
DECLARE_EXTENSION_END(xpath)
DECLARE_EXTENSION(srus)
DECLARE_IDL(SRuS)
DECLARE_IDL_DEPENDENCY(SProS)
DECLARE_IDL_DEPENDENCY(CIS)
DECLARE_EXTENSION_END(srus)

INCLUDE_DIRECTORIES(SRuS)
INCLUDE_DIRECTORIES(xpath)

ADD_LIBRARY(xpath xpath/XPathImpl.cpp)
TARGET_LINK_LIBRARIES(xpath cellml)
SET_TARGET_PROPERTIES(xpath PROPERTIES VERSION ${GLOBAL_VERSION} SOVERSION ${XPATH_SOVERSION})
INSTALL(TARGETS xpath DESTINATION lib)
ADD_LIBRARY(srus SRuS/SRuSImpl.cpp SRuS/SRuSEval.cpp)
TARGET_LINK_LIBRARIES(srus xpath cis ${CMAKE_DL_LIBS})
SET_TARGET_PROPERTIES(srus PROPERTIES VERSION ${GLOBAL_VERSION} SOVERSION ${SRUS_SOVERSION})
INSTALL(TARGETS srus DESTINATION lib)

DECLARE_BOOTSTRAP("SRuSBootstrap" "SRuS" "Bootstrap" "SRuS" "createSRuSBootstrap" "CreateSRuSBootstrap" "SRuSBootstrap.hpp" "SRuS" "srus")

IF (BUILD_TESTING)
  ADD_EXECUTABLE(RunSEDML SRuS/tests/RunSEDML.cpp)
  TARGET_LINK_LIBRARIES(RunSEDML cellml ccgs cuses cevas malaes annotools spros srus xpath)
  ADD_TEST(CheckSRuS ${BASH} ${CMAKE_CURRENT_SOURCE_DIR}/tests/RetryWrapper ${CMAKE_CURRENT_SOURCE_DIR}/tests/CheckSRuS)
  DECLARE_TEST_LIB(srus)
  DECLARE_TEST_LIB(xpath)
  DECLARE_CPPUNIT_FILE(XPath)
ENDIF()
