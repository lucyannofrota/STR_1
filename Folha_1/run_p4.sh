#!/bin/sh

OUTPUT_FILE_1="P4A_OUTPUT.txt"
OUTPUT_FILE_2="P4B1_OUTPUT.txt"
OUTPUT_FILE_3="P4B2_OUTPUT.txt"

make
-rm $OUTPUT_FILE
clear
sudo ./problema_4A | tee $OUTPUT_FILE_1
sudo ./problema_4B1 | tee $OUTPUT_FILE_2
sudo ./problema_4B2 | tee $OUTPUT_FILE_3