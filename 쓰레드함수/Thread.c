#include <stdio.h>
#include <pthread.h>

void *print_hello(void *thread_id)
{
  long tid = (long)thread_id;
  printf("Hello from Thread %ld\n", tid);
  pthread_exit(NULL);
}

int main()
{
  pthread_t threads[3];
  int rc;
  long t;

  for (t = 0; t < 3; t++)
  {
    printf("Creating Thread %ld\n", t);
    rc = pthread_create(&threads[t], NULL, print_hello, (void *)t);
    if (rc)
    {
      printf("Error creating thread; return code: %d\n", rc);
      return 1;
    }
  }

  pthread_exit(NULL);
}
