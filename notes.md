# install conan dependencies

conan install -if build

# cmake configure step

cmake -H. -Bbuild

# cmake build step

cmake --build build
