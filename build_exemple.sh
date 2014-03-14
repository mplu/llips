#! /bin/bash
rm llips_exemple
gcc llips_exemple.c lipps/llips_contrastdetect.c lipps/llips_functions.c lipps/llips_general.c lipps/llips_io.c lipps/llips_motiondetect.c -o llips_exemple