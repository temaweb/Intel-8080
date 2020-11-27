# Compiller
CC=g++

# Target
TARGET=cpu

# Compiller flags
#
# -DASMLOG  - Show assembler logs
# -DLOGTEST - Run in test mode
CFLAGS=-std=c++14 -c -Wall -Wno-unknown-pragmas -DLOGTEST
LFLAGS=-lstdc++

all: $(TARGET)

.PHONY: all clean

$(TARGET): example.o cpu.o status.o asmlog.o
	$(CC) $(LFLAGS) Example.o Cpu.o Status.o Asmlog.o -o $(TARGET)

asmlog.o: Asmlog.cpp
	$(CC) $(CFLAGS) Asmlog.cpp
	
status.o: Status.cpp
	$(CC) $(CFLAGS) Status.cpp

cpu.o: Cpu.cpp
	$(CC) $(CFLAGS) Cpu.cpp

example.o: Example.cpp
	$(CC) $(CFLAGS) Example.cpp

clean:
	rm -rf *.o $(TARGET)
	
run: all
	@./$(TARGET)
