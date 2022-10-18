#!/bin/sh

OUTPUT_FILE="P3_OUTPUT.txt"

make
-rm $OUTPUT_FILE
clear
sudo ./problema_3 | tee $OUTPUT_FILE