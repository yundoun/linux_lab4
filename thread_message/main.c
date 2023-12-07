#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_CLIENTS 5

// 공유 데이터 구조
typedef struct
{
  char message[256];
  int new_message;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
} SharedData;

// 서버 쓰레드 함수
void *server_thread(void *arg)
{
  SharedData *shared = (SharedData *)arg;

  while (1)
  {
    pthread_mutex_lock(&shared->mutex);

    // 새 메시지가 있을 때까지 대기
    while (!shared->new_message)
    {
      pthread_cond_wait(&shared->cond, &shared->mutex);
    }

    // 모든 클라이언트에게 메시지 방송
    printf("Server broadcasting: %s\n", shared->message);
    shared->new_message = 0;

    pthread_cond_broadcast(&shared->cond);
    pthread_mutex_unlock(&shared->mutex);
  }

  return NULL;
}

// 클라이언트 쓰레드 함수
void *client_thread(void *arg)
{
  SharedData *shared = (SharedData *)arg;
  int client_id = rand() % 1000;

  while (1)
  {
    pthread_mutex_lock(&shared->mutex);

    // 메시지 생성
    sprintf(shared->message, "Message from client %d", client_id);
    shared->new_message = 1;

    // 서버에게 메시지 알림
    pthread_cond_signal(&shared->cond);

    pthread_mutex_unlock(&shared->mutex);

    // 잠시 대기
    sleep(1);
  }

  return NULL;
}

int main()
{
  pthread_t server, clients[MAX_CLIENTS];
  SharedData shared;
  pthread_mutex_init(&shared.mutex, NULL);
  pthread_cond_init(&shared.cond, NULL);
  shared.new_message = 0;

  // 서버 쓰레드 생성
  pthread_create(&server, NULL, server_thread, &shared);

  // 클라이언트 쓰레드들 생성
  for (int i = 0; i < MAX_CLIENTS; i++)
  {
    pthread_create(&clients[i], NULL, client_thread, &shared);
  }

  // 쓰레드들이 끝날 때까지 대기
  pthread_join(server, NULL);
  for (int i = 0; i < MAX_CLIENTS; i++)
  {
    pthread_join(clients[i], NULL);
  }

  pthread_mutex_destroy(&shared.mutex);
  pthread_cond_destroy(&shared.cond);

  return 0;
}
