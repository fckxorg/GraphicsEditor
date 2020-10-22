#!/bin/bash
cd build && cmake $1 .. && make && cd .. && ./build/Main
