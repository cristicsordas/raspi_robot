#ifndef PCF_IOCTL_H
#define PCF_IOCTL_H

#ifdef __KERNEL__
#include <asm-generic/ioctl.h>
#include <linux/types.h>
#else
#include <sys/ioctl.h>
#include <stdint.h>
#endif

#define PCF_IOC_MAGIC 0x17

#define PCF_IOC_SET_CHANNEL _IOWR(PCF_IOC_MAGIC, 1, uint8_t)

/**
 * The maximum number of commands supported, used for bounds checking
 */
#define PCF_IOC_MAXNR 1


#endif //PCF_IOCTL_H