#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
  int server_socket;
  struct sockaddr_in server_addr;
  char message[1024];

  // 서버 소켓 생성
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1)
  {
    perror("Error in socket creation");
    exit(1);
  }

  // 서버 주소 설정
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(8080); // 포트 번호 8080 사용
  server_addr.sin_addr.s_addr = INADDR_ANY;

  // 서버 소켓을 주소와 바인딩
  if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
  {
    perror("Error in socket binding");
    exit(1);
  }

  // 서버 소켓을 수신 모드로 대기
  listen(server_socket, 5);
  printf("Server listening on port 8080...\n");

  // 클라이언트 요청 대기
  int client_socket;
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);
  client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);

  // 클라이언트로부터 메시지 수신
  recv(client_socket, message, sizeof(message), 0);
  printf("Received message from client: %s\n", message);

  // 클라이언트에게 응답 메시지 전송
  char response[] = "Hello from server!";
  send(client_socket, response, sizeof(response), 0);

  // 소켓 종료
  close(server_socket);
  close(client_socket);

  return 0;
}
