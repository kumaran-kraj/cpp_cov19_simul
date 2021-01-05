#
set datafile separator ","
plot "dat.dat" using 1:2 smooth cspline
