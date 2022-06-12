#include "Global.h"
#include <stdlib.h>
#include <stdio.h>

pthread_t* readers = NULL;
pthread_t* writers = NULL;

uint32_t r = 0;
uint32_t w = 0;

volatile sig_atomic_t quit = 0;

void Log(int i,uint32_t rQ, uint32_t wQ, uint32_t rIn, uint32_t wIn)
{
  printf("[%i] | ReaderQ: %u WriterQ: %u [in: R:%u W:%u]\n", i, rQ, wQ, rIn, wIn);
}

void Run(uint32_t rCount, void *(*readersFn)(void *), uint32_t wCount, void *(*writersFn)(void *))
{
  w = wCount;
  r = rCount;

  writers = malloc(w * sizeof(pthread_t));
  readers = malloc(r * sizeof(pthread_t));

  for(int i=0; i<w; i++) { pthread_create(&writers[i], NULL, writersFn, (void*)i); }
  for(int i=0; i<r; i++) { pthread_create(&readers[i], NULL, readersFn, (void*)i); }

  for(int i=0; i<w; i++) { pthread_join(writers[i], NULL); }
  for(int i=0; i<r; i++) { pthread_join(readers[i], NULL); }

  free(writers);
  free(readers);
}

void Quit()
{
  quit = 1;
}