configure() {
    conan install -if build .
    cmake -H. -Bbuild
}

build() {
    cmake --build build
}

run() {
    ./build/bin/copper
}