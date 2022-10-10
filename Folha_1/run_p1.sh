#!/bin/sh

OUTPUT_FILE="P1_OUTPUT.txt"

make
-rm $OUTPUT_FILE
clear
sudo ./problema_1 | tee $OUTPUT_FILE