#include "helper.h"

char *database[][2] = {
    {"www.google.com", "142.250.205.238"}, {"www.facebook.com", "138.10.129.0"},
    {"www.youtube.com", "190.12.123.10"},  {"www.twitch.tv", "148.121.0.8"},
    {"www.reddit.com", "203.2.9.1"},       {"www.twitter.com", "231.81.292.1"},
    {"www.github.com", "231.89.13.6"}};
void print_hex(const char *s) {
  while (*s)
    printf("%02x", (unsigned int)*s++);
  fflush(stdout);
}

int main(int argc, char *argv[]) {
  int db_size = sizeof(database) / sizeof(database[0]);
  /* printf("%d", db_size); */
  fflush(stdout);
  int listenfd, connfd, n;
  struct sockaddr_in servaddr;
  uint8_t buff[MAXLINE + 1];
  uint8_t recvline[MAXLINE + 1];
  if (argc != 2) {
    err_n_die("usage %s <port>", argv[0]);
  }
  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    err_n_die("socket error.");
  bzero(&servaddr, sizeof servaddr);
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(atoi(argv[1]));
  if ((bind(listenfd, (SA *)&servaddr, sizeof(servaddr))) < 0)
    err_n_die("bind error.");

  if ((listen(listenfd, 10)) < 0)
    err_n_die("listen error");
  for (;;) {

    struct sockaddr_in addr;
    socklen_t addr_len;
    printf("Waiting for a connection on a port %d\n", atoi(argv[1]));
    fflush(stdout);
    connfd = accept(listenfd, (SA *)NULL, NULL);
    memset(recvline, 0, MAXLINE);
    if ((n = read(connfd, recvline, MAXLINE - 1)) > 0) {
      fprintf(stdout, "\n%s\n", recvline);
      fflush(stdout);
    }
    if (n < 0)
      err_n_die("read error");
    int x = 2;
    while (recvline[x] != '\0') {
      x++;
      fflush(stdout);
    }
    // 1 www.google.com
    char stuff[x - 2];
    int i = 0;
    int k = 0;
    while (recvline[i + 3] != '\0' && i != x - 1) {
      stuff[i] = recvline[i + 2];
      i++;
    }
    /* printf("zz\n"); */
    /* printf("\n%s\n", stuff); */
    /* fflush(stdout); */
    char sendmesg[MAXLINE];
    int found = 0;
    int foundindex;
    if (recvline[0] == '1') {
      db_size = sizeof(database) / sizeof(database[0]);
      for (k = 0; k < db_size; ++k) {
        if (strcmp(stuff, database[k][0]) == 0) {
          printf("%s\n", database[k][1]);
          printf("%s\n", (char *)sendmsg);
          fflush(stdout);
          found = 1;
          foundindex = k;
          printf("the stuff hass beeen found\n");
          fflush(stdout);
          break;
        }
      }
    } else {
      printf("%s", stuff);
      fflush(stdout);
      for (k = 0; k < db_size; ++k) {
        /* printf("%s\n", database[k][1]); */
        /* printf("%s\n", stuff); */
        if (strcmp(stuff, database[k][1]) == 0) {
          found = 1;
          foundindex = k;
          printf("the stuff hass beeen found\n");
        }
      }
    }
    /* printf("%d\n", found); */
    /* printf("zzz\n"); */
    fflush(stdout);
    if (found == 0) {
      write(connfd, "-", strlen("-"));
    } else {
      int z = (recvline[0] == '1') ? 1 : 0;
      write(connfd, database[foundindex][z], strlen(database[foundindex][z]));
    }
    close(connfd);
  }
}
