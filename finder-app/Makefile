CC=$(CROSS_COMPILE)gcc
CFLAGS=-Wall -Wextra -Werror
TARGET=writer

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	rm -f $(TARGET) *.o
