project(OrderBook)

add_definitions(-std=gnu++11)

set(HEADER_FILES OrderBook.h)
set(SOURCE_FILES OrderBook.cpp)

add_library(OrderBook STATIC  ${SOURCE_FILES} ${HEADER_FILE})


set(MY_HEADER_FILES MyOrderBook.h)
set(MY_SOURCE_FILES MyOrderBook.cpp PriceUpdate.cpp BookUpdate.cpp)

add_library(MyOrderBook STATIC  ${MY_SOURCE_FILES} ${MY_HEADER_FILE})

