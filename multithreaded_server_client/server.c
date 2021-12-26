#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

typedef enum
{
  WAIT_FOR_MSG,
  IN_MSG,
} state_t;

//Used for ensuring mutual exlcusive access of output
pthread_mutex_t lock;

void *protocol_exec(void *args)
{
  int *client_socket = (int *) args;
  int recv_len, k;
  char recv_buf[100], out_buf[100];
  state_t state = WAIT_FOR_MSG;

  // init client protocol
  char *msg = "*";
  size_t len = strlen(msg);
  send(*client_socket, msg, len, 0);
 
  while(1)
  {
    recv_len = recv(*client_socket, recv_buf, sizeof(recv_buf), 0);
    k = 0;
    if(recv_len > 0)
    {
      pthread_mutex_lock(&lock);
      printf("Thread ID: %ld -  Received from client: %s %d bytes\n", pthread_self(), recv_buf, recv_len);
      pthread_mutex_unlock(&lock);
      state = WAIT_FOR_MSG;
      for(int i = 0; i < recv_len; ++i)
      {
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
      send(*client_socket, out_buf, strlen(out_buf), 0); 
    }
    else if(recv_len == 0)
    {
      printf("Connection closed, recv_len == 0\n");
      break;
    }
  }
  close(*client_socket);
  return NULL; 
}

int main(void)
{
  char recv_buf[256], out_buf[256];
  memset(recv_buf, 0, 256);
  memset(out_buf, 0, 256); 

  if(pthread_mutex_init(&lock, NULL) != 0)
  {
    printf("\n Mutex init has failed\n");
  }
 
  int server_socket;
  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  //define the server address
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(9002);
  server_address.sin_addr.s_addr = INADDR_ANY;

  bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));
  listen(server_socket, 5);

  int *client_socket = (int *)malloc(sizeof(int));

  pthread_t threads[50];
  int i = 0;
  for(;;)
  {
    *client_socket = accept(server_socket, NULL, NULL);
    printf("Accepting connection\n");
    if(pthread_create(&threads[i], NULL, protocol_exec, client_socket) != 0)
    {
      printf("Error on thread creation\n");
    }
    pthread_detach(threads[i]);
    ++i;
  }


  close(server_socket);

  return 0;
}
