#include <stdio.h>
#include <pthread.h>

// 쓰레드 함수 정의: 각 쓰레드가 실행할 함수
void *print_hello(void *thread_id)
{
  long tid = (long)thread_id;             // 쓰레드 식별자를 long 형으로 형변환
  printf("Hello from Thread %ld\n", tid); // 해당 쓰레드에서 출력
  pthread_exit(NULL);                     // 쓰레드 종료
}

int main()
{
  pthread_t threads[3]; // 쓰레드 식별자 배열
  int rc;               // pthread_create() 함수의 리턴 코드
  long t;               // 쓰레드 식별자 (쓰레드 번호)

  for (t = 0; t < 3; t++)
  {
    printf("Creating Thread %ld\n", t);                             // 각 쓰레드 생성을 알림
    rc = pthread_create(&threads[t], NULL, print_hello, (void *)t); // 쓰레드 생성
    if (rc)
    {
      printf("Error creating thread; return code: %d\n", rc); // 에러 발생 시 메시지 출력
      return 1;                                               // 프로그램 종료
    }
  }

  pthread_exit(NULL); // 메인 쓰레드가 종료될 때, 모든 쓰레드도 함께 종료
}
