#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>

#define MAX_CLIENTS 10
#define MAX_MESSAGE_LEN 1024

int main()
{
  int server_socket, client_sockets[MAX_CLIENTS];
  fd_set read_fds, master_fds;
  char message[MAX_MESSAGE_LEN];
  int max_fd, i, j;

  for (i = 0; i < MAX_CLIENTS; i++)
  {
    client_sockets[i] = -1;
  }

  // 서버 소켓 생성
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1)
  {
    perror("Socket creation failed");
    exit(1);
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(12345);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  // 서버 소켓을 주소에 바인딩
  if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
  {
    perror("Binding failed");
    exit(1);
  }

  // 서버 소켓을 수신 대기 상태로 변경
  if (listen(server_socket, 5) == -1)
  {
    perror("Listen failed");
    exit(1);
  }

  printf("채팅 서버가 시작되었습니다.\n");

  FD_ZERO(&master_fds);
  FD_SET(server_socket, &master_fds);
  max_fd = server_socket;

  while (1)
  {
    read_fds = master_fds;

    if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1)
    {
      perror("Select failed");
      exit(1);
    }

    for (i = 0; i <= max_fd; i++)
    {
      if (FD_ISSET(i, &read_fds))
      {
        if (i == server_socket)
        {
          // 새로운 클라이언트가 연결
          int new_socket = accept(server_socket, NULL, NULL);
          if (new_socket == -1)
          {
            perror("Accept failed");
          }
          else
          {
            // 클라이언트 소켓을 배열에 추가
            for (j = 0; j < MAX_CLIENTS; j++)
            {
              if (client_sockets[j] == -1)
              {
                client_sockets[j] = new_socket;
                break;
              }
            }

            // 새로운 클라이언트를 모든 클라이언트에게 알림
            for (j = 0; j < MAX_CLIENTS; j++)
            {
              if (client_sockets[j] != -1)
              {
                sprintf(message, "새로운 클라이언트가 참여하였습니다. (Socket %d)\n", new_socket);
                send(client_sockets[j], message, strlen(message), 0);
              }
            }

            // 최대 소켓 번호 갱신
            if (new_socket > max_fd)
            {
              max_fd = new_socket;
            }

            // 새로운 클라이언트를 마스터 FD 집합에 추가
            FD_SET(new_socket, &master_fds);
          }
        }
        else
        {
          // 클라이언트로부터 메시지 수신
          int valread;
          if ((valread = recv(i, message, MAX_MESSAGE_LEN, 0)) <= 0)
          {
            // 오류 또는 연결이 끊어짐
            if (valread == 0)
            {
              // 클라이언트 연결 종료
              printf("클라이언트 (Socket %d)와의 연결이 끊어졌습니다.\n", i);
            }
            else
            {
              perror("Receive failed");
            }

            // 클라이언트 소켓을 닫고 FD 집합에서 제거
            close(i);
            FD_CLR(i, &master_fds);
            for (j = 0; j < MAX_CLIENTS; j++)
            {
              if (client_sockets[j] == i)
              {
                client_sockets[j] = -1;
                break;
              }
            }
          }
          else
          {
            // 수신한 메시지를 모든 클라이언트에게 브로드캐스트
            message[valread] = '\0';
            for (j = 0; j < MAX_CLIENTS; j++)
            {
              if (client_sockets[j] != -1)
              {
                send(client_sockets[j], message, strlen(message), 0);
              }
            }

            // 수신한 메시지를 콘솔에 출력
            printf("수신한 메시지 (Socket %d): %s", i, message);
          }
        }
      }
    }
  }

  return 0;
}
