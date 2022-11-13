
#ifndef _IOCTL_TEST_H
#define _IOCTL_TEST_H

#include <linux/ioctl.h>

struct ioctl_arg {
        unsigned int val;
};

#define IOC_MAGIC 'k'

#define IOCTL_VALRESET    _IO(IOC_MAGIC,0)
#define IOCTL_VALSET      _IOW(IOC_MAGIC, 1, struct ioctl_arg)
#define IOCTL_VALGET      _IOR(IOC_MAGIC, 2, struct ioctl_arg)
#define IOCTL_VALGET_INT  _IOR(IOC_MAGIC, 3, int)
#define IOCTL_VALSET_INT  _IOW(IOC_MAGIC, 4, int)

#define IOCTL_VAL_MAXNR 4

#endif
