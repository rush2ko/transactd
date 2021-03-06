##=================================================================
#   Copyright (C) 2012 2013 BizStation Corp All rights reserved.
#
#   This program is free software; you can redistribute it and/or
#   modify it under the terms of the GNU General Public License
#   as published by the Free Software Foundation; either version 2
#   of the License, or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software 
#   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  
#   02111-1307, USA.
##=================================================================
##
## Transactd C++ client
##
## *** original file is ../tdclcpp/CMakeLists.txt ***
##
cmake_minimum_required(VERSION 2.6)
include(../common/transactd_required.cmake)
include(../common/transactd_common.cmake)
include(../common/transactd_cl_common.cmake)
include(../common/options.cmake)
include(../common/system.cmake)
include(../common/get_boost_libs.cmake)
include(../common/check_for_link_iconv.cmake)
check_for_link_iconv()


# ==========================================================
# check project name and options
# ==========================================================
get_filename_component(this_target "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
string(TOLOWER "${this_target}" this_target)
project(${this_target})
# m:multibyte u:unicode mr:multibyte-use_rtm
if("${this_target}" STREQUAL "tdclcppm")
  set(TDCL_CPP_CHARSET m)
  set(TDCL_CPP_USE_RUNTIME_LIBS OFF)
elseif("${this_target}" STREQUAL "tdclcppu")
  set(TDCL_CPP_CHARSET u)
  set(TDCL_CPP_USE_RUNTIME_LIBS OFF)
elseif("${this_target}" STREQUAL "tdclcppmr")
  set(TDCL_CPP_CHARSET m)
  set(TDCL_CPP_USE_RUNTIME_LIBS ON)
else()
  set(TDCL_CPP_USE_RUNTIME_LIBS OFF)
endif()


# ==========================================================
# options
# ==========================================================
if(NOT TRANSACTD_CLIENTS_PREFIX)
  set(TRANSACTD_CLIENTS_PREFIX "" CACHE STRING "Prefix for transactd client libraries")
endif()


# ==========================================================
# set variables
# ==========================================================
if("${CMAKE_CURRENT_BINARY_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  get_filename_component(TRANSACTD_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)
  set(TRANSACTD_BINARY_ROOT "${CMAKE_BINARY_DIR}")
endif()


# ==========================================================
# add source / include dir , set compiler / linker options
# ==========================================================
tdcl_add_source_files("${TRANSACTD_ROOT}")
tdcl_add_include_directory("${TRANSACTD_ROOT}")


# ==========================================================
# add resource file (for Windows Visual Studio)
# ==========================================================
set(${this_target}_RC_FILE "${TRANSACTD_ROOT}/build/tdclcpp/tdclcpp.rc")
transactd_read_rc("${${this_target}_RC_FILE}")
if(WIN32)
  transactd_add_rc_file()
endif()


# ==========================================================
# add_library
# ==========================================================
add_library(${this_target} SHARED ${${this_target}_SOURCE_FILES})

# ==========================================================
# add dependencie projects
# ==========================================================
add_dependencies(${this_target} tdclc)


# ==========================================================
# Add compile options
# ==========================================================
transactd_reset_MTMD()
if (MSVC)
  if((TRANSACTD_RUBY_GEM) OR (TRANSACTD_PHP))
    bz_add_cxx_flag("/MD" "_RELEASE")
    bz_add_cxx_flag("/MDd" "_DEBUG")
    bz_add_cxx_flag("/MD" "_RELWITHDEBINFO")
    bz_add_cxx_flag("/MD" "_MINSIZEREL")
  else()
    bz_add_cxx_flag("/MT" "_RELEASE")
    bz_add_cxx_flag("/MTd" "_DEBUG")
    bz_add_cxx_flag("/MT" "_RELWITHDEBINFO")
    bz_add_cxx_flag("/MD" "_MINSIZEREL")
  endif()
  foreach(build_type "" "_RELEASE" "_DEBUG" "_RELWITHDEBINFO" "_MINSIZEREL")
    bz_add_cxx_flag("/wd4996" "${build_type}")
  endforeach()
else()
  bz_add_cxx_flag("-Wno-unknown-pragmas -Wno-ignored-qualifiers" "_RELEASE")
  bz_add_cxx_flag("-Wno-unknown-pragmas -Wno-ignored-qualifiers" "_DEBUG")
  bz_add_cxx_flag("-Wno-unknown-pragmas -Wno-ignored-qualifiers" "_RELWITHDEBINFO")
  bz_add_cxx_flag("-Wno-unknown-pragmas -Wno-ignored-qualifiers" "_MINSIZEREL")
endif()

# ==========================================================
# Add compile definitions
# ==========================================================
if(MSVC)
  set_property(TARGET ${this_target} PROPERTY COMPILE_DEFINITIONS
    WIN32 _WIN32 _WINDOWS _USRDLL BOOST_ALL_NO_LIB 
    LIB_TDCLCPP TRDCL_AUTOLINK_OFF TRDCLENGN_EXPORTS _CRT_SECURE_NO_WARNINGS)
else()
  set_property(TARGET ${this_target} PROPERTY COMPILE_DEFINITIONS
    PIC _REENTRANT BOOST_ALL_NO_LIB)
  if(NOT MINGW)
    set_property(TARGET ${this_target} APPEND PROPERTY COMPILE_DEFINITIONS
      LINUX)
  else()
    set_property(TARGET ${this_target} APPEND PROPERTY COMPILE_DEFINITIONS
      WIN32 _WIN32 _SYS_TIME_H_ _GETTIMEOFDAY_DEFINED)
  endif()
endif()
if(TRANSACTD_RUBY_GEM OR TRANSACTD_PHP)
  set_property(TARGET ${this_target} APPEND PROPERTY COMPILE_DEFINITIONS
    SWIG_BUILDING)
endif()
if("${TDCL_CPP_CHARSET}" STREQUAL "u")
  set_property(TARGET ${this_target} APPEND PROPERTY COMPILE_DEFINITIONS
    UNICODE _UNICODE)
else()
  set_property(TARGET ${this_target} APPEND PROPERTY COMPILE_DEFINITIONS
    _MBCS)
endif()


# ==========================================================
# add link / include
# ==========================================================
transactd_link_boost_libraries("filesystem;thread;chrono;system")
if(CAN_LINK_ICONV)
  target_link_libraries(${this_target} "${ICONV_LIBRARY}")
endif()
if(NOT WIN32)
  target_link_libraries(${this_target} dl pthread)
endif()


# ==========================================================
# set binary name
# ==========================================================
bz_get_compiler_bitness()
if(WIN32)
  ## Windows tdclcpp[_Compiler]_[32|64][m|u|mr]_[MAJOR]_[MINOR].dll
  bz_get_compiler()
  set(TDCL_CPP_NAME_TMP "tdclcpp${BZ_COMPILER}_${BZ_BITNESS}${TDCL_CPP_CHARSET}")
  
  foreach(BUILD_TYPE DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
    transactd_has_MD_option("${CMAKE_CXX_FLAGS_${BUILD_TYPE}}")
    if("${IS_MD}" STREQUAL "ON")
      set_target_properties(${this_target} PROPERTIES OUTPUT_NAME_${BUILD_TYPE} "${TDCL_CPP_NAME_TMP}r_${TD_RC_VER_MAJOR}_${TD_RC_VER_MINOR}")
    else()
      set_target_properties(${this_target} PROPERTIES OUTPUT_NAME_${BUILD_TYPE} "${TDCL_CPP_NAME_TMP}_${TD_RC_VER_MAJOR}_${TD_RC_VER_MINOR}")
    endif()
  endforeach()
  set_target_properties(${this_target} PROPERTIES PREFIX "")
  set_target_properties(${this_target} PROPERTIES SUFFIX ".dll")
else()
  ## Linux   libtdclcpp_[32|64][m|u].so.[MAJOR].[MINOR].[RELEASE]
  set_target_properties(${this_target} PROPERTIES PREFIX "lib")
  set_target_properties(${this_target} PROPERTIES OUTPUT_NAME
    "tdclcpp_${BZ_BITNESS}${TDCL_CPP_CHARSET}")
  if(APPLE)
  ## Apple   libtdclcpp_[32|64][m|u].[MAJOR].[MINOR].[RELEASE].dylib
    set_target_properties(${this_target} PROPERTIES SUFFIX ".dylib")
  else()
    set_target_properties(${this_target} PROPERTIES SUFFIX ".so")
  endif()
  set_target_properties(${this_target} PROPERTIES
    SOVERSION ${TD_RC_VER_MAJOR}.${TD_RC_VER_MINOR}
    VERSION ${TD_RC_VER_MAJOR}.${TD_RC_VER_MINOR}.${TD_RC_VER_RELEASE})
endif()


# ==========================================================
# set output directory
# ==========================================================
if(NOT COMMAND tdcl_set_output)
  if(TRANSACTD_RUBY_GEM)
    include(../tdclrb/gem_output.cmake)
  else()
    include(../common/transactd_cl_output.cmake)
  endif()
endif()
tdcl_set_output("${TRANSACTD_BINARY_ROOT}" "${TRANSACTD_CLIENTS_PREFIX}")
