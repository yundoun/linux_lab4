#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
  int client_socket;
  struct sockaddr_in server_addr;
  char message[1024];

  // 클라이언트 소켓 생성
  client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1)
  {
    perror("Error in socket creation");
    exit(1);
  }

  // 서버 주소 설정
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(8080);                   // 서버와 동일한 포트 사용
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 서버 IP 주소

  // 서버에 연결
  if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
  {
    perror("Error in socket connection");
    exit(1);
  }

  // 메시지 전송
  char request[] = "Hello from client!";
  send(client_socket, request, sizeof(request), 0);
  printf("Sent message to server: %s\n", request);

  // 서버로부터 응답 메시지 수신
  recv(client_socket, message, sizeof(message), 0);
  printf("Received response from server: %s\n", message);

  // 소켓 종료
  close(client_socket);

  return 0;
}
