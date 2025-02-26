# Triangles3D

Computational task aimed to get intersections of triangles
in 3D space.

## Prerequisites

To build you need the following installed:

* Git
* CMake
* Any C++ compiler supported by CMake

## Build

To build:

1. Clone repo:

```sh
git clone --recursive https://github.com/victorbaldin56/Triangles3D.git
```

1. Generate build files:

```sh
cmake -S . -B build
```

1. Build:

```sh
cmake --build build
```

## Usage

Input:

* $N$ - the number of triangles
* $3N$ points, coordinates are single-precision floating point numbers (type `float` in C++).
