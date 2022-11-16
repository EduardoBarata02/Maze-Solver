# Roap make file

CC= gcc #compiler
CFLAGS= -Wall -std=c99 -g #flags
#generate variables to compile
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c, %.o, $(SRCS))
HEADERS=$(wildcard *.h)

#executable file name
TARGET= roap

all: $(TARGET)

$(TARGET): $(OBJS) Makefile
	$(CC) $(CFLAGS) $(OBJS) -o $@

#Dependancies list
%.o: %.c $(HEADERS) Makefile
	$(CC) -c $(CFLAGS) -o $@ $<

#Make clean
clean:
	rm -f *.o $(TARGET)



