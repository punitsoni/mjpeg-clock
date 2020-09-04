# MJPEG Clock

This is a self-contained HTTP server written in C++ that streams a generated
motion jpeg video stream to browser.


## Build and Run

We use CMake build system with conan package manager. Make sure cmake and conan
are installed and in your $PATH. Then use following commands to build and run.

```
# source the useful workflow commands
source envsetup.sh

# Install dependencies and configure build
configure

# Build targets
build

# Run server
run

```

## View video stream in browser

After server is up you can view the stream in browser at http://localhost:8000

