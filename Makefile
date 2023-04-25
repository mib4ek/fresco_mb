.PHONY = all clean

CC = gcc			# compiler to use

CFLAGS = -g -Wall -fshort-enums -Os
LINKERFLAG = -lm

SOURCES    = $(wildcard *.c)
OBJECTS  = $(patsubst %.c, %.o, $(SOURCES))

FOLDER_NAME=$(shell basename $(CURDIR))
TARGET:=$(FOLDER_NAME).o

all: clean $(TARGET)
	$(CC) $(TARGET) -o $(FOLDER_NAME)
	@echo $(OBJECTS) $(TARGET)

$(OBJECTS):
	@echo $@
	$(CC) $(CFLAGS) -c ${@:.o=.c} -o $@

clean:
	@echo "Cleaning up..."
	rm -rvf *.o $(TARGET) $(FOLDER_NAME)

$(TARGET): $(OBJECTS)
	@echo "Linking ... $@"
	$(LD) -r $^ -o $@