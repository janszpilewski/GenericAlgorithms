project(${PERMUTATION_VISUALIZER})
cmake_minimum_required(VERSION 3.5)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${GENERIC_ALGOS_LIB})

