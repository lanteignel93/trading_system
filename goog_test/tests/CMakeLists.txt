 include_directories(${gtest_SOURCE_DIR}/include $gtest_SOURCE_DIR)
add_executable(RunTests project_test.cpp ../../classes/SmartPointer.cpp ../../classes/SmartPointer.h)
target_link_libraries(RunTests gtest gtest_main ProjectClass)
