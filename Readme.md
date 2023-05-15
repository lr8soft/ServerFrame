# ServerFrame
ServerFrame is a simple web application framework implemented in modern C++.
This project is still in the early stage of development, and the API is not stable.
## Build
1.Install vcpkg from https://github.com/microsoft/vcpkg

2.Install cmake which version>=VERSION 3.25

2.Clone the project and run the following commands in the project directory:
```
git clone --recurse-submodules https://github.com/lr8soft/ServerFrame.git
cd ServerFrame

cmake -B build -S . "-DCMAKE_TOOLCHAIN_FILE={vcpkg install path}/vcpkg/scripts/buildsystems/vcpkg.cmake"
cd build
cmake ..
make

./ServerFrame
```