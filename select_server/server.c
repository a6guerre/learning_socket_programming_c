#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>

#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
  struct sockaddr_in addr;
  struct sockaddr_storage client_addr;
  socklen_t client_addr_len;
  int server_socket, new_socket;
  fd_set master, readfs;
  int fdmax;

  server_socket = socket(AF_INET, SOCK_STREAM, 0);
    
  addr.sin_family = AF_INET;
  addr.sin_port = htons(9002);
  addr.sin_addr.s_addr = INADDR_ANY;

  bind(server_socket, (struct sockaddr *)&addr, sizeof(struct sockaddr));
  
  listen(server_socket, 10);
  
  FD_ZERO(&master);
  FD_ZERO(&readfs);

  FD_SET(server_socket, &master);
  fdmax = server_socket;
  
  char buf[256];

  for(;;)
  {
    readfs = master;
    if(select(fdmax + 1, &readfs, NULL, NULL, NULL) == -1)
    {
      perror("Error on select\n");
      exit(4);
    }

    for(int i = 0; i <= fdmax; ++i)
    {
      if(FD_ISSET(i, &readfs))
      {
        if(i == server_socket)
	{
          new_socket = accept(server_socket, NULL, NULL);
          if(new_socket != -1)
	    printf("New connection accepted\n");

	  FD_SET(new_socket, &master);
	  if(new_socket > fdmax)
	  {
            fdmax = new_socket;
          }
	}
	else
	{
          int len = recv(i, buf, sizeof(buf), 0);
          printf("received\n");
	  if(len == -1)
	  {
            close(i);
            perror("Receive Error\n");
            FD_CLR(i, &master);
	  }
          else if(len == 0)
	  {
            printf(" Connetion closed by client\n");
	    close(i);
	    FD_CLR(i, &master);
	  }
	  else
	  {
	    printf("Received: %s\n", buf);
            for (int j = 0; j <= fdmax; ++j)
	    {
              if(FD_ISSET(j, &master))
	      {
                if( j != i && j != server_socket)
		{
                  if(send(j, buf, strlen(buf), 0) < 0)
		    perror("Send: ");
		}
	      }
	    }
	  }
	 
	}
      }
    }
  }

  close(server_socket);

  return 0;
}
