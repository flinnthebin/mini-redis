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


