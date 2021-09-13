cmake_minimum_required(VERSION 3.2)
project(Alpex)

#find_package(Sanitizers)

# by default build optimized code with debug symbols
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  # set(CMAKE_BUILD_TYPE RELWITHDEBINFO)
  set(CMAKE_BUILD_TYPE Release)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# require C++11-capable compiler
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.9")
    message(WARNING "Compiler with incomplete C++11 support detected: functionality limited")
    set(OLD_CXX true)
  endif()
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  message("clang, undefined dynamic_lookup")
  set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -undefined dynamic_lookup")
endif()

find_package(ROOT)

# set C++ standard ensuring compatibility with ROOT
set(CMAKE_CXX_STANDARD 11)
if(ROOT_cxx17_FOUND)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
elseif(ROOT_cxx14_FOUND)
  set(CMAKE_CXX_STANDARD 14)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
elseif(ROOT_cxx11_FOUND)
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()
message(STATUS "Using C++${CMAKE_CXX_STANDARD}")

add_compile_options("-Wall")
add_compile_options("-Wpedantic")
add_compile_options("-Wno-unused-variable")
add_compile_options("-Wunused-variable")
add_compile_options("-Wno-uninitialized")

# check for build in the source tree
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(STATUS "Building in source tree")
  set(BUILDINSOURCE true)
endif()

message(STATUS "In Project Directory: ${PROJECT_SOURCE_DIR}")
message(STATUS "ROOT Include Directory: ${ROOT_INCLUDE_DIRS}")

file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hpp)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)

file(COPY ${PROJECT_SOURCE_DIR}/include DESTINATION ${CMAKE_BINARY_DIR} )
file(COPY ${PROJECT_SOURCE_DIR}/include/AXStyle.cpp DESTINATION ${CMAKE_BINARY_DIR} )
file(COPY ${PROJECT_SOURCE_DIR}/rootlogon.C DESTINATION ${CMAKE_BINARY_DIR} )

message(STATUS "found inc ${headers}")
message(STATUS "found src ${sources}")

add_library(alpex SHARED ${sources})

target_link_libraries(alpex PUBLIC ROOT::Core ROOT::Gpad ROOT::Hist ROOT::TreePlayer ROOT::Tree)
#target_link_libraries(alpex PUBLIC )

include_directories(include macro)

add_subdirectory(exe)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Data)
