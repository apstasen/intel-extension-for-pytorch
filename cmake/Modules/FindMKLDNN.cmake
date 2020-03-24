# - Try to find MKLDNN
#
# The following variables are optionally searched for defaults
#  MKL_FOUND             : set to true if a library implementing the CBLAS interface is found
#
# The following are set after configuration is done:
#  MKLDNN_FOUND          : set to true if mkl-dnn is found.
#  MKLDNN_INCLUDE_DIR    : path to mkl-dnn include dir.
#  MKLDNN_LIBRARIES      : list of libraries for mkl-dnn
#
# The following variables are used:
#  MKLDNN_USE_NATIVE_ARCH : Whether native CPU instructions should be used in MKLDNN. This should be turned off for
#  general packaging to avoid incompatible CPU instructions. Default: OFF.

IF (NOT MKLDNN_FOUND)

SET(MKLDNN_LIBRARIES)
SET(MKLDNN_INCLUDE_DIR)

IF(MSVC)
  MESSAGE(STATUS "MKL-DNN needs omp 3+ which is not supported in MSVC so far")
  RETURN()
ENDIF(MSVC)

SET(IDEEP_ROOT "${PROJECT_SOURCE_DIR}/third_party/dnnl_for_gpu")
SET(MKLDNN_ROOT "${IDEEP_ROOT}/mkl-dnn")

FIND_PACKAGE(BLAS)
FIND_PATH(IDEEP_INCLUDE_DIR ideep.hpp PATHS ${IDEEP_ROOT} PATH_SUFFIXES include)
FIND_PATH(MKLDNN_INCLUDE_DIR mkldnn.hpp mkldnn.h PATHS ${MKLDNN_ROOT} PATH_SUFFIXES include)
IF (NOT MKLDNN_INCLUDE_DIR)
  EXECUTE_PROCESS(COMMAND git${CMAKE_EXECUTABLE_SUFFIX} submodule update --init mkl-dnn WORKING_DIRECTORY ${IDEEP_ROOT})
  FIND_PATH(MKLDNN_INCLUDE_DIR mkldnn.hpp mkldnn.h PATHS ${MKLDNN_ROOT} PATH_SUFFIXES include)
ENDIF(NOT MKLDNN_INCLUDE_DIR)

IF (NOT IDEEP_INCLUDE_DIR OR NOT MKLDNN_INCLUDE_DIR)
  MESSAGE(STATUS "MKLDNN source files not found!")
  RETURN()
ENDIF(NOT IDEEP_INCLUDE_DIR OR NOT MKLDNN_INCLUDE_DIR)
LIST(APPEND MKLDNN_INCLUDE_DIR ${IDEEP_INCLUDE_DIR})
IF(MKL_FOUND)
  ADD_DEFINITIONS(-DIDEEP_USE_MKL)
  # Append to mkldnn dependencies
  LIST(APPEND MKLDNN_LIBRARIES ${MKL_LIBRARIES})
  LIST(APPEND MKLDNN_INCLUDE_DIR ${MKL_INCLUDE_DIR})
ELSE(MKL_FOUND)
  SET(DNNL_USE_MKL "NONE" CACHE STRING "" FORCE)
ENDIF(MKL_FOUND)
SET(MKL_cmake_included TRUE)

IF (USE_SYCL)
  SET(DNNL_CPU_RUNTIME "SYCL" CACHE STRING "mkldnn cpu backend" FORCE)
  SET(DNNL_GPU_RUNTIME "SYCL" CACHE STRING "mkldnn gpu backend" FORCE)
  SET(DNNL_ENABLE_PRIMITIVE_CACHE TRUE CACHE BOOL "mkldnn sycl primitive cache" FORCE)
ENDIF()

IF(MKLDNN_USE_NATIVE_ARCH)  # Disable HostOpts in MKLDNN unless MKLDNN_USE_NATIVE_ARCH is set.
  SET(DNNL_ARCH_OPT_FLAGS "HostOpts" CACHE STRING "" FORCE)
ELSE()
  IF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    SET(DNNL_ARCH_OPT_FLAGS "-msse4" CACHE STRING "" FORCE)
  ELSE()
    SET(DNNL_ARCH_OPT_FLAGS "" CACHE STRING "" FORCE)
  ENDIF()
ENDIF()

SET(DNNL_BUILD_TESTS FALSE CACHE BOOL "build with mkl-dnn tests" FORCE)
SET(DNNL_BUILD_EXAMPLES FALSE CACHE BOOL "build with mkl-dnn examples" FORCE)
SET(DNNL_ENABLE_CONCURRENT_EXEC TRUE CACHE BOOL "multi-thread primitive execution" FORCE)
SET(DNNL_LIBRARY_TYPE STATIC CACHE STRING "" FORCE)

ADD_SUBDIRECTORY(${MKLDNN_ROOT})
IF(NOT TARGET dnnl)
  MESSAGE("Failed to include MKL-DNN target")
  RETURN()
ENDIF(NOT TARGET dnnl)
IF(MKL_FOUND)
  SET(USE_MKL_CBLAS -DUSE_MKL)
  IF(USE_MKLDNN_CBLAS)
    LIST(APPEND USE_MKL_CBLAS -DUSE_CBLAS)
  ENDIF(USE_MKLDNN_CBLAS)
  TARGET_COMPILE_DEFINITIONS(dnnl PRIVATE USE_MKL_CBLAS)
ENDIF(MKL_FOUND)
IF(NOT APPLE AND CMAKE_COMPILER_IS_GNUCC)
  TARGET_COMPILE_OPTIONS(dnnl PRIVATE -Wno-uninitialized)
  TARGET_COMPILE_OPTIONS(dnnl PRIVATE -Wno-strict-overflow)
  TARGET_COMPILE_OPTIONS(dnnl PRIVATE -Wno-error=strict-overflow)
ENDIF(NOT APPLE AND CMAKE_COMPILER_IS_GNUCC)
TARGET_COMPILE_OPTIONS(dnnl PRIVATE -Wno-tautological-compare)
GET_TARGET_PROPERTY(DNNL_INCLUDES dnnl INCLUDE_DIRECTORIES)
LIST(APPEND MKLDNN_INCLUDE_DIR ${DNNL_INCLUDES})
LIST(APPEND MKLDNN_LIBRARIES dnnl)

SET(MKLDNN_FOUND TRUE)
MESSAGE(STATUS "Found MKL-DNN: TRUE")

ENDIF(NOT MKLDNN_FOUND)
