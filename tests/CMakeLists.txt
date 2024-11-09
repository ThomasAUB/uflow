set(UFLOW_UNIT_TESTS uflow_tests)

include(CTest)

file(GLOB TARGET_SRC "./tests.cpp" )

add_executable(${UFLOW_UNIT_TESTS} ${TARGET_SRC})

add_test(${UFLOW_UNIT_TESTS} ${UFLOW_UNIT_TESTS})