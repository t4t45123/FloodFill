#!/bin/ksh

rm -r image/

mkdir image


./a.out

ffmpeg -y -i image/output%d.ppm -r 40 output.gif

mpv --loop output.gif
