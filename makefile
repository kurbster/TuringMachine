EXEC = tm
CC = g++
CFLAGS = -Wall

$(EXEC): utm.cc utm.h
	$(CC) $(CFLAGS) -o $(EXEC) utm.cc utm.h

clean:
	rm *.o
