unset key
set cbrange [0:0.5]
set view map
set size 1.0, 1.5
set origin 0.0, -.2
splot 'range_spedup_final.txt' matrix with image
pause -1
