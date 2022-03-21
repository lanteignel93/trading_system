project(ProjectClass)

add_definitions(-std=gnu++11)

set(HEADER_FILES AdmenmentUpdate.h AppBase.h BookBuilder.h BookUpdate.h MarketSimulator.h MDReader.h Order.h OrderManager.h TradingStrategy.h)
set(SOURCE_FILES AppBase.cpp BookBuilder.cpp BookUpdate.cpp MarketSimulator.cpp MDReader.cpp Order.cpp OrderManager.cpp TradingStrategy.cpp SmartPointer.cpp SmartPointer.h)

add_library(ProjectClass STATIC  ${SOURCE_FILES} ${HEADER_FILE} SmartPointer.cpp SmartPointer.h)


