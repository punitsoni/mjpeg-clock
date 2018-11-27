project(copper)
cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_VERBOSE_MAKEFILE TRUE)

# Conan setup
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(KEEP_RPATHS)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed (for Qt)
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files (for Qt)
# TODO: this does not work in cmake 3.13
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)

add_executable(copper copper_main.cc)
target_link_libraries(copper ${CONAN_LIBS} Qt5::Widgets Qt5::Gui)
set_target_properties(copper PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
