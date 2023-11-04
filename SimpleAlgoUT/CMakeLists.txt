project(${GENERIC_ALGOS_SIMPLE_UT})
cmake_minimum_required(VERSION 3.5)

set(BASIC_UT_BIN BasicUT)
set(SUFFIX_ARRAY_UT_BIN SuffixArrayUT)
set(BINARY_HEAP_UT_BIN BinHeapUT)

add_executable(${BASIC_UT_BIN} SimpleAlgosUT.cpp)
target_link_libraries(${BASIC_UT_BIN} ${GENERIC_ALGOS_LIB})

add_executable(${SUFFIX_ARRAY_UT_BIN} SuffixArrayUT.cpp)
target_link_libraries(${SUFFIX_ARRAY_UT_BIN} ${GENERIC_ALGOS_LIB})

add_executable(${BINARY_HEAP_UT_BIN} HeapUT.cpp)
target_link_libraries(${BINARY_HEAP_UT_BIN} ${GENERIC_ALGOS_LIB})


add_test(NAME BasicUT COMMAND ${BASIC_UT_BIN})
add_test(NAME SuffixArrayUT COMMAND ${SUFFIX_ARRAY_UT_BIN})
add_test(NAME BinHeapUT COMMAND ${BINARY_HEAP_UT_BIN})

