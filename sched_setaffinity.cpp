/* sched_setaffinity.cpp:

sched_setaffinity()
    - sets CPU affinity mask of thread
    - pid/caller pid set to value of mask
    - writes to struct cpu_set_t
    - pcu_set_t points to mask
    - cpusetsize = size_t (bytes) of mask
    - return 0 or -1 & set errno (EFAULT | EINVAL | EPERM | ESRCH)

The set of CPUs on which the thread will actually run is the intersection of:
    - the set specified in mask argument
    - the set of CPUs present on the system

The system may further restrict the set of CPUs on which the thread
runs if the cpuset (read man) is being used.

Number of available CPUs can be determined by inspecting the contents
of /proc/cpuinfo;using sysconf to obtain the values of the
_SC_NPROCESSORS_CONF and _SC_NPROCESSORS_ONLN parameters;
and inspecting the list of CPU directories under /sys/devices/system/cpu/

sched - linux scheduling scheme

CPU_ALLOC the mask argument dynamically. Achieve by probing for the size
of the required mask using sched_getaffinity() with increasing mask sizes
(until the call does not fail with the error EINVAL).

CPU_ALLOC may allocate a slightly larger CPU set than requested, because
CPU sets are implemented as bit masks allocated in units of sizeof(long).

The caller should iterate over the bits in the returned set, counting
those which are set and stop upon reaching the value returned by
CPU_COUNT, rather than iterating over the number of bits requested
to be allocated.

Example Code

The program below creates a child process.  The parent and child
then each assign themselves to a specified CPU and execute iden‐
tical loops that consume some CPU time.  Before terminating, the
parent waits for the child to complete.  The program takes three
command-line arguments: the CPU number for the parent,  the  CPU
number  for  the  child,  and the number of loop iterations that
both processes should perform.

As the sample runs below demonstrate, the amount of real and CPU
time consumed when running the program will depend on intra-core
caching effects and whether the processes  are  using  the  same
CPU.

We first employ lscpu(1) to determine that this (x86) system has
two cores, each with two CPUs:

$ lscpu | egrep -i 'core.*:|socket'
Thread(s) per core:    2
Core(s) per socket:    2
Socket(s):             1

We  then  time  the  operation  of the example program for three
cases: both processes running on the same  CPU;  both  processes
running  on  different CPUs on the same core; and both processes
running on different CPUs on different cores.

$ time -p ./a.out 0 0 100000000
real 14.75
user 3.02
sys 11.73
$ time -p ./a.out 0 1 100000000
real 11.52
user 3.98
sys 19.06
$ time -p ./a.out 0 3 100000000
real 7.89
user 3.29
sys 12.07

Program source


*/

// #define _GNU_SOURCE
#include <err.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int parentCPU, childCPU;
  cpu_set_t set;
  unsigned int nloops;

  if (argc != 4) {
    fprintf(stderr, "Usage: %s parent-cpu child-cpu num-loops\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  parentCPU = atoi(argv[1]);
  childCPU = atoi(argv[2]);
  nloops = atoi(argv[3]);

  CPU_ZERO(&set);

  switch (fork()) {
  case -1: /* Error */
    err(EXIT_FAILURE, "fork");

  case 0: /* Child */
    CPU_SET(childCPU, &set);

    if (sched_setaffinity(getpid(), sizeof(set), &set) == -1)
      err(EXIT_FAILURE, "sched_setaffinity");

    for (unsigned int j = 0; j < nloops; j++)
      getppid();

    exit(EXIT_SUCCESS);

  default: /* Parent */
    CPU_SET(parentCPU, &set);

    if (sched_setaffinity(getpid(), sizeof(set), &set) == -1)
      err(EXIT_FAILURE, "sched_setaffinity");

    for (unsigned int j = 0; j < nloops; j++)
      getppid();

    wait(NULL); /* Wait for child to terminate */
    exit(EXIT_SUCCESS);
  }
}
