cmake_minimum_required(VERSION 3.22...3.25)
include(ExternalProject)

project(
    Pillow 
    VERSION 0.1.0 # any version number
    LANGUAGES CXX C # programming languages used by the project
)

macro(print_all_variables)
    message(STATUS "print_all_variables------------------------------------------{")
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
    message(STATUS "print_all_variables------------------------------------------}")
endmacro()

ExternalProject_Add(glm
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/glm
    URL https://github.com/g-truc/glm/archive/refs/heads/master.zip
    INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/glm
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/${GLM_NAME}
               -DCMAKE_C_COMPILER_WORKS=${CMAKE_C_COMPILER_WORKS} 
               -DCMAKE_C_FLAGS=${GLM_C_FLAGS} 
               -DCMAKE_CXX_FLAGES=${GLM_CXX_FLAGS}
               -DCMAKE_XCODE_EFFECTIVE_PLATFORMS=${CMAKE_XCODE_EFFECTIVE_PLATFORMS} 
               -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
               -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} 
               -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
               -DMSGPACK_CXX17=ON
)

print_all_variables()



add_executable(pillow
    Core/ElementBase.cpp
    Core/SubdivideVertex.cpp
    Core/Vertex.cpp
	main.cpp
)

set_target_properties(pillow PROPERTIES
    CXX_STANDARD 17
    COMPILE_WARNING_AS_ERROR OFF
)

add_dependencies(pillow glm)



function(target_treat_all_warnings_as_errors Target)
	# Treat warnings as errors
	set_target_properties(${Target} PROPERTIES COMPILE_WARNING_AS_ERROR ON)

	# Turn all warnings on
	if (MSVC)
		target_compile_options(${Target} PRIVATE /W4)
	else()
		target_compile_options(${Target} PRIVATE -Wall -Wextra -pedantic)
	endif()
endfunction()

#target_treat_all_warnings_as_errors(pillow)