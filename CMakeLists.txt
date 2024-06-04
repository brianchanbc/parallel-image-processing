cmake_minimum_required(VERSION 3.11)
project(Parallel-Image-Processing)

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

file(GLOB SOURCES "src/*.cpp")
add_executable(Parallel-Image-Processing ${SOURCES})

target_link_libraries(Parallel-Image-Processing PRIVATE ${OpenCV_LIBS})
target_link_libraries(Parallel-Image-Processing PRIVATE Threads::Threads)