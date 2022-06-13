#include "Problems.h"

#include "Global.h"

#include <pthread.h>
#include <stdlib.h>

static pthread_mutex_t libraryMutex          = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t readerCountCheckMutex = PTHREAD_MUTEX_INITIALIZER;

static volatile uint32_t readersInLibCount = 0;

static void* Writer(void* iblob)
{
  int i = (int)iblob;
  while(!quit)
  {
    pthread_mutex_lock(&libraryMutex);
    Log(i, r, w - 1, 0, 1);
      // write
    pthread_mutex_unlock(&libraryMutex);
  }

  return NULL;
}

static void* Reader(void* iblob)
{
  int i = (int)iblob;
  while(!quit)
  {
    pthread_mutex_lock(&readerCountCheckMutex);
    if(++readersInLibCount == 1) 
    {
      pthread_mutex_lock(&libraryMutex);
    }
    pthread_mutex_unlock(&readerCountCheckMutex);

    Log(i, r - readersInLibCount, w, readersInLibCount, 0);
    // read

    pthread_mutex_lock(&readerCountCheckMutex);
    if(--readersInLibCount == 0) 
    {
      pthread_mutex_unlock(&libraryMutex);
    }
    pthread_mutex_unlock(&readerCountCheckMutex);
  }

  return NULL;
}

void WritersStarvation(uint32_t r, uint32_t w)
{
  Run(r, Reader, w, Writer);

  pthread_mutex_destroy(&libraryMutex);
  pthread_mutex_destroy(&readerCountCheckMutex);
}