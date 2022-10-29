#!/bin/sh

OUTPUT_FILE="P6_OUTPUT.txt"

make
-rm $OUTPUT_FILE
clear
sudo ./problema_6 | tee $OUTPUT_FILE