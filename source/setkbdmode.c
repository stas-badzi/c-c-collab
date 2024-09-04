#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/kd.h>
 
// getfd()
#include <getfd.h>
 
int main(int argc, char *argv[])
{
  int fd, mode, orig_mode, err;
  
  if ((fd = getfd()) < 0) {
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
  mode = argv[1][0] - '0';
  if (mode < 0 || mode > 4) {
     printf("wrong arguments %d is not in range 0..4\n",mode);
     exit(-1);
  }
  

      if (err = ioctl(fd, KDGKBMODE, &orig_mode)) {
        printf("ioctl KDGKBMODE error %d\n", err);
        exit(1);
      }

      
      if (err, ioctl(fd, KDSKBMODE, mode)) {
        printf("ioctl KDSKBMODE error %d\n", err);
        exit(-1);
      }
 
     return orig_mode;
 }
