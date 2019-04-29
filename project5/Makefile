CC=gcc
CFLAGS=-I. -pthread -Wall -Werror
TARGET=test
OBJS=network_helper.o pthread_list.o
DEPS=network_helper.h pthread_list.h

makebase: $(OBJS) $(DEPS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf *.o $(TARGET)
