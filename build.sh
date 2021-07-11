#!/bin/bash

gcc -o mqtt.o -lbsd -c mqtt.c
gcc -o macaddr.o -c macaddr.c
gcc -o touchscreen.o -c touchscreen.c
gcc -o colours.o -c colours.c

gcc -lbsd -lws2811 -lm -lmosquitto -o pilite pilite.c mqtt.o touchscreen.o macaddr.o colours.o rpi_ws281x/build/CMakeFiles/ws2811.dir/lightgrid.c.o
