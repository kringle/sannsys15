TARGET = assB

CFLAGS = -g -Wall -D_GNU_SOURCE 

LDFLAGS = -g -lrt -pthread -lrt -lcomedi -lm

SOURCES = $(shell find -name "*.c")

all: $(TARGET)

OBJECTS = $(SOURCES:.c=.o)

$(TARGET): $(OBJECTS)
	gcc $^ -o $@ $(LDFLAGS)

%.o : %.c
	gcc $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJECTS)
