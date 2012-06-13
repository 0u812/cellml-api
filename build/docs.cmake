FIND_PACKAGE(Doxygen)
FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/docs/Doxyfile.in ${CMAKE_BINARY_DIR}/docs/Doxyfile)
ADD_CUSTOM_TARGET(docs ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/docs/Doxyfile
                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
