# Simulated-annealing
This is a simulated annealing(SA) algorithm for the EDA floorplan issue

For usage
You can use make to re-compile to obtain the executable file SA_N26124963

For execute:
The execute format is in :
 ./SA_N26124963 {.yal}   A   W  {.txt}   {.out}   {.m} 
 where .yal is the input while A is the aspect ratio W is the white space ratio (ex. 15 mean there is 15% of whitespace)
 output files are .txt .out .m
For example:
 ./SA_N26124963 ami33.yal 3 15 ami33_3_15.txt ami33_3_15.out ami33_3_15.m
