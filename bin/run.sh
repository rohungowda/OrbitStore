#!/bin/sh

 g++  tests/Memtable_tests.cpp src/memtable.cpp src/arena.cpp src/linkedList.cpp src/keys.cpp  -o test