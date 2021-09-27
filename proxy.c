#include "helper.h"

#define SIZE 3
void sendAndReceive(char *message, char *serverip, int serverport,
                    char *servrecvline);
int main(int argc, char *argv[]) {
  char *cache[][2] = {{"0", "0"}, {"0", "0"}, {"0", "0"}};
  int index = 0;
  char *cantfind = "entry not found in the database";
  int listenfd, connfd, n;
  struct sockaddr_in servaddr;
  uint8_t buff[MAXLINE + 1];
  uint8_t recvline[MAXLINE + 1];
  char servrecvline[MAXLINE + 1];
  char *serverip;
  int serverport;
  if (argc != 2) {
    err_n_die("usage: ./%s <port>", argv[0]);
  }
  printf("Please Enter the dns server's ip address");
  scanf("%s", serverip);
  printf("Please enter server's port");
  scanf("%d", &serverport);
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(atoi(argv[1]));
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    err_n_die("socket error");
  if (bind(listenfd, (SA *)&servaddr, sizeof(servaddr)) < 0) {
    err_n_die("bind error");
  }
  if (listen(listenfd, 10) < 0) {
    err_n_die("listen error");
  }
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
    int msgsize = 0;
    while (recvline[msgsize] != '\n') {
      msgsize++;
    }
    uint8_t receivemsg[msgsize];
    strncpy((char *)receivemsg, (char *)recvline, msgsize);
    char stuff[msgsize - 2];
    int i = 2;
    while (receivemsg[i] != '\0') {
      stuff[i - 2] = receivemsg[i];
      i++;
    }
    bool cached = false;
    if (receivemsg[0] == '1') {
      for (int k = 0; k < 3; ++k) {
        if (cache[i][0] == stuff) {
          cached = true;
          strcpy((char *)buff, cache[i][1]);
          write(connfd, buff, sizeof(buff));
          close(connfd);
        }
      }
      if (!cached) {
        sendAndReceive((char *)receivemsg, serverip, serverport, servrecvline);
        char *returnstr;
        if (servrecvline[0] == '-') {
          returnstr = cantfind;
        } else {
          returnstr = servrecvline;
          cache[index][0] = stuff;
          cache[index][1] = servrecvline;
        }

        index = (index + 1) % 3;
        write(connfd, returnstr, sizeof(servrecvline));
        close(connfd);
      }
    } else {
      for (int k = 0; k < 3; ++k) {
        if (cache[i][1] == stuff) {
          cached = true;
          strcpy((char *)buff, cache[i][0]);
          write(connfd, buff, sizeof(buff));
          close(connfd);
        }
      }
      if (!cached) {
        sendAndReceive((char *)receivemsg, serverip, serverport, servrecvline);
        write(connfd, servrecvline, sizeof(servrecvline));
        close(connfd);
      }
    }
  }
  return 0;
}

void sendAndReceive(char *message, char *serverip, int port,
                    char *servrecvline) {

  int sockfd, n;
  int sendbytes;
  struct sockaddr_in servaddr;
  char sendline[MAXLINE];
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    err_n_die("Error while creating socket");
  }
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);
  if (inet_pton(AF_INET, serverip, &servaddr.sin_addr) <= 0) {
    err_n_die("inet_pton error for %s", serverip);
  }
  if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0) {
    err_n_die("connect failed");
  }
  strcpy(sendline, message);
  sendbytes = strlen(sendline);
  if (write(sockfd, sendline, sendbytes) != sendbytes) {
    err_n_die("write error");
  }

  memset(servrecvline, 0, MAXLINE);
  while ((n = read(sockfd, servrecvline, MAXLINE - 1)) > 0) {
    printf("%s", servrecvline);
  }
  if (n < 0) {
    err_n_die("read error");
  }
}
