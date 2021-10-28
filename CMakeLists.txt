# Root Cmake. Use to link the different libraries
#
# @author  sylvain Colomer, P. Gaussier
# @version 1.0


#########################################################################################################
###                                       GENERAL CONFIG                                               ##
#########################################################################################################

message("%%% SYNTHESIS PROJECT : DRONE COLLECTOR")
message("\n%% GENERAL CONFIGURATION")

# GENERAL SETTINGS
project(DroneCollector CXX)
cmake_minimum_required(VERSION 3.2)
set(VERSION 1.0.0)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

# OUTPUT DIR SETTINGS
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# COMPILER OPTIONS
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG   " -g3 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE " -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") # Warning : option are important for some lib


########################################################################################################
###                                       LIBRARY CONFIG                                              ##
########################################################################################################

message("\n%% EXTERNALS libs configuration")

message("- Checking : pkg")
find_package(PkgConfig REQUIRED)
if(PkgConfig_FOUND)
    message("-> succes " )
endif()

message("- Checking : libpqxx")
find_library(LIB_LIBPQXX pqxx)
if(LIB_LIBPQXX)
    message("-> success ")
else()
    message(FATAL_ERROR "Fail : you must install libpqxx")
endif()

message("- Checking : mavlink")
if(EXISTS ${CMAKE_SOURCE_DIR}/lib/mavlink)
    message("-> succes " )
else()
    message("Fail: You must install mavlink")
endif()
set(MAVLINK_LIB -I/lib/mavlink)
include_directories(./lib/mavlink)


message("- Checking : Loguru")
if(EXISTS ${CMAKE_SOURCE_DIR}/lib/loguru)
   message("-> succes " )
else()
   message("Fail: You must add loguru log system in lib folder")
endif()
set(LOGURU_LIB -I/lib/loguru)
include_directories(./lib/loguru)

message("- Checking : Blc channels")
find_package(blc_channel REQUIRED)
find_package(blc_program REQUIRED)
include_directories( ${BL_INCLUDE_DIRS} )
add_definitions( ${BL_DEFINITIONS} )


message("- Checking : Thread")
find_package(Threads)

# message("- Checking : Curses")
# find_package(Curses REQUIRED)
# include_directories(${CURSES_INCLUDE_DIR})


######################################################################################################
###                                            LINKS                                                ##
######################################################################################################

link_libraries(
    ${BL_LIBRARIES} 
    ${MAVLINK_LIB}
    ${LOGURU_LIB}
    # ${CURSES_LIBRARIES}
    # ${CMAKE_THREAD_LIBS_INIT}
    dl
    panel
)



######################################################################################################
###                                         APPLICATION                                             ##
######################################################################################################

message("\n%% APPLICATION GENERATION")

# %% App files
add_executable(
    DroneCollector

    # libraries to compile
    lib/loguru/loguru.cpp
    lib/loguru/loguru.hpp

    # Misc
    include/global_variables.h

    # Drone
    include/drone/Data_Drone.h
    src/drone/Data_Drone.cpp

    # Network
    include/network/Com_Serial.h
    include/network/Com_Wifi.h
    src/network/Com_Serial.cpp

    # Threads
    include/threads/Abstract_ThreadClass.h
    include/threads/Database_ThreadClass.h
    include/threads/DroneReceiver_ThreadClass.h
    src/threads/Abstract_ThreadClass.cpp
    src/threads/Database_ThreadClass.cpp
    src/threads/DroneReceiver_ThreadClass.cpp

    # entry point
    src/Main.cpp
)

# The libpqxx test app
# add_executable(
#     TestLibpqxx

#     # libraries to compile
#     lib/loguru/loguru.cpp
#     lib/loguru/loguru.hpp

#     # entry point
#     src/TestLibpqxx.cpp
# )

target_link_libraries(DroneCollector -lpqxx -lpq)
# target_link_libraries(TestLibpqxx -lpqxx -lpq)