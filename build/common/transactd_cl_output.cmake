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
# ==========================================================
#   tdcl_set_output
# ==========================================================
if(NOT COMMAND tdcl_set_output)
macro(tdcl_set_output TRANSACTD_BINARY_ROOT prefix)
  if(WIN32)
    set_target_properties(${this_target} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${TRANSACTD_BINARY_ROOT}/bin"
      LIBRARY_OUTPUT_DIRECTORY "${TRANSACTD_BINARY_ROOT}/bin"
      ARCHIVE_OUTPUT_DIRECTORY "${TRANSACTD_BINARY_ROOT}/lib")
    if(MSVC)
      transactd_make_msvc_install_file("${TRANSACTD_BINARY_ROOT}")
      add_custom_command(TARGET ${this_target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -P
        ${CMAKE_CURRENT_BINARY_DIR}/msvc_install_$<CONFIGURATION>.cmake)
    endif()
  else()
    if("${prefix}" STREQUAL "")
      install(TARGETS ${this_target} LIBRARY DESTINATION /usr/lib)
    else()
      install(TARGETS ${this_target} LIBRARY DESTINATION "${prefix}")
    endif()
  endif()
endmacro()
endif()


# ==========================================================
#   transactd_set_exe_output
# ==========================================================
if(NOT COMMAND transactd_set_exe_output)
macro(transactd_set_exe_output TRANSACTD_BINARY_ROOT prefix)
  if(WIN32)
    set_target_properties(${this_target} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${TRANSACTD_BINARY_ROOT}/bin")
    if(MSVC)
      transactd_make_msvc_install_file("${TRANSACTD_BINARY_ROOT}")
      add_custom_command(TARGET ${this_target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -P
        ${CMAKE_CURRENT_BINARY_DIR}/msvc_install_$<CONFIGURATION>.cmake)
    endif()
  else()
    if("${prefix}" STREQUAL "")
      install(TARGETS ${this_target} RUNTIME DESTINATION /usr/local/transactd/bin)
    else()
      install(TARGETS ${this_target} RUNTIME DESTINATION "${prefix}/bin")
    endif()
  endif()
endmacro()
endif()


# ==========================================================
#   transactd_make_msvc_install_file MACRO
# ==========================================================
if(NOT COMMAND transactd_make_msvc_install_file)
macro(transactd_make_msvc_install_file TRANSACTD_BINARY_ROOT)
  get_target_property(${this_target}_BINARY_BASENAME "${this_target}" LOCATION)
  get_filename_component(${this_target}_BINARY_BASENAME "${${this_target}_BINARY_BASENAME}" NAME_WE)
  foreach(build_type "RELEASE" "DEBUG" "RELWITHDEBINFO" "MINSIZEREL")
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/msvc_install_${build_type}.cmake
"
foreach(dirname bin lib)
  file(GLOB dellist \"${TRANSACTD_BINARY_ROOT}/\${dirname}/${${this_target}_BINARY_BASENAME}*\")
  if(NOT \"\${dellist}\" STREQUAL \"\")
    file(REMOVE \${dellist})
  endif()
  file(GLOB copylist
    \"${TRANSACTD_BINARY_ROOT}/\${dirname}/${build_type}/${${this_target}_BINARY_BASENAME}*\")
  file(COPY \${copylist} DESTINATION \"${TRANSACTD_BINARY_ROOT}/\${dirname}/\")
endforeach()
")
  endforeach()
endmacro()
endif()