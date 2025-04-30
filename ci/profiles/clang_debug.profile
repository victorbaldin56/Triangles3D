[settings]
arch=x86_64
os=Linux
compiler=clang
compiler.version=14
compiler.libcxx=libc++
compiler.cppstd=gnu17
build_type=Debug

[buildenv]
CC=/usr/bin/clang-14
CXX=/usr/bin/clang++-14
CXXFLAGS=-fsanitize=address,leak,undefined
LDFLAGS=-fsanitize=address,leak,undefined
