all: server.c client.c
	gcc -g -o server server.c
	#gcc -g -o client client.c
	gcc -c client.c
	gcc -g -o main main.c client.o

clean:
	rm server client main client.o
