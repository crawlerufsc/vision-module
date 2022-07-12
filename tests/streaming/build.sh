#! /bin/sh
rm -rf build
mkdir  -p build
cd build && cmake ..
make
build/stream_tst 10.0.0.144 20000 20000
build/stream_tst 10.0.0.144 20001 20001
build/stream_tst 10.0.0.144 20002 20002
