
# Bret Abel
# 3/30/2020

This is a homework assignment from an Operating Systems course I took in undergrad.
The write-up I did about the program for my professor is below:

This program performs sequential matrix multiplication if given 2 command line arguments,
or if given 3 arguments it will do the same thing but in a multi-threaded fashion.
The number of threads and size of the matrices are inputted by the user.
It throws a warning when compiled because the multiplication function is defined
in a separate file, but it works exactly as intended.

For my parallel strategy I set it up such that each thread is alotted a number of 
rows in the multiplicand to work on that is proportional to the size of the matrix.

My virtual machine only has 1 CPU core compared to my laptop's quadcore hyperthreading CPU,
so I think that may have affected the amount of speedup. The speedup is negative at smaller
matrices (which makes sense given thread overhead), but is visible with sufficiently large matrices. 

For my speed testing I used this command: "./mmm P 4 500"

The average speed on my virtual machine was about 0.36 seconds for parallel and 0.60 seconds for
sequential. The average speedup then was a factor of about 1.7. I got the average from 3 runs.
