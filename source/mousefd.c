#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

static int getmouseeventpath(char *buffer, size_t size) {
    DIR *dir;
    struct dirent *ent;
    const char *find1 = "event-mouse";
    size_t fsize1 = strlen(find1);
    const char *find2 = "mouse";
    size_t fsize2 = strlen(find2);
    if ((dir = opendir("/dev/input/by-path")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            size_t i;
            size_t x = 0;
            for (i = 0; i < size && ent->d_name[i]; i++) {
                if (x < size) {
                    if (find1[x] != ent->d_name[i]) { x=0; }
                    else  { ++x; }
                }
                buffer[i] = ent->d_name[i];
            }
            buffer[i] = '\0';
            if (x == fsize1) {
                closedir(dir);
                return i;
            }
        }
        closedir (dir);
        dir = opendir("/dev/input/by-id");
        while ((ent = readdir(dir)) != NULL) {
            size_t i;
            size_t x = 0;
            for (i = 0; i < size && ent->d_name[i]; i++) {
                if (x < size) {
                    if (find2[x] != ent->d_name[i]) { x=0; }
                    else  { ++x; }
                }
                buffer[i] = ent->d_name[i];
            }
            buffer[i] = '\0';
            if (x == fsize2) {
                closedir(dir);
                return i;
            }
        }
        closedir (dir);
        return -1;
    }
    return -2;
}

int
getmousefd(const char *fnam){
	int fd, i;

	if (fnam) {
		if ((fd = open(fnam, O_RDONLY)) >= 0)
            return fd;
		return -1;
	}

    char buffer[256] = "/dev/input/by-path/";
    int size = getmouseeventpath(buffer+strlen(buffer), sizeof(buffer)-strlen(buffer));
    if (size <= 0) return size-2;
    if ((fd = open(buffer, O_RDONLY)) < 0)
        return -1;
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    return fd;
}
