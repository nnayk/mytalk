CC=gcc
CFLAGS = -g -c -pedantic -Wall -Werror -I
INCLUDE = ~pn-cs357/Given/Talk/include
LDFLAGS = -g -pedantic -Wall -Werror
LIBDIRS = ~pn-cs357/Given/Talk/lib64
LIBS = -ltalk -lncurses
OBJS =  mytalk.o server.o client.o safeIO.o

mytalk: $(OBJS)
	$(CC) $(LDFLAGS) -o mytalk $(OBJS) -L $(LIBDIRS) $(LIBS) 

mytalk.o: mytalk.c
	$(CC) $(CFLAGS) $(INCLUDE) mytalk.c

client.o: client.c
	$(CC) $(CFLAGS) $(INCLUDE) client.c

server.o: server.c
	$(CC) $(CFLAGS) $(INCLUDE) server.c

safeIO.o: safeIO.c
	$(CC) $(CFLAGS) $(INCLUDE) safeIO.c

clean:	
	rm -f *.o
