cmake_minimum_required(VERSION 3.20)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(GitVersionDetect)
set(DIME_MAJOR_VERSION ${GITVERSIONDETECT_VERSION_MAJOR})
set(DIME_MINOR_VERSION ${GITVERSIONDETECT_VERSION_MINOR})
set(DIME_MICRO_VERSION ${GITVERSIONDETECT_VERSION_PATCH})
set(DIME_VERSION ${GITVERSIONDETECT_VERSION_MAJOR}.${GITVERSIONDETECT_VERSION_MINOR}.${GITVERSIONDETECT_VERSION_PATCH})
set(DIME_VERSION_FULL ${GITVERSIONDETECT_VERSION})


project(dime VERSION ${DIME_VERSION})
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)

string(TIMESTAMP DIME_BUILD_YEAR "%Y")
math(EXPR DIME_SO_VERSION ${PROJECT_VERSION_MAJOR}*20)

# ############################################################################
# these will be removed after upgrading CMake minimum version
set(PROJECT_DESCRIPTION   "A library for reading, constructing, manipulating, and writing DXF file data")
# ############################################################################

# ############################################################################
# Prevent in-source builds, as they often cause severe build problems
# ############################################################################
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "${CMAKE_PROJECT_NAME} requires an out of source build. Please create a separate build directory and run 'cmake <path_to_${CMAKE_PROJECT_NAME}> [options]' from there.")
endif()

# ############################################################################
# Include necessary submodules
# ############################################################################
include(CheckIncludeFile)
include(CheckSymbolExists)
include(CMakeDependentOption)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# ############################################################################
# Provide options to customise the build
# ############################################################################

option(DIME_BUILD_SHARED_LIBS "Build shared library when ON, static when OFF (default)." OFF)
#option(DIME_BUILD_TESTS "Build unit tests when ON (default), skips them when OFF." ON)
option(DIME_BUILD_DOCUMENTATION "Build and install API documentation (requires Doxygen)." OFF)
option(DIME_ENABLE_INSTALL "Should targets be installed" ON)
cmake_dependent_option(DIME_BUILD_INTERNAL_DOCUMENTATION "Document internal code not part of the API." OFF "DIME_BUILD_DOCUMENTATION" OFF)
cmake_dependent_option(DIME_BUILD_DOCUMENTATION_MAN "Build Dime man pages." OFF "DIME_BUILD_DOCUMENTATION" OFF)
cmake_dependent_option(DIME_BUILD_DOCUMENTATION_QTHELP "Build QtHelp documentation." OFF "DIME_BUILD_DOCUMENTATION" OFF)
cmake_dependent_option(DIME_BUILD_DOCUMENTATION_CHM "Build compressed HTML help manual (requires HTML help compiler)" OFF "DIME_BUILD_DOCUMENTATION" OFF)

# ##########################################################################
# Setup build environment
# ##########################################################################

check_symbol_exists(isinf math.h HAVE_ISINF)
check_symbol_exists(isnan math.h HAVE_ISNAN)
if(NOT HAVE_ISNAN)
  check_symbol_exists(_isnan float.h HAVE__ISNAN)
endif()
check_symbol_exists(finite math.h HAVE_FINITE)
if(NOT HAVE_FINITE)
  check_symbol_exists(_finite float.h HAVE__FINITE)
endif()
check_include_file(ieeefp.h HAVE_IEEEFP_H)
check_symbol_exists(fpclass ieeefp.h HAVE_FPCLASS)
if(NOT HAVE_FPCLASS)
  check_symbol_exists(_fpclass float.h HAVE__FPCLASS)
endif()

# Get all compilation units.
file(GLOB_RECURSE COMMON_SRCS src/*.cpp)
file(GLOB_RECURSE COMMON_HDRS include/*.h)

# Setup the library.
if(WIN32)
  if(MINGW)
    set(DIME_DEFAULT_SHARED_POSTFIX "")
    set(DIME_DEFAULT_STATIC_POSTFIX "")
  else()
    # On Windows the major version number is part of the library name
    set(DIME_DEFAULT_SHARED_POSTFIX ${PROJECT_VERSION_MAJOR})
    set(DIME_DEFAULT_STATIC_POSTFIX ${PROJECT_VERSION_MAJOR}s)
  endif()
  if(DIME_BUILD_SHARED_LIBS)
    set(DIME_DEFAULT_POSTFIX ${DIME_DEFAULT_SHARED_POSTFIX})
    set(DIME_LIBRARY_SUFFIX ${CMAKE_SHARED_LIBRARY_SUFFIX})
  else()
    set(DIME_DEFAULT_POSTFIX ${DIME_DEFAULT_STATIC_POSTFIX})
    set(DIME_LIBRARY_SUFFIX ${CMAKE_STATIC_LIBRARY_SUFFIX})
  endif()
  set(CMAKE_RELEASE_POSTFIX ${DIME_DEFAULT_POSTFIX})
  set(CMAKE_MINSIZEREL_POSTFIX ${DIME_DEFAULT_POSTFIX})
  set(CMAKE_RELWITHDEBINFO_POSTFIX ${DIME_DEFAULT_POSTFIX})
  set(CMAKE_DEBUG_POSTFIX ${DIME_DEFAULT_POSTFIX}d)

  set(DIME_RELEASE_SYSTEM_LIBRARY_NAME dime${CMAKE_RELEASE_POSTFIX}${DIME_LIBRARY_SUFFIX})
  set(DIME_DEBUG_SYSTEM_LIBRARY_NAME dime${CMAKE_DEBUG_POSTFIX}${DIME_LIBRARY_SUFFIX})
elseif(APPLE)
else()
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)

  set(DIME_RELEASE_SYSTEM_LIBRARY_NAME "$<TARGET_FILE_NAME:dime>")
  set(DIME_DEBUG_SYSTEM_LIBRARY_NAME "$<TARGET_FILE_NAME:dime>")
endif()

if(DIME_BUILD_SHARED_LIBS)
  add_library(${PROJECT_NAME} SHARED ${COMMON_SRCS} ${COMMON_HDRS})
else()
  add_library(${PROJECT_NAME} STATIC ${COMMON_SRCS} ${COMMON_HDRS})
endif()

if(WIN32 AND NOT MINGW)
  set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${DIME_SO_VERSION})
else()
  set_target_properties(${PROJECT_NAME} PROPERTIES VERSION "${DIME_SO_VERSION}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}" SOVERSION ${DIME_SO_VERSION})
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE HAVE_CONFIG_H DIME_DEBUG=$<CONFIG:Debug>)

if(WIN32)
  if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_NONSTDC_NO_DEPRECATE _CRT_SECURE_NO_DEPRECATE _CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES)
  endif()
  if(DIME_BUILD_SHARED_LIBS)
    configure_file(${PROJECT_NAME}.rc.cmake.in ${PROJECT_NAME}.rc)
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.rc")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DIME_MAKE_DLL INTERFACE DIME_DLL)
  else()
    target_compile_definitions(${PROJECT_NAME} INTERFACE DIME_NOT_DLL)
  endif()
else()
  target_link_libraries(${PROJECT_NAME} m)
endif()

target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

option(DIME_BUILD_EXAMPLES "Whether to build executable examples" OFF)
if (DIME_BUILD_EXAMPLES)
    add_executable(dxf2vrml dxf2vrml/dxf2vrml.cpp)
    target_link_libraries(dxf2vrml PRIVATE dime)

    add_executable(dxfsphere dxfsphere/dxfsphere.cpp)
    target_link_libraries(dxfsphere PRIVATE dime)
endif()

# ############################################################################
# Add a target to generate API documentation with Doxygen
# ############################################################################

if(DIME_BUILD_DOCUMENTATION)
  find_package(Doxygen)
  if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation.")
  endif()

# ############################################################################
# Setup documentation options
# ############################################################################
  set(GENERATE_HTMLHELP NO)
  set(DOXYGEN_GENERATE_MAN NO)
  set(GENERATE_QHP NO)
  set(GENERATE_TREEVIEW YES)
  set(HHC_PROGRAM)
  if(WIN32)
    if(DIME_BUILD_DOC_CHM)
      find_program(HHC_PROGRAM NAMES hhc.exe PATHS "C:/Program Files/HTML Help Workshop" "C:/Program Files (x86)/HTML Help Workshop" DOC "HTML Help Compiler program")
      if(NOT HHC_PROGRAM)
        message(FATAL_ERROR "Missing program HTML Help Compiler")
      else()
        set(GENERATE_HTMLHELP YES)
        set(GENERATE_TREEVIEW NO)
        mark_as_advanced(HHC_PROGRAM)
      endif()
    endif()
  else()
    if(DIME_BUILD_DOC_MAN)
      set(DOXYGEN_GENERATE_MAN YES)
    endif()
  endif()
  if(DIME_BUILD_DOC_QTHELP)
    find_program(QHG_LOCATION NAMES qhelpgenerator qhelpgenerator-qt5 DOC "Qt qhelpgenerator")
    if(NOT QHG_LOCATION)
      message(FATAL_ERROR "Missing program Qt qhelpgenerator")
    else()
      set(GENERATE_QHP YES)
      mark_as_advanced(QHG_LOCATION)
    endif()
  endif()

  set(DOXYGEN_INTERNAL_DOCS NO)
  set(DOXYGEN_EXTRACT_PRIVATE NO)
  set(DOXYGEN_WARN_IF_UNDOCUMENTED YES)
  set(DOXYGEN_EXCLUDE)

  set(DOXYFILE "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
  set(DOXYGEN_OUTPUT "${CMAKE_BINARY_DIR}/html/index.html")
  configure_file("${CMAKE_SOURCE_DIR}/docs/dime.doxygen.cmake.in" ${DOXYFILE} @ONLY)

# ############################################################################
# Setup documentation targets
# ############################################################################
  add_custom_command(
    OUTPUT ${DOXYGEN_OUTPUT}
    COMMAND ${CMAKE_COMMAND} -E echo_append "Generating API documentation with Doxygen "
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
    COMMAND ${CMAKE_COMMAND} -E echo "done."
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    DEPENDS ${DOXYFILE}
  )
  add_custom_target(documentation ALL DEPENDS ${DOXYGEN_OUTPUT})

# ############################################################################
# Install built documentation files
# ############################################################################
  install(DIRECTORY "${CMAKE_BINARY_DIR}/html" DESTINATION ${CMAKE_INSTALL_DOCDIR} COMPONENT documentation REGEX ".*\\.(chm|qch)" EXCLUDE)
  if(GENERATE_HTMLHELP)
    install(FILES "${CMAKE_BINARY_DIR}/html/${PROJECT_NAME}.chm" DESTINATION ${CMAKE_INSTALL_DOCDIR} COMPONENT documentation)
  endif()
  if(GENERATE_QHP)
    install(FILES "${CMAKE_BINARY_DIR}/html/${PROJECT_NAME}.qch" DESTINATION ${CMAKE_INSTALL_DOCDIR} COMPONENT documentation)
  endif()
  if(DOXYGEN_GENERATE_MAN)
    install(DIRECTORY "${CMAKE_BINARY_DIR}/man/man3" DESTINATION ${CMAKE_INSTALL_MANDIR} COMPONENT documentation)
    message(STATUS "CMAKE_INSTALL_MANDIR ${CMAKE_INSTALL_MANDIR}")
  endif()
endif()

# ############################################################################
# Packaging prerequisites
# ############################################################################

set(PACKAGE ${PROJECT_NAME})
set(PACKAGE_DESCRIPTION "A library for reading, constructing, manipulating, and writing DXF file data")
set(PACKAGE_BUGREPORT "coin-support@coin3d.org")
set(PACKAGE_NAME ${PROJECT_NAME})
set(PACKAGE_STRING "${PROJECT_NAME} ${PROJECT_VERSION}")
set(PACKAGE_TARNAME ${PROJECT_NAME_LOWER})
set(PACKAGE_URL "https://github.com/coin3d/${PROJECT_NAME_LOWER}")
set(PACKAGE_VERSION ${PROJECT_VERSION})
set(PACKAGE_HOST ${CMAKE_HOST_SYSTEM_PROCESSOR}-${CMAKE_HOST_SYSTEM_NAME})
set(PACKAGE_COMPILER ${CMAKE_CXX_COMPILER})
set(PACKAGE_REQUIREMENTS "dime")

configure_file(include/config.h.cmake.in config.h)

# ############################################################################
# Install headers and binaries
# ############################################################################
if (DIME_ENABLE_INSTALL)
    install(DIRECTORY include/${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT development FILES_MATCHING PATTERN "*.h")
    install(TARGETS ${PROJECT_NAME}  EXPORT ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT runtime
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT runtime NAMELINK_SKIP
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT development
    )

    if(DIME_BUILD_EXAMPLES)
        install(TARGETS dxf2vrml dxfsphere EXPORT ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT runtime
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT runtime NAMELINK_SKIP
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT development
        )
    endif()
    if(DIME_BUILD_SHARED_LIBS)
        install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT development NAMELINK_ONLY)
        if(MSVC)
            install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION ${CMAKE_INSTALL_BINDIR} CONFIGURATIONS Debug RelWithDebInfo COMPONENT debug)
        endif()
    endif()

    # ############################################################################
    # Install pkgconfig file and CMake config package
    # ############################################################################

    if(UNIX OR MINGW)
      configure_file(${CMAKE_SOURCE_DIR}/${PROJECT_NAME_LOWER}.pc.cmake.in ${CMAKE_BINARY_DIR}/${PROJECT_NAME_LOWER}.pc @ONLY)
      install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME_LOWER}.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
    endif()

    configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/${PROJECT_NAME_LOWER}-config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_LOWER}-config.cmake"
      INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
      PATH_VARS CMAKE_INSTALL_LIBDIR
    )

    write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_LOWER}-config-version.cmake"
      VERSION ${PROJECT_VERSION}
      COMPATIBILITY SameMajorVersion
    )

    # To make the component usable not only from the install directory but also from the build directory
    export(
      TARGETS ${PROJECT_NAME}
      FILE ${PROJECT_NAME_LOWER}-export.cmake
    )

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_LOWER}-config.cmake" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_LOWER}-config-version.cmake"
      DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
      COMPONENT development
    )

    install(EXPORT ${PROJECT_NAME}
      DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
      NAMESPACE ${PROJECT_NAME}::
      FILE ${PROJECT_NAME_LOWER}-export.cmake
      COMPONENT development
    )
endif()

