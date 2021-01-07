set terminal svg dashlength 0.2
set output "g7r1.svg"
plot "G7R1.dat" using 1:2 smooth cspline title 'S', \
"G7R1.dat" using 1:3 smooth cspline title 'I', \
"G7R1.dat" using 1:4 smooth cspline title 'R', \
"G7R1.dat" using 1:5 smooth cspline title 'D', \
