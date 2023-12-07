#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // sleep 함수를 사용하기 위한 헤더

#define BUFFER_SIZE 5   // 버퍼 크기
#define NUM_PRODUCERS 2 // 생산자 수
#define NUM_CONSUMERS 2 // 소비자 수

int buffer[BUFFER_SIZE];                                    // 제한 버퍼
int in = 0, out = 0;                                        // 버퍼에 데이터를 넣고 빼는 인덱스
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;          // 뮤텍스 초기화
pthread_cond_t buffer_not_full = PTHREAD_COND_INITIALIZER;  // 버퍼가 가득차지 않았음을 나타내는 조건 변수
pthread_cond_t buffer_not_empty = PTHREAD_COND_INITIALIZER; // 버퍼가 비어있지 않음을 나타내는 조건 변수

void *producer(void *arg)
{
  int item;
  while (1)
  {
    item = rand() % 100; // 임의의 아이템 생성
    pthread_mutex_lock(&mutex);
    while (((in + 1) % BUFFER_SIZE) == out)
    { // 버퍼가 가득 찼을 때 대기
      printf("Producer is waiting...\n");
      pthread_cond_wait(&buffer_not_full, &mutex);
    }
    buffer[in] = item; // 아이템을 버퍼에 추가
    printf("Produced: %d\n", item);
    in = (in + 1) % BUFFER_SIZE;
    pthread_cond_signal(&buffer_not_empty); // 버퍼가 비어있음을 알림
    pthread_mutex_unlock(&mutex);
    sleep(1); // 1초 동안 스레드를 일시 중지
  }
}

void *consumer(void *arg)
{
  int item;
  while (1)
  {
    pthread_mutex_lock(&mutex);
    while (in == out)
    { // 버퍼가 비어있을 때 대기
      printf("Consumer is waiting...\n");
      pthread_cond_wait(&buffer_not_empty, &mutex);
    }
    item = buffer[out]; // 아이템을 버퍼에서 꺼냄
    printf("Consumed: %d\n", item);
    out = (out + 1) % BUFFER_SIZE;
    pthread_cond_signal(&buffer_not_full); // 버퍼가 가득차지 않았음을 알림
    pthread_mutex_unlock(&mutex);
    sleep(1); // 1초 동안 스레드를 일시 중지
  }
}

int main()
{
  pthread_t producer_threads[NUM_PRODUCERS];
  pthread_t consumer_threads[NUM_CONSUMERS];

  // 생산자 스레드 생성
  for (int i = 0; i < NUM_PRODUCERS; i++)
  {
    pthread_create(&producer_threads[i], NULL, producer, NULL);
  }

  // 소비자 스레드 생성
  for (int i = 0; i < NUM_CONSUMERS; i++)
  {
    pthread_create(&consumer_threads[i], NULL, consumer, NULL);
  }

  // 스레드 종료 대기
  for (int i = 0; i < NUM_PRODUCERS; i++)
  {
    pthread_join(producer_threads[i], NULL);
  }
  for (int i = 0; i < NUM_CONSUMERS; i++)
  {
    pthread_join(consumer_threads[i], NULL);
  }

  return 0;
}
