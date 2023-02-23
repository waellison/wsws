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
    if(i < 0) {
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
      fprintf(stderr, "errno: %i\n", errno);
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
  char *buf = malloc(80), *msg = malloc(80);

  skt = request_connection("localhost", 8080);

  if(skt) {
    if(fgets(buf, 80, stdin) == NULL) {
      free(buf);
      exit(1);
    }

    buf[strlen(buf) - 1] = '\0';

    send_string(buf, skt);
    receive_string(msg, 80, skt);

    printf("%s\n", msg);

    free(buf);
    free(msg);
  }

  return EXIT_SUCCESS;
}

