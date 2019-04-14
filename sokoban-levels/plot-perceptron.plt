set key autotitle columnhead
unset key

set title "Result for perceptron"
set ylabel "Time (ms)"
set xlabel "Path length"

set grid
set term png
set output "graph-perceptron.png"

plot "graph-perceptron.csv"