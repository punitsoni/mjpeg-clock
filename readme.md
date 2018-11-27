We are using conan package manager to manage the external
library dependency for this C++ project. Build system is CMake.

## Setup build system
```
# install cmake
brew install cmake

# install conan package manager
pip install conan

# setup conan remotes
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
```

## Configure and build
```
# load environment
source envsetup.sh

config

build

run
```