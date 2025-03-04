#pragma once

extern
#ifdef __cplusplus
"C"
#endif
int setled(const char *led, int brightness);

extern
#ifdef __cplusplus
"C"
#endif
int getled(const char *led);

extern
#ifdef __cplusplus
"C"
#endif
int getledmax(const char *led);
