# bin/sh

# File complilation
gcc jacobi.cpp -o exec/jacobi

# File execution
./exec/jacobi data/$1

# Creating plot from file results
gnuplot -e "error_per_iteration='gnuplot/data/$1'; set term png; set output 'gnuplot/jacobi_$1.png'; plot error_per_iteration with lines"
