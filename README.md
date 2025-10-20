# ideal2resolved

## Instruction
- requires a recorded Paraver trace and its _ideal network_ simulated one
- Compile with `libmath`:
  ```bash
  gcc -O3 -o ideal2resolved.exe source/ideal2resolved.c -lm
  ```
- Use the following command:
  ```bash
  ./ideal2resolved.exe input.in
  ```
- An example for the contents of `input.in`
  ```bash
  $ cat input.in
  prv-file <path-to>/sor.gcc.ompi.exe.prv
  sim-file <path-to>/sor.gcc.ompi.exe_256P.sim.prv
  ```
- Results in a `resolved.dat` file at the execution directory.
- The following Gnuplot script prints the transfer-efficiency monitors:
  ```bash
  fname='resolved.dat'
  set title '{/:Bold Transfer efficiency monitor of rank-0}' font ',18'
  set xlabel '{/:Bold Elapsed time [s]}' font ',18'
  set ylabel '{/:Bold Transfer Efficiency}' font ',18' offset 0,0
  set grid
  set key default
  set key reverse bottom left Left width -7 font ',24'
  set xtics font ',18'
  set ytics font ',18'
  set lmargin 10
  set bmargin 4

  p 1/0 w p pt 2 lw 2 ps 4 lc rgb 'light-red' title '{/:Bold Tr.Eff._{local}}'\
  , fname u ($1*1.0e-9):($2/$3) w p pt 2 lw .5 ps 2 lc rgb 'light-red' notitle axes x1y2\
  , '' u ($1*1.0e-9):($4/$5) w l lw 4 lc rgb 'web-blue' title '{/:Bold Tr.Eff._{cumulative}}'\
  , 0.92355130 w l lw 2 dt 2 lc rgb 'spring-green' title '{/:Bold Tr.Eff.^@{Basic-analysis}_{final}}'
  ROUND_X=.03*(GPVAL_DATA_X_MAX- GPVAL_DATA_X_MIN)
  ROUND_Y2=.03*(GPVAL_DATA_Y2_MAX- GPVAL_DATA_Y2_MIN)
  set xrange[GPVAL_DATA_X_MIN-ROUND_X:GPVAL_DATA_X_MAX+ROUND_X]
  set yrange[GPVAL_DATA_Y2_MIN-ROUND_Y2:GPVAL_DATA_Y2_MAX+ROUND_Y2]

  #set term pngcairo enhanced dashed size 1600,400
  #set output fname[:strlen(fname)-4].'.png'
  ```
  Last `y-value` is of the whole execution.
- For demonstration, the recorded and simulated traces for the stencil code is here:
<https://drive.google.com/drive/folders/1Kj7QqYrAHnxR8EW84JRkAQ_uOrjxZgXc>
