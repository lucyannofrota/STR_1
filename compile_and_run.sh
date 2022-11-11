#!/bin/sh

make
rm "P1_OUTPUT.txt" "P3_OUTPUT.txt" "P4A_OUTPUT.txt" "P4B1_OUTPUT.txt" "P4B2_OUTPUT.txt" "P5A_OUTPUT.txt" "P5B_OUTPUT.txt" "P6_OUTPUT.txt"
# sudo ./problema_1 | tee "P1_OUTPUT.txt"
echo "Runing problema_1..."
sudo ./problema_1 >> "P1_OUTPUT.txt" 2>&1
echo "Runing problema_3..."
sudo ./problema_3 >> "P3_OUTPUT.txt" 2>&1
echo "Runing problema_4A..."
sudo ./problema_4A >> "P4A_OUTPUT.txt" 2>&1
echo "Runing problema_4B..."
sudo ./problema_4B >> "P4B_OUTPUT.txt" 2>&1
echo "Runing problema_5A..."
sudo ./problema_5A >> "P5A_OUTPUT.txt" 2>&1
echo "Runing problema_5B..."
sudo ./problema_5B >> "P5B_OUTPUT.txt" 2>&1
echo "Runing problema_6..."
sudo ./problema_6 >> "P6_OUTPUT.txt" 2>&1