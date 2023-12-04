#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
int data_ready = 0;

void *producer(void *arg)
{
  sleep(1); // 생산 시간 지연
  pthread_mutex_lock(&mutex);
  data_ready = 1;
  printf("Producer: Data is ready.\n");
  pthread_cond_signal(&condition); // 소비자에게 신호 보냄
  pthread_mutex_unlock(&mutex);
  pthread_exit(NULL);
}

void *consumer(void *arg)
{
  pthread_mutex_lock(&mutex);
  while (data_ready == 0)
  {
    printf("Consumer: Waiting for data...\n");
    pthread_cond_wait(&condition, &mutex); // 데이터가 준비될 때까지 대기
  }
  printf("Consumer: Data received.\n");
  pthread_mutex_unlock(&mutex);
  pthread_exit(NULL);
}

int main()
{
  pthread_t producer_thread, consumer_thread;

  pthread_create(&producer_thread, NULL, producer, NULL);
  pthread_create(&consumer_thread, NULL, consumer, NULL);

  pthread_join(producer_thread, NULL);
  pthread_join(consumer_thread, NULL);

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&condition);
  pthread_exit(NULL);
}
