#!/usr/bin/bash
cd /home/theshizik3/Desktop/projects/flow/client
cmake -S . -B build
cmake --build build
cd build
./appclient