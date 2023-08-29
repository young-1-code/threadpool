#编译线程池
CC = gcc
LDFLAGS = -lpthread
CFLAGS = -std=c11 -O1
TARGET = mainApp
SRCS = $(wildcard *.c)

ALL:	
	$(CC) $(SRCS) -o $(TARGET) $(CFLAGS) $(LDFLAGS)
.PHONY:clean
clean:
	@rm $(TARGET)
