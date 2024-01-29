
# Warmup

1. 3 load accesses, 1 store access, and 3 floating point operations. The arithmetic intensity is = 0.09375 FLOP/byte

1. 1 load access, 0 store access, and 2 floating point operations. The arithmetic intensity is = 0.25 FLOP/byte

1. 2 load accesses, 0 store access, and 2 floating point operations. The arithmetic intensity is = 0.125 FLOP/byte

1. 2 load accesses, 1 store access, and 2 floating point operations. The arithmetic intensity is = 0.0833 FLOP/byte

_________________________________________________

# Part 1

1. mat_mult.c

1. For an NxN matrix as in our experiments, the number of floating point operations is 2N<sup>3</sup>. In general, it’s O(N<sup>3</sup>) where N is the largest of (n,p,m) for matrices (np) and (pm).

1. We run our code on the `dev-intel16` MSU HPCC development node. For the NxN matrix, we generate random double precision values in the range [0,1]. We disable optimization using the`-O0` flag. For all experiments in Part 1, we bind our running process to a single core so that the operating system’s scheduler doesn’t bind it to a different core after time slice expiration or interrupts since every core has its own L1 cache and we want cache reuse. We use the `sched_setaffinity` wrapper function in the C standard library to implement the binding. By using the wall clock time measured in seconds using microsecond precision, we find that it takes on average 0.0047 seconds (10 runs ; standard deviation=0.0007) for the process to complete the matrix multiplication of a N by N matrix for N=100. The `get_walltime` function was called directly before and after the matrix multiplication operator. The result of 0.0047 seconds to complete the matrix multiplication of a 100 by 100 matrix makes sense since the number of floating point operations here is 2*(100)^3 = 2,000,000. It takes 0.0047 seconds to finish 2,000,000 operations, so it would take 1 second to finish 425,531,915 operations assuming everything else stays behaving the same (cache misses).The specifications of the core we are running on is 2.40 GHz which is 2,400,000,000 cycles per second. If we assume that each floating point operation takes 1 cycle then the core’s peak performance is 2,400,000,000 floating point operations per second. The core’s peak performance is 2,400,000,000 floating point operations per second (2.4 GFLOP/s) and our matrix operator on average does 425,531,915 operations per second (for 100 by 100 matrix multiplication) then our matrix operator only uses 425,531,915 / 2,400,000,000 = 0.177 of the core’s peak performance (17.7 %). 425,531,915 FLOP/s is 425.531915 MFLOP/s.
Extra Experiment: Using the `perf_event_open` Linux system call wrapper function, we measure the CPU cycles of the core with `PERF_COUNT_HW_REF_CPU_CYCLES` (not affected by CPU frequency scaling), and we excluded the kernel cpu cycles. For the 100 by 100 matrix multiplication, 11,958,984 cycles were observed (11,958,984 cycles in 0.0047 seconds). The core's peak performance is 2,400,000,000 cycles per second which is 11,280,000 cycles per 0.0047. So, our observed CPU cycles per second matches the core's peak performance in terms of CPU cycles per second. However, it's only computing 425,531,915 floating point operations per second so our naive matrix multiplication approach isn't even able to achieve 1 floating point operation in every cycle (idle cycles waiting for memory). The CPU is also capable of running more than 1 flop per clock cycle (due to pipelining) which can be observed in the roofline model's graph below where the cpu's peak performance is 30.7 GFLOP/s.  We also found that CPU utilization returned by the top command can be misleading probably because it accounts for the CPU being utilized when it’s waiting for memory transfer. 
1. Using the `lscpu` command, (Intel(R) Xeon(R) CPU E5-2680 v4 @ 2.40GHz). As discussed in question 3, 2.40 GHz is 2,400,000,000 cycles per second (the theoretical peak performance) which translates to 2,400,000,000 floating point operations per second under the assumption that the processor is capable of one flop per clock cycle. 2,400,000,000 FLOP/s is 2.4 GFLOP/s. In part 3, we mentioned that our matrix operator on average does 425,531,915 floating point operations per second (for 100 by 100 matrix multiplication) which is 0.425531915 GFLOP/s. As shown in question 3, the ratio is = 0.425531915 / 2.4 = 0.177 so only 17.7 % of the processor’s theoretical power is being utilized by our matrix operator. 
1. `part1_q4_1.png` shows the plot in linear scale. `part1_q4_2.png` shows the plot where the y-axis is in log scale. The peak performance y=2.4 GFLOP/s is also shown. We plot from N=1 to N=1412. We stopped plotting at N=1412 because we might have figured out the trend of the plot. Around N=1400s, it's taking 22-26 seconds to compute for each N where the time will keep on increasing. If we assumed for all the N>1400 that it will take 26 seconds each then it will take us 5 more hours to compute the matrix multiplication of 1000 more N values. 
1. `lscpu` command mentions that each core has its own L1 data cache of size (32 Kilobytes = 32,0000 bytes). The L2 cache is of size (256 Kilobytes = 256,000 bytes) and the L3 cache is of size (35840 Kilobytes = 35,840,000 bytes). The L1 cache can fit 32,000 / 8.0 = 4,000 double precision numbers so computing further away from N=$\sqrt{4000}$=63 dimensional matrices will rapidly decrease performance since the L1 cache would be filled (the 4,000 elements of the columns multiplied by a single row). This can be seen in the plot where there's increase in performance from small values N till around N=60 then a rapid decrease in performance. After this rapid decrease in performance, there's a stable decrease in performance up until N=160-170. This is because the L2 cache will be filling entirely around this N range since the L2 cache can hold 35,840,000 / 8.0 = 32,000 double precision numbers which means that it can hold up to a N by N matrix where N=$\sqrt{32000}$=178 before it fills up. After this rapid decrease in performance, the decrease of performance becomes stable and we can see that around late N=1300s the decrease is slowly becoming rapid again since we are getting near the N value which will fill up the L3 cache (N=$\sqrt{4480000}$=2116). 

# Part 2

1. Run on Dev-intel16 MSU HPCC development node.

1. Run on Dev-intel16 MSU HPCC development node.

1. Plot: `roofline_model.png`
    L1: bandwidth=104.1 GB/s ; Ridgepoint=0.3 FLOP/Byte
    L2: bandwidth=65.6 GB/s ; Ridgepoint=0.43 FLOP/Byte
    L3: bandwidth=41.9 GB/s ; Ridgepoint=0.7 FLOP/Byte
    DRAM: bandwidth=17.8 GB/s ; Ridgepoint=1.68 FLOP/Byte
    
1. [For the memory-bound kernels where DRAM or a certain cache level is a bottleneck, optimizing loops by unrolling them and applying blocking techniques will help with minimizing cache misses]
SpMV: 0.25 Flops/Byte [Falls to the left of all the diagonals. Can't reach peak performance even if L1 cache is used only. 
To improve performance, we need a higher bandwidth L1 cache or distribute the computation/memory work on different cores (each has own L1 cache).
LBMHD: 1.07 Flops/Byte [Falls to the right of the L1,L2, and L3 diagonals but to the left of the DRAM diagonal. Peak performance is achieved if it doesn't make use of the DRAM by making more use of the L3 cache by distributing computation on different nodes where each node has a different L3 cache. So reducing memory transfers would improve performance here since the performance is limited by theavailable DRAM memory bandwidth].
Stencil: 0.5 Flops/Byte [its column falls to the right of the L1 and L2 diagonals but to the left of the L3 and DRAM diagonals. 
Peak performance is achieved if it doesn't make use of the DRAM and L3 by making more use of the L2 cache by distributing computation on different nodes where each node has a different L2 cache and maximizing cache hits for L2 cache]. By not making use of L3 and DRAM, it becomes compute-bound and is able to achieve maximum performance.
3-D FFT: 1.64 Flops/Byte  [Falls to the right of the L1,L2, and L3 diagonals but to the left of the DRAM diagonal. Peak performance is achieved if it doesn't make use of the DRAM by making more use of the L3 cache by distributing computation on different nodes where each node has a different L3 cache]. By not making use of DRAM, it becomes compute-bound and is able to achieve maximum performance.

1. Question 5 Remaining 
1. Question 6 Remaining
