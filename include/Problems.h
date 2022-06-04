#ifndef PROBLEMS_H
#define PROBLEMS_H

#include <inttypes.h>

void WritersStarvation(uint32_t rCount, uint32_t wCount);
void ReadersStarvation(uint32_t rCount, uint32_t wCount);
void NoStarvation(uint32_t rCount, uint32_t wCount);

#endif