## OpenGrain 
## Copyright 2011 John Williamson
## License: BSD3


## OpenGrain build file

FIND_PACKAGE(SWIG)
FIND_PACKAGE(PythonLibs)
IF(SWIG_FOUND AND PYTHONLIBS_FOUND)


message("SWIG and Python found. Building the python extensions...")

INCLUDE(${SWIG_USE_FILE})


INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${OPENGRAIN_SOURCE_DIR}/src/api)
link_directories(${OPENGRAIN_BINARY_DIR}/src)

SET(CMAKE_SWIG_FLAGS "")

SET_SOURCE_FILES_PROPERTIES(opengrain.i PROPERTIES CPLUSPLUS OFF)
SET_SOURCE_FILES_PROPERTIES(opengrain.i PROPERTIES SWIG_FLAGS "-includeall")
SWIG_ADD_MODULE(opengrain python opengrain.i)
SWIG_LINK_LIBRARIES(opengrain ${PYTHON_LIBRARIES} opengrain)
ELSE()
message("No SWIG or no Python found. Python extensions not built!")
ENDIF()