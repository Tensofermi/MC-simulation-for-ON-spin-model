set terminal pdfcairo lw 1.5 font "Times New Roman,15" enhanced
set termoption enhanced 
set terminal pdf size 5in, 3.5in  # 纸张的大小
set size ratio 3.0/4.0

col = 44 # Qm
col = 48 # C12
col = 50 # corr
set size ratio 3.0/4.0

#######################################################
#######################################################
set output "test.pdf"
set pointsize 1
set xlabel "{/Times-Italic β}"  font "Times New Roman,20"
set ylabel "{/Times-Italic Q}"  font "Times New Roman,20"

set key left

# set xrange [0.21 : 0.24]

plot  '../data/cmp/cmp_L_8.dat' using 3:(column(col)):(column(col + 1))  with errorline pt 6  lc 'purple'  t '8',\
'../data/cmp/cmp_L_16.dat' using 3:(column(col)):(column(col + 1))  with errorline pt 6  lc 'green'  t '16',\
'../data/cmp/cmp_L_20.dat' using 3:(column(col)):(column(col + 1))  with errorline pt 6  lc 'blue'  t '20',\
'../data/cmp/cmp_L_32.dat' using 3:(column(col)):(column(col + 1))  with errorline pt 6  lc 'red'  t '32'

# plot  '../data/cmp/cmp_L_8.dat' using 3:(column(col))  with errorline pt 6  lc 'purple'  t '8',\
# '../data/cmp/cmp_L_16.dat' using 3:(column(col))  with errorline pt 6  lc 'green'  t '16',\
# '../data/cmp/cmp_L_32.dat' using 3:(column(col))  with errorline pt 6  lc 'blue'  t '32',\
# '../data/cmp/cmp_L_64.dat' using 3:(column(col))  with errorline pt 6  lc 'red'  t '64'

set output