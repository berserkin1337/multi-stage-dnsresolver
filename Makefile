cc=gcc
CFLAGS = -g
COMMON_OBJS = helper.o
all: $(COMMON_OBJS) $(DB_FILE) server proxy client

client : client.c $(COMMON_OBJS) 
	$(CC)	$(CFLAGS) -o  client client.c $(COMMON_OBJS) 

proxy: proxy.c $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o  proxy proxy.c $(COMMON_OBJS)

server: server.c $(COMMON_OBJS) 
	$(CC) $(CFLAGS) -o server server.c $(COMMON_OBJS)

make clean:
	rm -rf *.dSYM client proxy server *.o
