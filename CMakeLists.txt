cmake_minimum_required(VERSION 3.2)
project(RxCppGuiThreaded)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 17)

add_executable(RxCppGuiThreaded 
  src/main.cpp
  )

target_link_libraries(RxCppGuiThreaded ${CONAN_LIBS})
