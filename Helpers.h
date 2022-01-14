#ifndef _HELPERS_H_
#define _HELPERS_H_


#include <stdint.h>
#include <stdbool.h>

// #define FTOA_MAX_AFTERPOINT 5

uint8_t ftoa (uint8_t *pui8_resBuf, float val, uint8_t maxAfterPoint = 5, bool b_round = true);



#endif // _HELPERS_H_