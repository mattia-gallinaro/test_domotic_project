cmake_minimum_required(VERSION 2.84 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_C_FLAGS_RELEASE "-O2")
set(CMAKE_BUILD_TYPE Release)
project(Progetto_Domotica)

include_directories(include)

add_library(${PROJECT_NAME} SHARED
	    src/Device.cpp
            src/Manual.cpp
            src/ProgrammedCycle.cpp
            src/ControlSystem.cpp)

add_executable(main src/main.cpp)

target_link_libraries(main ${PROJECT_NAME})
