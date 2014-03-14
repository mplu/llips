#! /bin/bash
rm llips_exemple
gcc llips_exemple.c llips/llips_contrastdetect.c llips/llips_functions.c llips/llips_general.c llips/llips_io.c llips/llips_motiondetect.c -o llips_exemple