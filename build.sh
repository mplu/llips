#! /bin/bash
rm BMP_Compare
gcc BMP_Compare.c functions.c lib_img.c motiondetect.c motorcontrol.c -o BMP_Compare