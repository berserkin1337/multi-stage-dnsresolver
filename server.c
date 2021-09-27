#include "helper.h"

const char *database[][2] = {
    {"www.google.com", "142.250.205.238"}, {"www.facebook.com", "138.10.129.0"},
    {"www.youtube.com", "190.12.123.10"},  {"www.twitch.tv", "148.121.0.8"},
    {"www.reddit.com", "203.2.9.1"},       {"www.twitter.com", "231.81.292.1"},
    {"www.github.com", "231.89.13.6"}};
int main(int argc, char *argv[]) {
  int db_size = sizeof(database) / sizeof(database[0]);
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
    }
    if (n < 0)
      err_n_die("read error");
    int i = 2;
    while (recvline[i] != '\0')
      i++;
    char stuff[i - 2];
    i = 0;
    while (recvline[i + 2] != '\0') {
      stuff[i] = recvline[i + 2];
    }
    bool found = false;
    const char *sendmesg;
    if (recvline[0] == '1') {
      for (int k = 0; k < db_size; ++k) {
        if (database[k][0] == stuff) {
          sendmesg = database[k][1];
          found = true;
        }
      }
    } else {
      for (int k = 0; k < db_size; ++k) {
        if (database[k][1] == stuff) {
          sendmesg = database[k][0];
          found = true;
        }
      }
    }
    if (!found) {
      sendmesg = "-";
    }
    write(connfd, sendmesg, strlen((char *)sendmsg));
    close(connfd);
  }
}
