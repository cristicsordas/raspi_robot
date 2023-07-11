#ifndef PCF8591_DRIVER_H_
#define PCF8591_DRIVER_H_

//#define AESD_DEBUG

#define COMMAND 0x40

#undef PDEBUG             /* undef it, just in case */
#ifdef AESD_DEBUG
#  ifdef __KERNEL__
     /* This one if debugging is on, and kernel space */
#    define PDEBUG(fmt, args...) printk( KERN_DEBUG "pcf8591: " fmt, ## args)
#  else
     /* This one for user space */
#    define PDEBUG(fmt, args...) fprintf(stderr, fmt, ## args)
#  endif
#else
#  define PDEBUG(fmt, args...) /* not debugging: nothing */
#endif


#endif /* PCF8591_DRIVER_H_ */
