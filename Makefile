CC=g++
CFLAGS= -g -w -Wall

client: client.cpp requests.cpp helpers.cpp buffer.cpp interact.cpp
	$(CC) $(CFLAGS) -o client client.cpp requests.cpp helpers.cpp buffer.cpp interact.cpp

run: client
	./client

clean:
	rm -f *.o client
