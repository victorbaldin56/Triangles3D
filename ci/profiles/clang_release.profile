[settings]
arch=x86_64
os=Linux
compiler=clang
compiler.version=14
compiler.libcxx=libstdc++
compiler.cppstd=gnu17
build_type=Release

[conf]
tools.build:compiler_executables={"cpp": "clang++", "c": "clang", "rc": "clang"}