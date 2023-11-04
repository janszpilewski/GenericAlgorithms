project(${GENERIC_ALGOS_LIB})
cmake_minimum_required(VERSION 3.5)

aux_source_directory(. SRC_LIST)
add_library(${PROJECT_NAME} STATIC ${SRC_LIST})

