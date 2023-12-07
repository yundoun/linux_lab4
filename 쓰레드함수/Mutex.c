#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4  // 생성할 쓰레드 개수 정의
pthread_mutex_t mutex; // 뮤텍스 (Mutex) 초기화
int shared_data = 0;   // 공유 데이터 초기화

// 쓰레드 함수 정의: 공유 데이터를 안전하게 증가시키는 역할
void *increment_data(void *thread_id)
{
  long tid = (long)thread_id;                                 // 쓰레드 식별자를 long 형으로 형변환
  pthread_mutex_lock(&mutex);                                 // 뮤텍스를 이용하여 임계 영역 진입
  shared_data++;                                              // 공유 데이터 증가
  printf("Thread %ld: shared_data = %d\n", tid, shared_data); // 현재 스레드 번호와 공유 데이터 출력
  pthread_mutex_unlock(&mutex);                               // 임계 영역을 벗어남
  pthread_exit(NULL);                                         // 스레드 종료
}

int main()
{
  pthread_t threads[NUM_THREADS]; // 쓰레드 식별자 배열
  int rc;                         // pthread_create() 함수의 리턴 코드
  long t;                         // 쓰레드 식별자 (쓰레드 번호)

  pthread_mutex_init(&mutex, NULL); // 뮤텍스 초기화

  for (t = 0; t < NUM_THREADS; t++)
  {
    printf("Creating Thread %ld\n", t);                                // 각 쓰레드 생성을 알림
    rc = pthread_create(&threads[t], NULL, increment_data, (void *)t); // 쓰레드 생성
    if (rc)
    {
      printf("Error creating thread; return code: %d\n", rc); // 에러 발생 시 메시지 출력
      return 1;                                               // 프로그램 종료
    }
  }

  for (t = 0; t < NUM_THREADS; t++)
  {
    pthread_join(threads[t], NULL); // 모든 쓰레드의 종료를 대기
  }

  pthread_mutex_destroy(&mutex); // 뮤텍스 제거
  pthread_exit(NULL);            // 메인 쓰레드가 종료될 때, 모든 쓰레드도 함께 종료
}
