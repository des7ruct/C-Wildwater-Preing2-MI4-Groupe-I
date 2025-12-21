set terminal pngcairo enhanced font "Arial,24" size 1728,972
set output 'retour.png'

set style fill solid 1.0 border lc rgb "black"

unset key
set title "10 plus grandes usines" font "Arial,32"
set xlabel "IDs des usines" font "Arial,22"
set ylabel "Volume(M.m3)" font "Arial,22" offset 0,-1
set xtics font "Arial,10" format "%s" rotate by -90
set grid xtics lt 0 lw 1 lc rgb "#DDDDDD"

set style line 1 lc rgb '#2E86C1' lw 2

set tmargin 3
set bmargin 5

set boxwidth 0.9 relative
set border 3
plot 'gtemp.dat' using 0:2:xtic(1) with boxes ls 1 notitle