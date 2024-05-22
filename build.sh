#!/bin/bash
cmake -S./ -B./build -G $1
cmake --build ./build