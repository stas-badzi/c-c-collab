#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/kd.h>

#include <getfd.h>

ssize_t readlink(const char *restrict path, char *restrict buf, size_t bufsize);
 
int main(int, char *argv[])
{
  int fd, mode, orig_mode, err;
  mode = argv[1][0] - '0';
  if (mode < 0 || mode > 4) {
    printf("wrong arguments %d is not in range 0..4\n",mode);
    exit(-1);
  }
  
  if ((fd = getfd(0)) < 0) {
    char buf[256];
    int buf_size = readlink( "/proc/self/exe" , buf, 256);
    buf[buf_size] = ' ';
    buf[buf_size + 1] = argv[1][0];
    buf[buf_size + 2] = '\0';
    char* sudo = "sudo ";
    char* file = &buf[0];
    char * command = (char *) malloc(1 + strlen(sudo)+ strlen(file) );
    strcpy(command, sudo);
    strcat(command, file);
    return system((const char *)(command));
  }
  

  if ((err = ioctl(fd, KDGKBMODE, &orig_mode))) {
    close(fd);
    printf("ioctl KDGKBMODE error %d\n", err);
    return -1;
  }

      
  if ((err = ioctl(fd, KDSKBMODE, mode))) {
    close(fd);
    printf("ioctl KDSKBMODE error %d\n", err);
    return -1;
  }

  close(fd);
  
  return orig_mode;
}
