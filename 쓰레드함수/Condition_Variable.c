#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;   // 뮤텍스 초기화
pthread_cond_t condition = PTHREAD_COND_INITIALIZER; // 조건 변수 초기화
int data_ready = 0;                                  // 데이터 준비 여부를 나타내는 플래그

// 생산자 스레드 함수 정의
void *producer(void *arg)
{
  sleep(1);                             // 생산 시간 지연
  pthread_mutex_lock(&mutex);           // 뮤텍스를 이용하여 임계 영역 진입
  data_ready = 1;                       // 데이터가 준비되었음을 표시
  printf("Producer: Data is ready.\n"); // 데이터가 준비되었다고 출력
  pthread_cond_signal(&condition);      // 소비자에게 데이터가 준비되었음을 신호로 알림
  pthread_mutex_unlock(&mutex);         // 임계 영역을 벗어남
  pthread_exit(NULL);                   // 스레드 종료
}

// 소비자 스레드 함수 정의
void *consumer(void *arg)
{
  pthread_mutex_lock(&mutex); // 뮤텍스를 이용하여 임계 영역 진입
  while (data_ready == 0)
  {
    printf("Consumer: Waiting for data...\n"); // 데이터가 준비될 때까지 대기
    pthread_cond_wait(&condition, &mutex);     // 조건 변수를 사용하여 대기하고, 뮤텍스 해제
  }
  printf("Consumer: Data received.\n"); // 데이터를 받았다고 출력
  pthread_mutex_unlock(&mutex);         // 임계 영역을 벗어남
  pthread_exit(NULL);                   // 스레드 종료
}

int main()
{
  pthread_t producer_thread, consumer_thread;

  // 생산자 스레드와 소비자 스레드 생성
  pthread_create(&producer_thread, NULL, producer, NULL);
  pthread_create(&consumer_thread, NULL, consumer, NULL);

  // 생산자 스레드와 소비자 스레드의 종료를 대기
  pthread_join(producer_thread, NULL);
  pthread_join(consumer_thread, NULL);

  pthread_mutex_destroy(&mutex);    // 뮤텍스 제거
  pthread_cond_destroy(&condition); // 조건 변수 제거
  pthread_exit(NULL);               // 메인 스레드 종료
}
