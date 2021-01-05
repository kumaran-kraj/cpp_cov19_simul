# The graphic title
set title 'Sin(x)'
#plot the graphic
plot  "pts.dat" using 1:2 smooth cspline title 'fx', \
"pts.dat" using 1:3 smooth cspline title 'ts' , \
"pts.dat" using 1:4 smooth cspline title 'ts E' , \
"pts.dat" using 1:5 smooth cspline title 'MEM' , \
"pts.dat" using 1:6 smooth cspline title 'MEM E' , \
"pts.dat" using 1:7 smooth cspline title 'IEM' , \
"pts.dat" using 1:8 smooth cspline title 'IEM E' , \
"pts.dat" using 1:9 smooth cspline title 'RK2' , \
"pts.dat" using 1:10 smooth cspline title 'RK2 E' , \
"pts.dat" using 1:11 smooth cspline title 'RK4' , \
"pts.dat" using 1:12 smooth cspline title 'RK4 E' 
pause -1