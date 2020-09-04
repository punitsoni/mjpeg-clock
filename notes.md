

# install conan dependencies

conan install --settings compiler.cppstd=17 --build=missing -if build .

# cmake configure step

cmake -S . -B build

# cmake build step

cmake --build build

ffmpeg store raw mjpeg in mp4 container

ffmpeg -framerate 30 -f mjpeg -i input.mjpeg -vcodec copy output.mp4



