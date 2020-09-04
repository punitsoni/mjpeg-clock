configure() {
  # Install dependencies via conan
  conan install \
    --settings compiler.cppstd=17 \
    --build=missing \
    -if build .
  # CMake generate build system
  cmake -S . -B build
}

build() {
  # Build targets
  cmake --build build
}

clean() {
    rm -rf build/
}

run() {
    ./build/bin/mjclock
}

