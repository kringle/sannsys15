TARGET = assA

CFLAGS = -g -Wall 

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
