#!/bin/ksh

ffmpeg -y -i image/output%d.ppm -r 20 output.gif

mpv --loop output.gif
