#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <libproc.h>
#include <stdbool.h>
#include <string.h>

static const char* getProcessName(pid_t pid) {
    static char pathbuf[PROC_PIDPATHINFO_MAXSIZE];
    int ret = proc_pidpath(pid, pathbuf, sizeof(pathbuf));
    if (ret <= 0) {
        fprintf(stderr, "proc_pidpath() failed: %s, pid: %d\n", strerror(errno), pid);
        exit(0x90);
    }
    return pathbuf;
}

static pid_t getParentPid(pid_t pid) {
    struct proc_bsdshortinfo pinfo;
    int ret = proc_pidinfo(pid, PROC_PIDT_SHORTBSDINFO, 0, &pinfo, sizeof(pinfo));
    if (ret <= 0) {
        fprintf(stderr, "proc_pidpath() failed: %s, pid: %d\n", strerror(errno), pid);
        exit(0x90);
    }
    return pinfo.pbsi_ppid;
}

static bool isApp(pid_t pid) {
    if (pid == 0) return true;
    const char* name = getProcessName(pid);
    return strstr(name, ".app") != NULL;
}

static pid_t* getTerminalPid(pid_t pid) {
    pid_t oldpid = 0;
    while (!isApp(pid))
        pid = getParentPid(oldpid = pid);
    static pid_t pids[2];
    pids[0] = pid;
    pids[1] = oldpid;
    return pids;
}

int main() {
    pid_t pid = getppid();
    pid_t* pids = getTerminalPid(pid);
    kill(pids[0], SIGKILL);
    if (pids[1]) kill(pids[1], SIGKILL);
    return 0;
}
