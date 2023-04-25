.PHONY = all clean

CC = gcc			# compiler to use

CFLAGS = -g -Wall -fshort-enums -Os
LINKERFLAG = -lm

SUBDIRECTORY_LIST = $(wildcard */)
SUBDIRECTORY = $(filter-out $(EXCLUDE_LIST),$(SUBDIRECTORY_LIST))

SUBSUBDIRECTORY := $(foreach dir,$(SUBDIRECTORY),$(wildcard $(dir)*/))

SUBDIRECTORY = $(sort $(dir $(SUBSUBDIRECTORY)))
SOURCES    = $(foreach dir,$(SUBDIRECTORY),$(wildcard $(dir)*.c))
OBJECTS  = $(patsubst %.c, %.o, $(SOURCES))

FOLDER_NAME=$(shell basename $(CURDIR))
TARGET:=$(FOLDER_NAME).o

all: clean $(TARGET)
	$(CC) $(TARGET) -o $(FOLDER_NAME)
	@echo $(OBJECTS) $(TARGET)
	

$(OBJECTS):
	@echo $@
	$(CC) $(CFLAGS) -c ${@:.o=.c} -o $@

$(TARGET): $(OBJECTS)
	@echo "Linking ... $@"
	$(LD) -r $^ -o $@
clean:
	@echo "Cleaning up..."
	rm -rvf code/*.o $(TARGET) $(FOLDER_NAME)

