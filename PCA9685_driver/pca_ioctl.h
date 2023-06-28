#ifndef PCA_IOCTL_H
#define PCA_IOCTL_H

#ifdef __KERNEL__
#include <asm-generic/ioctl.h>
#include <linux/types.h>
#else
#include <sys/ioctl.h>
#include <stdint.h>
#endif

struct pca_pwm
{
    uint16_t on_value;
    uint16_t off_value;
};

struct pca_channel_pwm
{
    uint8_t channel;
    uint16_t on_value;
    uint16_t off_value;
};

// Pick an arbitrary unused value from https://github.com/torvalds/linux/blob/master/Documentation/userspace-api/ioctl/ioctl-number.rst
#define PCA_IOC_MAGIC 0x16

#define PCA_IOC_SET_ALL_PWM _IOWR(PCA_IOC_MAGIC, 1, struct pca_pwm)
#define PCA_IOC_SET_PWM_FREQV _IOWR(PCA_IOC_MAGIC, 2, uint32_t)
#define PCA_IOC_SET_PWM_ON_CHANNEL _IOWR(PCA_IOC_MAGIC, 3, struct pca_channel_pwm)

/**
 * The maximum number of commands supported, used for bounds checking
 */
#define PCA_IOC_MAXNR 3


#endif //PCA_IOCTL_H