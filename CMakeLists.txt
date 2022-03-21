cmake_minimum_required(VERSION 3.12)
project(laurent_final_project)
set(BOOST_ROOT C:/boost/boost_1_76_0)

find_package(Boost 1.76.0)
set(CMAKE_CXX_STANDARD 14)

add_executable(laurent_final_project main.cpp classes/my_unit_test.cpp classes/my_unit_test.h classes/AdmenmentUpdate.h classes/OrderManager.cpp classes/OrderManager.h classes/MarketSimulator.cpp classes/MarketSimulator.h classes/TradingStrategy.cpp classes/TradingStrategy.h classes/Order.cpp classes/Order.h classes/BookUpdate.cpp classes/BookUpdate.h classes/AppBase.cpp classes/AppBase.h classes/BookBuilder.cpp classes/BookBuilder.h classes/SmartPointer.cpp classes/SmartPointer.h)

find_package(Boost)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()


add_subdirectory(classes)
add_subdirectory(goog_test)
target_link_libraries(ProjectClass)
