#! /bin/sh
IP=10.0.0.151
build/stream_tst $IP 20000 20000
build/stream_tst $IP 20001 20001
build/stream_tst $IP 20002 20002
build/stream_tst $IP 20003 20003

