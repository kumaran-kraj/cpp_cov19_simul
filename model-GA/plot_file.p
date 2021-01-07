#
set datafile separator ","
plot  "dat0.dat" using 1:2 smooth cspline title 'S', \
"dat0.dat" using 1:3 smooth cspline title 'I', \
"dat0.dat" using 1:4 smooth cspline title 'R', \
"dat0.dat" using 1:5 smooth cspline title 'D', \
