#pragma once

#ifndef __cplusplus
#include <stdbool.h>
#endif

extern
#ifdef __cplusplus
"C"
#endif
bool getkeystate(unsigned short key);

extern
#ifdef __cplusplus
"C"
#endif
bool capslocktoggled(void);
