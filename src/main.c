#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <socketfun.h>

static void receive_bytes(char *p, int len, int fd) {
  char *ptr = p;
  int i;

  while(ptr < p + len) {
    i = read(fd, ptr, p - ptr + len);
    if(i <= 0) {
      perror("read");
      fprintf(stderr, "errno: %i\n", errno);
      exit(1);
    }
    ptr += i;
  }
}

static void receive_string(char *s, int size, int fd) {
  int len;

  receive_bytes((char *) &len, sizeof(int), fd);

  if(len > size - 1) {
    fprintf(stderr, "%s: receiving %d bytes, expecting %d\n", __func__, len, size);
    exit(1);
  }

  receive_bytes(s, len, fd);
  s[len] = '\0';
}

static void send_bytes(char *p, int len, int fd) {
  char *ptr = p;
  int i;

  while(ptr < p + len) {
    i = write(fd, ptr, p - ptr + len);
    if(i < 0) {
      perror("write");
      fprintf(stderr, "errno: %d\n", errno);
      exit(1);
    }
    ptr += i;
  }
}

void send_string(char *str, int fd) {
  int len = strlen(str);
  send_bytes((char *) &len, sizeof(int), fd);
  send_bytes(str, len, fd);
}

int main(int argc, char **argv) {
  int skt = 0;

  printf("Roses are red\n");
  printf("Violets are blue\n");
  printf("All of our base\n");
  printf("Are belong to you.\n");

  printf("Now listening on port 8080.\n\n");

  skt = serve_socket("localhost", 8080);
  
  for(;;) {
    int client = accept_connection(skt);
    char *buf = (char *) malloc(80);

    receive_string(buf, 80, client);
    printf("%s\n", buf);

    send_string("You're the best!", client);

    close(client);
    free(buf);
  }

  return EXIT_SUCCESS;
}

