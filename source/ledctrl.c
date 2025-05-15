#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>

static int getledpath(const char *find, int fsize, char* buffer, int size) {
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir("/sys/class/leds")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            int i;
            int x = 0;
            for (i = 0; i < size && ent->d_name[i]; i++) {
                if (x < size) {
                    if (find[x] != ent->d_name[i]) { x=0; }
                    else  { ++x; }
                }
                buffer[i] = ent->d_name[i];
            }
            buffer[i] = '\0';
            if (x == fsize) {
                closedir(dir);
                return i;
            }
        }
        closedir (dir);
        return 0;
    } else {
        /* could not open directory */
        perror ("");
        return -1;
    }
}

int setled(char* led, int brightness) {
    char buffer[284] = "/sys/class/leds/";
    if (getledpath(led, strlen(led), buffer+strlen("/sys/class/leds/"), 255) <= 0) 
        return -1;
    strcat(buffer, "/brightness");
    FILE *fp = fopen(buffer, "w");
    if (fp == NULL) {
        return -1;
    }
    fprintf(fp, "%d", brightness);
    fclose(fp);
    return 0;
}

int getled(const char* led) {
    char buffer[284] = "/sys/class/leds/";
    if (getledpath(led, strlen(led), buffer+strlen("/sys/class/leds/"), 255) <= 0) 
        return -1;
    strcat(buffer, "/brightness");
    FILE *fp = fopen(buffer, "r");
    if (fp == NULL) {
        return -1;
    }
    int brightness = 0;
    int len = fscanf(fp, "%d", &brightness);
    fclose(fp);
    return brightness;
}

int getledmax(const char* led) {
    char buffer[284] = "/sys/class/leds/";
    if (getledpath(led, strlen(led), buffer+strlen("/sys/class/leds/"), 255) <= 0) 
        return -1;
    strcat(buffer, "/max_brightness");
    FILE *fp = fopen(buffer, "r");
    if (fp == NULL) {
        return -1;
    }
    int max_brightness = 0;
    int len = fscanf(fp, "%d", &max_brightness);
    fclose(fp);
    return max_brightness;
}
