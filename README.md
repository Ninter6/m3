# m3

> This stuff actually is the 3rd version of my old math lib [mathpls](https://github.com/Ninter6/mathpls).

This is a simple lightweight but useful linear algebra C++ module library.
Template lib as it is, it can also be used as a single-header lib.

`m3.ccm`is all you need.

## donwload

To download the module file, run:

```bash
curl -fsSLO "https://raw.githubusercontent.com/Ninter6/m3/refs/heads/main/m3.ccm"
```

For those who want to use header file, trying this is recommended:

```bash
curl -fsSL "https://raw.githubusercontent.com/Ninter6/m3/refs/heads/main/m3i.sh" | bash -s
```

This will create a `m3.hpp`, so that you can simply `#include` the lib.

## example

See [svd_demo](test/svd_demo/main.cpp), which is a simple app showing a soft spinning cube.

To run it, you can follow the following:

```bash 
git clone https://github.com/Ninter6/m3.git
cd m3
cmake -S . -B build -G Ninja
cmake --build build -j6
./build/test/svd_demo # PRESS ENTER TO START
```
Make sure you have a suitable compiler that supports cpp module.
> Warning like `anonymous structure` is our feature.
