cmake_minimum_required(VERSION 3.5)
project (GenericAlgosSolution) # Top project name

# Specific compiler fags

IF ( CMAKE_COMPILER_IS_GNUCXX OR (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang") )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX17_STANDARD_COMPILE_OPTION}")
ENDIF ( CMAKE_COMPILER_IS_GNUCXX OR (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang") )


# Output directories

set(BINARY_DIR ${CMAKE_SOURCE_DIR}/Build)
set(EXECUTABLE_OUTPUT_PATH ${BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${BINARY_DIR})
 

enable_testing() # UT enabled

# Subprojects

# Subproject names
set (GENERIC_ALGOS_LIB GenericAlgos)
set (GENERIC_ALGOS_SIMPLE_UT SimpleAlgoUT)
set (PERMUTATION_VISUALIZER PermutationVisualizer)

# Global include
set(PROJECT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/${GENERIC_ALGOS_LIB})
include_directories(${PROJECT_INCLUDE_DIR})


add_subdirectory(${GENERIC_ALGOS_LIB})
add_subdirectory(${GENERIC_ALGOS_SIMPLE_UT})
add_subdirectory(${PERMUTATION_VISUALIZER})
