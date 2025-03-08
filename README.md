# Triangles3D

Computational task aimed to get intersections of triangles
in 3D space.

## Prerequisites

To build you need the following installed:

Ensure that you installed with your package manager the following:

* Git
* CMake
* C++ compiler + libstdc++ with support of at least C++17
* Python 3
* `pip`

## Build

After installing dependencies listed above, follow these steps:

1. Clone:

   ```sh
   git clone https://github.com/victorbaldin56/Triangles3D.git
   cd Triangles3D
   ```

1. Create virtual environment for Python:

   ```sh
   python3 -m venv .venv
   source .venv/bin/activate
   ```

1. Install Conan:

   ```sh
   pip3 install conan
   conan profile detect --force
   ```

1. Install Conan requirements and toolchain:

   ```sh
   conan install . --output-folder=build --build=missing
   ```

1. Configure CMake:

   ```sh
   cd build
   ```

   * CMake >= 3.23
   ```
   cmake .. --preset conan-release
   ```

   * CMake < 3.23
   ```
   cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
   ```

2. Build:

   ```sh
   cmake --build . -j
   ```

## Usage

Input:

* $N$ - the number of triangles
* $3N$ points, coordinates are single-precision floating point numbers.
