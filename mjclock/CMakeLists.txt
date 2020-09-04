cmake_minimum_required(VERSION 3.1.0)
project(mjclock)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

# Conan setup
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(KEEP_RPATHS)

message(CONAN_TARGETS= "${CONAN_TARGETS}")
message(CONAN_LIBS= "${CONAN_LIBS}")

add_executable(mjclock)
target_sources(mjclock
    PRIVATE
    mjclock_main.cc
    mongoose.c
    test_video_gen.cc
)
target_link_libraries(mjclock
    ${CONAN_LIBS}
)
set_target_properties(mjclock PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
