# install conan dependencies

conan install -if build

# cmake configure step

cmake -H. -Bbuild

# cmake build step

cmake --build build


ffmpeg store raw mjpeg in mp4 container

ffmpeg -framerate 30 -f mjpeg -i input.mjpeg -vcodec copy output.mp4