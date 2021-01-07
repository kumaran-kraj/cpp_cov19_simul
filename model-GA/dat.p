#
set datafile separator ","
plot  "dat.dat" using 1:2 smooth cspline title 'S', \
"dat.dat" using 1:3 smooth cspline title 'I', \
"dat.dat" using 1:4 smooth cspline title 'R', \
"dat.dat" using 1:5 smooth cspline title 'D', \
