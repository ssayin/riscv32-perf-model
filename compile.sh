#!/bin/sh

cmake -B build
make -C build
./build/bin/perf_model

