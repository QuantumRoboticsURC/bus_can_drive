cmake_minimum_required(VERSION 3.0.2)
project(bus_can_drive)
add_definitions(-std=c++11)
## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/src/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/src)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/src/lib)
set(THREADS_PREFER_PTHREAD_FLAG ON)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
)



catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES bus_can_drive
#  CATKIN_DEPENDS roscpp rospy std_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)


find_package (Threads REQUIRED)
find_package(SDL2 REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/src/include")

add_executable(drive_can ${PROJECT_SOURCE_DIR}/src/drive_can.cpp)
if (${CMAKE_LIBRARY_ARCHITECTURE} STREQUAL "arm-linux-gnueabihf") # Jetson TK1 / Pi
    target_link_libraries(drive_can ${CMAKE_SOURCE_DIR}/bus_can_drive/src/lib/raspberry/libCTRE_Phoenix.a)
    target_link_libraries(drive_can ${CMAKE_SOURCE_DIR}/bus_can_drive/src/lib/raspberry/libCTRE_PhoenixCCI.a)
	target_link_libraries(drive_can ${CMAKE_SOURCE_DIR}/bus_can_drive/src/lib/raspberry/libCTRE_PhoenixDiagnostics.a)
    target_link_libraries(drive_can ${CMAKE_SOURCE_DIR}/bus_can_drive/src/lib/raspberry/libCTRE_PhoenixPlatform_socketcan.a)
    target_link_libraries(drive_can ${CMAKE_SOURCE_DIR}/bus_can_drive/src/lib/raspberry/libCTRE_PhoenixCanutils.a)
    target_link_libraries(drive_can ${CMAKE_SOURCE_DIR}/bus_can_drive/src/lib/raspberry/libCTRE_PhoenixCore.a)
elseif (${CMAKE_LIBRARY_ARCHITECTURE} STREQUAL "aarch64-linux-gnu") # Jetson TX2
    target_link_libraries(drive_can ${CMAKE_SOURCE_DIR}/bus_can_drive/src/lib/jetsontx/libCTRE_Phoenix.a)
    target_link_libraries(drive_can ${CMAKE_SOURCE_DIR}/bus_can_drive/src/lib/jetsontx/libCTRE_PhoenixCCI.a)
    target_link_libraries(drive_can ${CMAKE_SOURCE_DIR}/bus_can_drive/src/lib/jetsontx/libCTRE_PhoenixDiagnostics.a)
    target_link_libraries(drive_can ${CMAKE_SOURCE_DIR}/bus_can_drive/src/lib/jetsontx/libCTRE_PhoenixPlatform_socketcan.a)
    target_link_libraries(drive_can ${CMAKE_SOURCE_DIR}/bus_can_drive/src/lib/jetsontx/libCTRE_PhoenixCanutils.a)
    target_link_libraries(drive_can ${CMAKE_SOURCE_DIR}/bus_can_drive/src/lib/jetsontx/libCTRE_PhoenixCore.a)
else()
    target_link_libraries(drive_can ${CMAKE_SOURCE_DIR}/bus_can_drive/src/lib/x86-64/libCTRE_Phoenix.a)
    target_link_libraries(drive_can ${CMAKE_SOURCE_DIR}/bus_can_drive/src/lib/x86-64/libCTRE_PhoenixCCI.a)
    target_link_libraries(drive_can ${CMAKE_SOURCE_DIR}/bus_can_drive/src/lib/x86-64/libCTRE_PhoenixDiagnostics.a)
    target_link_libraries(drive_can ${CMAKE_SOURCE_DIR}/bus_can_drive/src/lib/x86-64/libCTRE_PhoenixPlatform_socketcan.a)
    target_link_libraries(drive_can ${CMAKE_SOURCE_DIR}/bus_can_drive/src/lib/x86-64/libCTRE_PhoenixCanutils.a)
    target_link_libraries(drive_can ${CMAKE_SOURCE_DIR}/bus_can_drive/src/lib/x86-64/libCTRE_PhoenixCore.a)
endif()

#add_executable(drive_can src/drive_can.cpp)
target_link_libraries(drive_can ${catkin_LIBRARIES})
target_link_libraries(drive_can Threads::Threads)
target_link_libraries(drive_can ${SDL2_LIBRARIES})


