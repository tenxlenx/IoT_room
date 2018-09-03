CC      = g++
CFLAGS  = -std=c++14
LDFLAGS = -pthread -lmosquittopp -lmosquitto 
SOURCE  = 


all: main clean

main: main.o
	$(CC) $(SOURCE) $(CFLAGS) -o $@ $^ $(LDFLAGS)

main.o: main.cpp 
	$(CC) -c $(CFLAGS) $<


.PHONY: clean cleanest

clean:
	rm *.o

cleanest: clean
	rm main
