cmake_minimum_required(VERSION 3.22)
project(OpenCVTest)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(cmake_minimum_required 11)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O0 -Wall -g")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wl,-z,stack-size=1024000000")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")



set(CMAKE_CXX_COMPILER "g++")

set(Ximgproc_DIR "ximgproc/include" "ximgproc/src" "inc" "ximgproc/include/opencv2")
set(SRC_DIR ./ximgproc/src)

list(APPEND CMAKE_PREFIX_PATH "/home/lynliam/Desktop/opencv_cpptest/build") # 安装路径前缀
# for opencv
find_package(OpenCV 4.8.0 REQUIRED)
message("OpenCV version: ${OpenCV_VERSION}")

include_directories(${OpenCV_INCLUDE_DIRS} ${Ximgproc_DIR})
file(GLOB_RECURSE SRC_FILES ${SRC_DIR}/*.cpp "src/*.cpp")
link_directories(${OpenCV_LIB_DIR})

add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
