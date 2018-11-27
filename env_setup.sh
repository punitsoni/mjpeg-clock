config() {
    conan install -if build .
    cmake -H. -Bbuild
}

build() {
    cmake --build build
}

cleanall() {
    rm -rf build/
}

run() {
    ./build/copper/bin/copper
}