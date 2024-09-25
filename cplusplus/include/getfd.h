#pragma once

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/kd.h>
#include <sys/ioctl.h>
/*
 * getfd.c
 *
 * Get an fd for use with kbd/console ioctls.
 * We try several things because opening /dev/console will fail
 * if someone else used X (which does a chown on /dev/console).
 */
inline int getfd() {
    int fd;
    char arg = 0;

    fd = open("/dev/tty", O_RDONLY);
    if (fd < 0 && errno == EACCES) fd = open("/dev/tty", O_WRONLY);  
    
    if (fd >= 0 && (ioctl(fd, KDGKBTYPE, &arg) == 0 && ((arg == KB_101) || (arg == KB_84)))) return fd;

    fd = open("/dev/console", O_RDONLY);
    if (fd < 0 && errno == EACCES) fd = open("/dev/console", O_WRONLY);
    
    if (fd >= 0 && (ioctl(fd, KDGKBTYPE, &arg) == 0 && ((arg == KB_101) || (arg == KB_84)))) return fd;

    for (fd = 0; fd < 3; fd++) if (ioctl(fd, KDGKBTYPE, &arg) == 0 && ((arg == KB_101) || (arg == KB_84))) return fd;
    return -1;
}