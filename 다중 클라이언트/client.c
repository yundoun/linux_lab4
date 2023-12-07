#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_MESSAGE_LEN 1024

int main()
{
  int client_socket;
  char message[1024];
  struct sockaddr_in server_addr;

  // 클라이언트 소켓 생성
  client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1)
  {
    perror("Socket creation failed");
    exit(1);
  }

  // 서버 주소 설정
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(12345);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  // 서버에 연결
  if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
  {
    perror("Connection failed");
    exit(1);
  }

  printf("채팅에 참여하였습니다. 메시지를 입력하세요:\n");

  while (1)
  {
    // 서버로부터 메시지 수신
    int valread;
    char received_message[MAX_MESSAGE_LEN];
    if ((valread = recv(client_socket, received_message, MAX_MESSAGE_LEN, 0)) <= 0)
    {
      // 서버와의 연결이 끊어진 경우 또는 오류 발생
      if (valread == 0)
      {
        printf("서버와의 연결이 끊어졌습니다.\n");
      }
      else
      {
        perror("Receive failed");
      }
      break;
    }
    received_message[valread] = '\0';

    // 수신한 메시지를 콘솔에 출력
    printf("수신한 메시지: %s", received_message);

    // 사용자로부터 메시지 입력 및 전송
    printf("메시지를 입력하세요: ");
    fgets(message, sizeof(message), stdin);
    send(client_socket, message, strlen(message), 0);
  }

  close(client_socket);

  return 0;
}
