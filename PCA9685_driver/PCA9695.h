#ifndef PCA9685_DRIVER_AESDCHAR_H_
#define PCA9685_DRIVER_AESDCHAR_H_

//#define AESD_DEBUG

#undef PDEBUG             /* undef it, just in case */
#ifdef AESD_DEBUG
#  ifdef __KERNEL__
     /* This one if debugging is on, and kernel space */
#    define PDEBUG(fmt, args...) printk( KERN_DEBUG "aesdchar: " fmt, ## args)
#  else
     /* This one for user space */
#    define PDEBUG(fmt, args...) fprintf(stderr, fmt, ## args)
#  endif
#else
#  define PDEBUG(fmt, args...) /* not debugging: nothing */
#endif

#endif /* PCA9685_DRIVER_AESDCHAR_H_ */
