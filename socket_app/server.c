#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>      // inet_addr
#include <netdb.h>
#include <unistd.h>

#define MAX 8
#define PORT 8080
#define SA struct sockaddr

struct data
{
  int rollno;
  int result;
}client;

char *client_ip;
int sockfd, connfd, len;
struct sockaddr_in servaddr, cli;
FILE *fptr;

static void create_socket()
{
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));
}

static void bind_socket()
{
  // assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");
}

static void listen_connection()
{
  // Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);
}

static void accept_connection()
{
  // Accept the data packet from client and verification
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server acccept failed...\n");
		exit(0);
	}
	else
		printf("server acccept the client...\n");


  client_ip = (char *)malloc(MAX*sizeof(char));
  inet_ntop(AF_INET, &(cli.sin_addr), client_ip, len);
  printf("client ip:%s\n", client_ip);

}

// Function designed for chat between client and server.
void func(int sockfd)
{
  //char buff[MAX];
  int count;
	int i;

  fptr = fopen("database.txt", "a");
  if(NULL == fptr)
    perror("error in opening file");

	// infinite loop for chat
//  for (i=0;i<2;i++){
//  for(; ;){

    // appending client IP to the file
    fwrite( "client ip:", sizeof(char), 10, fptr);
    fwrite( client_ip, sizeof(char), 9, fptr);
		// read the message from client and copy it in buffer

    read(sockfd, &client, sizeof(client));
    printf("In server rollno: %d result: %d\n", client.rollno, client.result);

    // appending to the file
    fwrite( " client rollno:", sizeof(char), 15, fptr);
    count = fwrite( &client.rollno,sizeof(int),4, fptr);

    // appending to the file
    fwrite( " client result:", sizeof(char), 16, fptr);
    fwrite( &client.result,sizeof(int),4, fptr);
    fwrite( "\n", sizeof(char), 1, fptr);

  //}
  fclose(fptr);
}

void initialize_socket()
{
  create_socket();
  bind_socket();
  listen_connection();
  accept_connection();
}

// Driver function
int main()
{
  initialize_socket();

	func(connfd);

	// After chatting close the socket
	close(sockfd);
}
