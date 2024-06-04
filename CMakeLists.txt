cmake_minimum_required(VERSION 3.27)
project(ManEater2)

set(CMAKE_CXX_STANDARD 20)


set(YAML_CPP_INCLUDE_DIR "C:/msys64/mingw64/include")
set(YAML_CPP_LIBRARIES "C:/msys64/mingw64/lib/libyaml-cpp.dll.a") # or libyaml-cpp.so for shared lib
include_directories("C:/Users/sarantis/OneDrive/Desktop/tabulate-master/include")


include_directories(${YAML_CPP_INCLUDE_DIR})
link_directories(${YAML_CPP_LIBRARIES})




add_executable(ManEater2 main.cpp
        base.cpp
        headers.h
        config.h
        config.cpp
        useful.h
        useful.cpp
        freegames.cpp
        stats.cpp
        globals.h
)

target_link_libraries(ManEater2 ${YAML_CPP_LIBRARIES})
