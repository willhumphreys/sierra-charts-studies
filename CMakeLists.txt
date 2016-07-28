cmake_minimum_required(VERSION 3.5)
project(sierra-charts-studies)

include_directories("ACS_Source")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES lines.cpp ACS_Source/sierrachart.h holdAboveAndBelowStudies.cpp tradeSystemFailAtExtremes.cpp news.cpp simpleStats.cpp)
add_executable(sierra-charts-studies ${SOURCE_FILES})