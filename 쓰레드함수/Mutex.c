#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4
pthread_mutex_t mutex;
int shared_data = 0;

void *increment_data(void *thread_id)
{
  long tid = (long)thread_id;
  pthread_mutex_lock(&mutex);
  shared_data++;
  printf("Thread %ld: shared_data = %d\n", tid, shared_data);
  pthread_mutex_unlock(&mutex);
  pthread_exit(NULL);
}

int main()
{
  pthread_t threads[NUM_THREADS];
  int rc;
  long t;

  pthread_mutex_init(&mutex, NULL);

  for (t = 0; t < NUM_THREADS; t++)
  {
    printf("Creating Thread %ld\n", t);
    rc = pthread_create(&threads[t], NULL, increment_data, (void *)t);
    if (rc)
    {
      printf("Error creating thread; return code: %d\n", rc);
      return 1;
    }
  }

  for (t = 0; t < NUM_THREADS; t++)
  {
    pthread_join(threads[t], NULL);
  }

  pthread_mutex_destroy(&mutex);
  pthread_exit(NULL);
}
