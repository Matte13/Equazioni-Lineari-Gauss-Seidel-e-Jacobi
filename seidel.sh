# bin/sh

# File complilation
gcc seidel.cpp -o exec/seidel

# File execution
./exec/seidel data/$1

# Creating plot from file results
gnuplot -e "error_per_iteration='gnuplot/data/$1'; set term png; set output 'gnuplot/seidel_$1.png'; plot error_per_iteration with lines"
