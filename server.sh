#!/usr/bin/bash
cd /home/theshizik3/Desktop/projects/flow/server
cmake -S . -B build
cmake --build build
cd build
./appserver