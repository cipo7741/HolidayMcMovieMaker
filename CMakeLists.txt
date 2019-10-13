cmake_minimum_required(VERSION 3.7)
project(HolidayMcMovieMaker)

set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

include_directories( ${Boost_INCLUDE_DIRS} )
include_directories( ${OpenCV_INCLUDE_DIRS} )

include_directories( src )
include_directories( src/thirdparty )

set(SOURCE_FILES
        src/HolidayMcMovieMaker.hpp
        src/HolidayMcMovieMaker.cpp
        src/main.cpp)

find_package( OpenCV REQUIRED )
find_package( Boost REQUIRED COMPONENTS system chrono date_time filesystem)

get_property(inc_dirs DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
message("inc_dirs = ${inc_dirs}")

add_executable(HolidayMcMovieMaker ${SOURCE_FILES})

target_link_libraries(HolidayMcMovieMaker ${OpenCV_LIBS})
target_link_libraries(HolidayMcMovieMaker ${Boost_LIBRARIES})
#target_link_libraries( HolidayVideoMaker thirdparty )

#-lpthread -lm -DBOOST_LOG_DYN_LINK -lboost_log -lboost_thread -lpthread)
