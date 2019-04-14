set key autotitle columnhead
unset key

set title "Result for random shuffle"
set ylabel "Time (ms)"
set xlabel "Path length"

set grid
set term png
set output "graph-random-shuffle.png"

plot "graph-random-shuffle.csv"