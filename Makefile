cc=gcc
CFLAGS = -g
COMMON_OBJS = helper.o
DB_FILE=db.o
all: $(COMMON_OBJS) $(DB_FILE) server proxy client

client : client.c $(COMMON_OBJS) 
	$(CC)	$(CFLAGS) client client.c $(COMMON_OBJS) 

proxy: proxy.c $(COMMON_OBJS)
	$(CC) $(CFLAGS) proxy proxy.c $(COMMON_OBJS)

server: server.c $(COMMON_OBJS) $(DB_FILE)
	$(CC) $(CFLAGS) server server.c $(COMMON_OBJS) $(DB_FILE)
