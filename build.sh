#!/bin/bash
gcc -lws2811 -lm -lmosquitto -o pilite pilite.c mqtt.o touchscreen.o macaddr.o colours.o rpi_ws281x/build/CMakeFiles/ws2811.dir/lightgrid.c.o
