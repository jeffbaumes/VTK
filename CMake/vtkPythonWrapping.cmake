find_package(PythonLibs REQUIRED)
include(vtkWrapPython)

function(vtk_add_python_wrapping module_name module_srcs module_hdrs)
  if(NOT VTK_WRAP_PYTHON_INIT_EXE)
    message(FATAL_ERROR "VTK must be built with Python wrapping turned on.")
  endif()
  # Need to add the Wrapping/Python to the include directory
  set(_python_include_dirs
    ${VTK_SOURCE_DIR}/Wrapping/Python
    ${VTK_BINARY_DIR}/Wrapping/Python
    ${PYTHON_INCLUDE_DIRS})

  if(NOT CMAKE_HAS_TARGET_INCLUDES)
    include_directories(${_python_include_dirs})
  endif()

  set(KIT_HIERARCHY_FILE ${CMAKE_CURRENT_BINARY_DIR}/${module_name}Hierarchy.txt)
  string(REGEX REPLACE "^vtk" "" kit_name "${module_name}")
  set(KIT ${kit_name})
  set(XY ${PYTHON_MAJOR_VERSION}${PYTHON_MINOR_VERSION})

  # Figure out the dependent PythonXYD libraries for the module
  foreach(dep ${VTK_MODULE_${vtk-module}_DEPENDS})
    if(NOT "${vtk-module}" STREQUAL "${dep}")
      if(NOT VTK_MODULE_${dep}_EXCLUDE_FROM_WRAPPING)
        list(APPEND extra_links ${dep}PythonD)
        list(APPEND VTK_WRAP_INCLUDE_DIRS ${${dep}_SOURCE_DIR})
      endif()
    endif()
  endforeach()

  set(_wrap_files ${module_srcs})
  foreach(hdr ${module_hdrs})
    get_source_file_property(_wrap_header "${hdr}" WRAP_HEADER)
    if(_wrap_header)
      list(APPEND _wrap_files "${hdr}")
    endif()
  endforeach()

  vtk_wrap_python3(${module_name}Python Python_SRCS "${_wrap_files}")
  vtk_add_library(${module_name}PythonD ${Python_SRCS} ${extra_srcs})
  set_target_properties(${module_name}PythonD
    PROPERTIES OUTPUT_NAME ${module_name}Python${XY}D)
  if(CMAKE_HAS_TARGET_INCLUDES)
    set_property(TARGET ${module_name}PythonD APPEND
      PROPERTY INCLUDE_DIRECTORIES ${_python_include_dirs})
  endif()
  if(VTK_MODULE_${module_name}_IMPLEMENTS)
    set_property(TARGET ${module_name}PythonD PROPERTY COMPILE_DEFINITIONS
      "${module_name}_AUTOINIT=1(${module_name})")
  endif()
  target_link_libraries(${module_name}PythonD ${module_name}
    vtkWrappingPython ${extra_links} ${VTK_PYTHON_LIBRARIES})
  python_add_module(${module_name}Python ${module_name}PythonInit.cxx)
  if(PYTHON_ENABLE_MODULE_${module_name}Python)
    target_link_libraries(${module_name}Python ${module_name}PythonD)
    if(CMAKE_HAS_TARGET_INCLUDES)
      set_property(TARGET ${module_name}Python APPEND
        PROPERTY INCLUDE_DIRECTORIES ${_python_include_dirs})
    endif()
  endif()
endfunction()
