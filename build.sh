#!/bin/bash

gcc -ggdb -o mqtt.o -lbsd -c mqtt.c
gcc -ggdb -o macaddr.o -c macaddr.c
gcc -ggdb -o touchscreen.o -c touchscreen.c
gcc -ggdb -o colours.o -c colours.c

gcc -ggdb -lbsd -lws2811 -lm -lmosquitto -o pilite pilite.c mqtt.o touchscreen.o macaddr.o colours.o rpi_ws281x/build/CMakeFiles/ws2811.dir/lightgrid.c.o
