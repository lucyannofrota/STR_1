#!/bin/sh

OUTPUT_FILE="P5_OUTPUT.txt"

make
-rm $OUTPUT_FILE
clear
sudo ./problema_5 | tee $OUTPUT_FILE