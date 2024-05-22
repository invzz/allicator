# Fast Memory Allocator

Memory allocator written in `C` can be used as a library 

## features : 

- fixed size allocator
- Uses RBT trees 

## Dependencies

1. cmake :  used to build

## build :

on Linux just run the script

```
./build.sh <Generator>

```
`Generator` can be "Unix Makefiles" or "Ninja" or whatever build system you want to use together with cmake.

More info [here](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html)

## targets

1. `rbt`     : red black trees implementation as static library
2. `analloc` : memory allocation implementation as static library
3. `main`    : test / example application