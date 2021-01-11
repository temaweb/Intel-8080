# Compiller
CC=g++

# Target
TARGET=cpu

# Buld directory
BUILDDIR=build
SOURCEDIR=.

# Compiller flags
CFLAGS=-std=c++14 -c -Wall -Wno-unknown-pragmas -DLOGTEST
LDFLAGS=-lstdc++

SOURCES=$(wildcard $(SOURCEDIR)/*.cpp)
OBJECTS=$(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

all: dir $(SOURCES) $(BUILDDIR)/$(TARGET)

dir:
	mkdir -p $(BUILDDIR)
    
.PHONY: all clean

$(BUILDDIR)/$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@

$(OBJECTS): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(BUILDDIR)/*o $(BUILDDIR)/$(TARGET)
	
run: all
	@./$(BUILDDIR)/$(TARGET)
