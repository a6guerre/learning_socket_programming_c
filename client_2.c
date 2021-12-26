#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(9002);
  addr.sin_addr.s_addr = INADDR_ANY;

  connect(sock, (struct sockaddr *) &addr, sizeof(addr));

  char buf[256];
  int len;
  recv(sock, buf, sizeof(buf), 0);

  printf("Response: %s\n", buf);
  char msg[3][256] = {"^6566$kjg^qazwsx$123^787", "^45454$lol^rfvtgbyhn", "^yuiop$dfg^bnm"};

  for( int i = 0; i <  sizeof(msg)/sizeof(msg[0]); ++i)
  {  
    send(sock, msg[i], strlen(msg[i]), 0);
    memset(buf, 0, 256);
    recv(sock, buf, sizeof(buf), 0);
    printf("Received message from server: %s\n", buf);
    //sleep(1);
  }

  close(sock);

  return 0;
}
