#include "Problems.h"

#include "Global.h"

#include <pthread.h>
#include <stdlib.h>

static volatile uint32_t readersInLibCount = 0;
static volatile uint32_t writersWaitingCount = 0;
static volatile uint32_t writerInLib = 0;

static pthread_mutex_t condMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t condVar = PTHREAD_COND_INITIALIZER;

static void* Writer(void* iblob)
{
  int i = (int)iblob;
  while(!quit)
  {
    pthread_mutex_lock(&condMutex);
    ++writersWaitingCount;
    while(readersInLibCount > 0 || writerInLib)
    {
      pthread_cond_wait(&condVar, &condMutex);
    }
    --writersWaitingCount;
    writerInLib = 1;
    pthread_mutex_unlock(&condMutex);  

    Log(i, r, w - 1, 0, 1);
    
    pthread_mutex_lock(&condMutex);
    writerInLib = 0;
    pthread_cond_broadcast(&condVar);
    pthread_mutex_unlock(&condMutex);
  }

  return NULL;
}

static void* Reader(void* iblob)
{
  int i = (int)iblob;
  while(!quit)
  {
    pthread_mutex_lock(&condMutex);
    while(writersWaitingCount > 0 || writerInLib)
    {
      pthread_cond_wait(&condVar, &condMutex);
    }
    ++readersInLibCount;
    pthread_mutex_unlock(&condMutex);

    Log(i, r - readersInLibCount, w, readersInLibCount, 0);

    pthread_mutex_lock(&condMutex);
    if(--readersInLibCount == 0)
    {
      pthread_cond_broadcast(&condVar);
    }
    pthread_mutex_unlock(&condMutex);
  }

  return NULL;
}

void ReadersStarvation(uint32_t r, uint32_t w)
{
  Run(r, Reader, w, Writer);

  pthread_mutex_destroy(&condMutex);
  pthread_cond_destroy(&condVar);
}