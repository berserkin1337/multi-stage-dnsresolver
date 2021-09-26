
#ifndef _COMMON_H_
#define _COMMON_H_
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXLINE 4096
#define SA struct sockaddr
void err_n_die(const char *fmt, ...);
#endif // !_COMMON_H_
