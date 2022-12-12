# Matrix-Multiplier-with-IPC-Multithreading-Round-Robin-Scheduler


Matrix Multiplication program in C performing calculations on large matrices using parallelism provided by the Linux process and threads libraries. IPC mechanisms used to create shared memory segment for communication between processes. Round Robin Algorithm Implemented to simulate a uniprocessor scheduler.


Contents :

C program (P1) which takes three filenames (in1.txt, in2.txt and out.txt) as command line arguments. 
The first two files (in1.txt and in2.txt) contain two matrices of arbitrary size but satisfying the criteria for matrix multiplication. 
The sizes will be passed in the command line. P1 spawns n threads which each read row(s) and column(s) each from in1.txt and in2.txt.
Different threads read different parts of the file. We vary the number of threads from 1… to arbitrarily large.

We record the time that it takes to read the entire file into memory with different number of threads (1, 2, … n). The timing is at the granularity of nanoseconds.
We plot time against the number of threads for different input sizes.




C program (P2) which uses IPC mechanisms to receive the rows and columns read by P1. P2 spawns multiple threads to compute the cells in the product matrix.
The program stores the product matrix in the file out.txt
We vary the number of threads from 1… to arbitrarily large.
We record the time it takes to compute the product with different number of threads. The timing is at the granularity of nanoseconds.
We plot the time against the number of threads for different input sizes.




We write a scheduler program S. S spawns 2 children processes which exec to become the processes P1 and P2 above. S uses mechanisms to simulate a uniprocessor scheduler. That is, it suspends P1 and lets P2 execute, and vice versa.We simulate Round Robin Scheduling algorithms in S with time quantums of 1s and 2s.



Used Python libraries to plot charts to display Runtime vs Number of threads of each processes on varying workload sizes in the granularity of nanoseconds.



Process 1 :


![image](https://user-images.githubusercontent.com/29221347/207156187-7e8c4525-e725-42ba-828c-afb9205dcdc5.png)




Process 2 :

![image](https://user-images.githubusercontent.com/29221347/207156381-ceffa133-aac0-4afb-9616-352c9201c99f.png)




Workflow :

![image](https://user-images.githubusercontent.com/29221347/207155518-194f47d2-38da-43e2-bee7-e640263bdd84.png)





This project was done as an Assignment in the coursework of CS F372 Operating Systems.

Team Members :

Shreyash Bhardwaj 
Animish Prateek 
Andrew Alex Devasia
Hirakjyoti Nath 
Krishn Parasar 
Nikhil Raj
