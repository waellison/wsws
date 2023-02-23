#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <socketfun.h>

char *inadport_decimal(struct sockaddr_in *sad) {
  char *buf = (char *) malloc(32);
  int address = ntohl(0xffffffff & sad->sin_addr.s_addr);
  int port = (int) ntohs(sad->sin_port);

  sprintf(buf, "%d.%d.%d.%d:%d",
          0xff & (address >> 24),
          0xff & (address >> 16),
          0xff & (address >> 8),
          0xff & address,
          0xffff & port);

  return buf;
}

int serve_socket(char *hostname, int port) {
  struct sockaddr_in socket_address;
  int fildes;
  struct hostent *host_entry;

  if(!(host_entry = gethostbyname(hostname))) {
    perror("gethostbyname");
    exit(1);
  }

  bzero(&socket_address, sizeof(socket_address));
  socket_address.sin_family = AF_INET;
  socket_address.sin_port = htons((short) port);
  socket_address.sin_addr = *(struct in_addr *)(host_entry->h_addr_list[0]);

  if((fildes = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }

  if(bind(fildes, (struct sockaddr *) &socket_address, sizeof(socket_address)) == -1) {
    perror("bind");
    exit(1);
  }

  return fildes;
}

int accept_connection(int fildes) {
  socklen_t length;
  struct sockaddr_in socket_address;
  int response;

  socket_address.sin_family = AF_INET;

  if(listen(fildes, 1) == -1) {
    perror("listen");
    exit(1);
  }

  length = sizeof(socket_address);
  if((response = accept(fildes, (struct sockaddr *) &socket_address, &length)) == -1) {
    perror("accept");
    exit(1);
  }

  return response;
}

int request_connection(char *hostname, int port) {
  struct sockaddr_in socket_address;
  int fildes, ok;
  struct hostent *host_entry;

  if(!(host_entry = gethostbyname(hostname))) {
    perror("gethostbyname");
    exit(1);
  }

  ok = 0;
  while (!ok) {
    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons((short) port);
    socket_address.sin_addr.s_addr = *(unsigned long *)(host_entry->h_addr_list[0]);

    if((fildes = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
      perror("socket");
      exit(1);
    }

    ok = (connect(fildes, (struct sockaddr *) &socket_address, sizeof(socket_address)) != -1);
    if (!ok)
      sleep(1);
  }

  return fildes;
}
