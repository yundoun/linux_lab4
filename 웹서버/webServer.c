#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_get_request(int client_socket)
{
  char response[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, World!\n";
  send(client_socket, response, sizeof(response), 0);
  printf("HTTP GET 요청을 받았습니다.\n");
}

void handle_post_request(int client_socket, char *post_data)
{
  printf("HTTP POST 요청을 받았습니다.\n");
  printf("POST 데이터: %s\n", post_data);
}

int main()
{
  int server_socket, client_socket;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_len = sizeof(client_addr);

  // 소켓 생성
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1)
  {
    perror("소켓 생성 실패");
    exit(1);
  }

  // 서버 주소 설정
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  // 소켓을 주소에 바인딩
  if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
  {
    perror("바인딩 실패");
    exit(1);
  }

  // 대기열 모드로 설정
  if (listen(server_socket, 5) == -1)
  {
    perror("대기열 모드 설정 실패");
    exit(1);
  }

  printf("서버가 %d 포트에서 시작되었습니다...\n", PORT);

  while (1)
  {
    // 클라이언트 연결 대기
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
    if (client_socket == -1)
    {
      perror("클라이언트 연결 실패");
      exit(1);
    }

    char buffer[BUFFER_SIZE] = {0};
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

    if (bytes_received > 0)
    {
      // HTTP 요청 처리
      if (strncmp(buffer, "GET", 3) == 0)
      {
        handle_get_request(client_socket);
      }
      else if (strncmp(buffer, "POST", 4) == 0)
      {
        char *post_data = strstr(buffer, "\r\n\r\n");
        if (post_data)
        {
          post_data += 4;
          handle_post_request(client_socket, post_data);
        }
      }
    }

    close(client_socket);
  }

  close(server_socket);

  return 0;
}
