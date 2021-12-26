#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

typedef enum
{
  WAIT_FOR_MSG,
  IN_MSG,
} state_t;

int main(void)
{
  char recv_buf[256];
  memset(recv_buf, 0, 256);
  char out_buf[256];
  memset(out_buf, 0, 256); 

  int recv_len, j, k = 0;
  state_t state = WAIT_FOR_MSG;

  int server_socket;
  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  //define the server address
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(9002);
  server_address.sin_addr.s_addr = INADDR_ANY;

  bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));
  listen(server_socket, 5);

  int client_socket;
  client_socket = accept(server_socket, NULL, NULL);
 
  // init client protocol
  char *msg = "*";
  size_t len = strlen(msg);

  send(client_socket, msg, len, 0);
 
  do
  {
    recv_len = recv(client_socket, recv_buf, sizeof(recv_buf), 0);
    printf("Received from client: %s %d bytes\n", recv_buf, recv_len);
    k = 0;
    for(int i = 0; i < recv_len; ++i)
    {
      //out_buf[k] = recv_buf[i];
      switch(state)
      {
        case WAIT_FOR_MSG:
          if(recv_buf[i] == '^')
	  {
            state = IN_MSG;
	  }
          break;
        case IN_MSG:
	  if(recv_buf[i] == '$')
	  {
            state = WAIT_FOR_MSG;
	  }
	  else
	  {
            out_buf[k] = ++recv_buf[i];
	    ++k;
	  }
          break;
      }
    }
    out_buf[k] = '\0';
    send(client_socket, out_buf, strlen(out_buf), 0);
  }while(0);

  close(server_socket);

  return 0;
}
