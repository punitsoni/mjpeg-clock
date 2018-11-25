cmake_minimum_required(VERSION 3.10)

project(copper)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

message(CONAN_LIBS = "${CONAN_LIBS}")

add_executable(copper copper/copper_main.cc)
set_property(TARGET copper PROPERTY CXX_STANDARD 14)

# target_link_libraries(copper opencv_core opencv_highgui)
target_link_libraries(copper ${CONAN_LIBS})
