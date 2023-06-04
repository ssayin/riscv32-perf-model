#!/bin/sh

cmake -B build -G"Unix Makefiles"
make -C build
./build/bin/model

