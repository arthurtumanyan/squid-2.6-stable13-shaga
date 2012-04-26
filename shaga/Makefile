CC = gcc
CP = cp
RM = rm
PATH = /usr/local/lib/
LIB = libshaga.so
all:

	$(CC) -g -Wall  -c -fPIC -ffreestanding libshaga.c -I/usr/local/include/mysql/ -L/usr/local/lib/mysql
	$(CC) -shared -I/usr/local/include/mysql/ -L/usr/local/lib/mysql -ffreestanding -fPIC -o libshaga.so libshaga.o -lmysqlclient

install:
	$(CP) $(LIB) $(PATH)

uninstall:
	$(RM) $(PATH)/$(LIB)

clean:
	$(RM) *.o $(LIB)
