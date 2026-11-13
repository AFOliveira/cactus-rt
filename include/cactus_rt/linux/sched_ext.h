#ifndef CACTUS_RT_LINUX_SCHED_EXT_H_
#define CACTUS_RT_LINUX_SCHED_EXT_H_

#include <sched.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <cstdint>

// glibc 2.41 and later declare struct sched_attr (from <linux/sched/types.h>) and the
// sched_setattr / sched_getattr wrappers in <sched.h> when _GNU_SOURCE is defined (g++ always
// defines it): defining them again here is a redefinition error, so glibc's are used then.
#if defined(__GLIBC__) && defined(__GLIBC_PREREQ) && defined(__USE_GNU)
#if __GLIBC_PREREQ(2, 41)
#define CACTUS_RT_LIBC_HAS_SCHED_ATTR 1
#endif
#endif

#ifndef CACTUS_RT_LIBC_HAS_SCHED_ATTR
struct sched_attr {
  uint32_t size;

  uint32_t sched_policy;
  uint64_t sched_flags;

  /* SCHED_NORMAL, SCHED_BATCH */
  int32_t sched_nice;

  /* SCHED_FIFO, SCHED_RR */
  uint32_t sched_priority;

  /* SCHED_DEADLINE (nsec) */
  uint64_t sched_runtime;
  uint64_t sched_deadline;
  uint64_t sched_period;
};

inline long sched_setattr(pid_t pid, const struct sched_attr *attr, unsigned int flags) {
  return syscall(SYS_sched_setattr, pid, attr, flags);
}

inline long sched_getattr(pid_t pid, struct sched_attr *attr, unsigned int size, unsigned int flags) {
  return syscall(SYS_sched_getattr, pid, attr, size, flags);
}
#endif

#endif
