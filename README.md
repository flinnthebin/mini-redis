# container

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

The system may further restrict the set of CPUs on which the thread runs if the cpuset (read man) is being used.

Number of available CPUs can be determined by inspecting the contents of /proc/cpuinfo;
using sysconf to obtain the values of the _SC_NPROCESSORS_CONF and _SC_NPROCESSORS_ONLN parameters;
and inspecting the list of CPU directories under /sys/devices/system/cpu/

sched - linux scheduling scheme

CPU_ALLOC the mask argument dynamically. Achieve by probing for the size of the required mask using sched_getaffinity()
with increasing mask sizes (until the call does not fail with the error EINVAL).

CPU_ALLOC may allocate a slightly larger CPU set than requested, because CPU sets are implemented as bit masks allocated
in units of sizeof(long).

The caller should iterate over the bits in the returned set, counting those which are set and stop upon reaching the
value returned by CPU_COUNT, rather than iterating over the number of bits requested to be allocated.

Example Code


