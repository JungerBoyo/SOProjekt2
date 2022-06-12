#include "Problems.h"

#include "Global.h"

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

static volatile uint32_t readersInLibCount = 0;

static pthread_mutex_t libMutex               = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t readersInLibCountMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t queueMutex             = PTHREAD_MUTEX_INITIALIZER;

static void* Writer(void* iblob)
{
  int i = (int)iblob;
  while(!quit)
  {
    pthread_mutex_lock(&queueMutex); 
    pthread_mutex_lock(&libMutex);
    pthread_mutex_unlock(&queueMutex); 

    Log(i, r, w - 1, 0, 1);

    pthread_mutex_unlock(&libMutex);
  }

  return NULL;
}

static void* Reader(void* iblob)
{
  int i = (int)iblob;
  while(!quit)
  {
    pthread_mutex_lock(&queueMutex);
    pthread_mutex_lock(&readersInLibCountMutex);
    if(++readersInLibCount == 1)
    {
      pthread_mutex_lock(&libMutex);
    }

    pthread_mutex_unlock(&queueMutex);
    pthread_mutex_unlock(&readersInLibCountMutex);

    Log(i, r - readersInLibCount, w, readersInLibCount, 0);

    pthread_mutex_lock(&readersInLibCountMutex);
    if(--readersInLibCount == 0)
    {
      pthread_mutex_unlock(&libMutex);
    }
    pthread_mutex_unlock(&readersInLibCountMutex);
  }

  return NULL;
}

void NoStarvation(uint32_t r, uint32_t w)
{
  Run(r, Reader, w, Writer);

  pthread_mutex_destroy(&libMutex);
  pthread_mutex_destroy(&queueMutex);
  pthread_mutex_destroy(&readersInLibCountMutex);
}