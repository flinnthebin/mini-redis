# container

sched_setaffinity()
    - sets CPU affinity mask of thread
    - pid/caller pid set to value of mask
    - writes to struct cpu_set_t
    - pcu_set_t points to mask
    - cpusetsize = size (bytes) of mask
    - return 0 or -1 & set errno (EFAULT | EINVAL | EPERM | ESRCH)

history
    - glibc interfaces include cpusetsize (uint)
    - 
