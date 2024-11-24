TARGET = eventease

SRC = ./src
INCLUDE = ./include
FLAGS = -I$(INCLUDE) -Wall -O2 -march=native


CC = gcc
FLAGS = -I$(INCLUDE) -Wall -O2 -march=native

OBJECTS = $(patsubst $(SRC)/%.c,%.o,$(wildcard $(SRC)/*.c))

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

# Default target
all: $(TARGET).so

.PHONY: clean install uninstall

# Compile source files into object files
%.o: $(SRC)/%.c $(INCLUDE)/%.h
	$(CC) -fPIC -c -o $@ $< $(FLAGS)

# Build the shared library
$(TARGET).so: $(OBJECTS)
	$(CC) -shared -o $@ $^ $(FLAGS)

# Clean build artifacts
clean:
	rm -f $(TARGET).so $(SRC)/*.o

# Install headers and shared library
install:
	mkdir -p $(PREFIX)/include/$(TARGET)
	cp $(INCLUDE)/*.h $(PREFIX)/include/$(TARGET)
	cp $(TARGET).so $(PREFIX)/lib/$(TARGET).so

# Uninstall headers and shared library
uninstall:
	rm -rf $(PREFIX)/include/$(TARGET)
	rm -rf $(PREFIX)/lib/$(TARGET).so
