# Homework1
This is a introduction to compile HW1 files. <br>

## Categories
There are two directories in HW1 file. Part1 and Part2. <br>
Part1 is implementation of a Gaussian blur filter with SystemC modules connected with SystemC FIFO channels. <br>
Part2 is rewrite of the above Gaussian Blur thread, that transfer pixel data based on rows of a image. <br>

## Compilation
For example, if we want to compile part1.
<br>

    cd part1/build 
<br>

    cmake ..
<br>

    make
<br>

    make run
<br>

The result image "out.bmp" will be saved at part1/build, and the elapsed time will be shown on the terminal. <br>

Same process applies to compiling part2.
