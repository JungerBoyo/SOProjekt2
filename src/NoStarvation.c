#include "Problems.h"

#include "Global.h"

#include <pthread.h>
#include <stdlib.h>

static pthread_mutex_t libraryMutex  = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t condVarWriters = PTHREAD_COND_INITIALIZER;
static pthread_cond_t condVarReaders = PTHREAD_COND_INITIALIZER;

static pthread_mutex_t readerCountCheckMutex = PTHREAD_MUTEX_INITIALIZER;
static uint32_t readersInLibCount = 0;

static void* Writer(void* iblob)
{
  int i = (int)iblob;
  //while(!quit)
  for(int j=0; j<10; ++j)
  {
    pthread_mutex_lock(&libraryMutex);
      pthread_cond_wait(&condVarWriters, &libraryMutex);
    pthread_mutex_unlock(&libraryMutex);

    Log(i, r, w - 1, 0, 1);
    // write

    pthread_mutex_lock(&libraryMutex);
      pthread_cond_broadcast(&condVarReaders);
    pthread_mutex_unlock(&libraryMutex);
  }

  pthread_mutex_lock(&libraryMutex);
    pthread_cond_signal(&condVarWriters);
  pthread_mutex_unlock(&libraryMutex);

  return NULL;
}

static void* Reader(void* iblob)
{
  int i = (int)iblob;
  //while(!quit)
  for(int j=0; j<10; ++j)
  {
    pthread_mutex_lock(&libraryMutex);
      pthread_cond_wait(&condVarReaders, &libraryMutex);
    pthread_mutex_unlock(&libraryMutex);

    pthread_mutex_lock(&readerCountCheckMutex);
      ++readersInLibCount;
    pthread_mutex_unlock(&readerCountCheckMutex);

    Log(i, r - readersInLibCount, w, readersInLibCount, 0);

    pthread_mutex_lock(&readerCountCheckMutex);
      if(--readersInLibCount == 0) 
      {
        pthread_cond_signal(&condVarWriters);
      }
    pthread_mutex_unlock(&readerCountCheckMutex);
  }

  return NULL;
}

static void InitSignal()
{
  pthread_cond_signal(&condVarWriters);
}

void NoStarvation(uint32_t r, uint32_t w)
{
  //pthread_mutex_init(&libraryMutex, NULL);
  //pthread_mutex_init(&readerCountCheckMutex, NULL);
  //pthread_cond_init(&condVarReaders, NULL);
  //pthread_cond_init(&condVarWriters, NULL);

  Run(r, Reader, w, Writer, InitSignal);

  pthread_mutex_destroy(&libraryMutex);
  pthread_mutex_destroy(&readerCountCheckMutex);
  pthread_cond_destroy(&condVarReaders);
  pthread_cond_destroy(&condVarWriters);
}