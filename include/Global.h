#ifndef VARS_H
#define VARS_H

#include <pthread.h>
#include <inttypes.h>
#include <signal.h>

extern pthread_t* readers;
extern pthread_t* writers;

extern uint32_t r; // const
extern uint32_t w; // const

extern volatile sig_atomic_t quit;

void Log(int i, uint32_t rQ, uint32_t wQ, uint32_t rIn, uint32_t wIn);
void Run(uint32_t rCount, void *(*readersFn)(void *), uint32_t wCount, void *(*writersFn)(void *), void*(*beforeJoinAction)());

void Quit();


#endif