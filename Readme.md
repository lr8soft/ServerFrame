# ServerFrame
ServerFrame is a cross-platform web application framework implemented in modern C++.

## Build
1.Install vcpkg from https://github.com/microsoft/vcpkg

2.Install cmake which version>=VERSION 3.25

2.Clone the project and run the following commands in the project directory:
```
git clone https://github.com/lr8soft/ServerFrame.git
cd ServerFrame

cmake -B build -S . "-DCMAKE_TOOLCHAIN_FILE={the path where vcpkg is installed}/vcpkg/scripts/buildsystems/vcpkg.cmake"
cd build
cmake ..
make

./ServerFrame
```