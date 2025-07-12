#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

extern gid_t getgid(void);
extern uid_t getuid(void);
extern int setegid(gid_t);
extern int seteuid(uid_t);

int main(int argc, char* argv[]) {
    if (getgid() != getegid()) {
        setegid(getgid());
        seteuid(getuid());
        /*
        const char** args = calloc(argc+1,sizeof(char*));
        for (int i = 0; i < argc; i++) {
            fprintf(stderr,"%s\n",argv[i]);
            args[i] = argv[i];
        }
        execv(args[0], (char* const*)args);*/
    }

    int fd = open("/dev/null", O_WRONLY);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    close(fd);
    fd = open("/dev/null", O_RDONLY);
    dup2(fd, STDIN_FILENO);
    close(fd);
    const char** args = calloc(argc,sizeof(char*));
    for (int i = 1; i < argc; i++) {
        args[i-1] = argv[i];
    }
    pid_t pid = fork();
    if (pid) return 0;
    execv(args[0], (char* const*)args);
    exit(127);
}
