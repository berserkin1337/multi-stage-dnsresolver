#include "helper.h"
// This programs reads from the user the server address and the port . It then
// asks the user for the request Message Format (1 or 2) . It then queries the
// server based on the format.
#define MAXLINE 4096
#define SA struct sockaddr
int main(int argc, char **argv) {
  int sockfd, n;
  int sendbytes;
  int reqtype;
  char domainname[MAXLINE];
  char ipaddr[MAXLINE];
  struct sockaddr_in servaddr;
  char sendline[MAXLINE];
  char recvline[MAXLINE];
  if (argc != 3) {
    err_n_die("usage: ./%s <Server address> <Server port>", argv[0]);
  }
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0) < 0)) {
    err_n_die("Error While creating socket");
  }
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = SOCK_STREAM;
  servaddr.sin_port = atoi(argv[2]);
  if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
    err_n_die("inet_pton error");
  }
  if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr))) {
    err_n_die("connect failed");
  }
  printf("Enter the type of request Message 1 or 2 ");
  scanf("%d", &reqtype);
  if (reqtype == 1) {
    printf("Enter the domain Name of the website");
    scanf("%s", domainname);
    sprintf(sendline, "1 %s", domainname);
    sendbytes = sizeof(sendline);
  } else if (reqtype == 2) {
    printf("Enter the ip address of the website");
    scanf("%s", ipaddr);
    sprintf(sendline, "2 %s", ipaddr);
    sendbytes = sizeof(sendline);
  } else {
    err_n_die("Incorrect request type");
  }
  if (write(sockfd, &sendline, sendbytes) != sendbytes) {
    err_n_die("write error");
  }
  memset(recvline, 0, MAXLINE);
  while ((n = read(sockfd, recvline, MAXLINE - 1)) > 0) {
    printf("%s", recvline);
    memset(recvline, 0, MAXLINE);
  }
  if (n < 0) {
    err_n_die("read error");
  }
  exit(0);
}
