# The graphic title
#plot the graphic
set datafile separator ","
plot  "tst.csv" using 1:2 smooth cspline title 'fx', "tst.csv" using 3:4 smooth cspline title 'ts' , "tst.csv" using 5:6 smooth cspline title 'ts2'
