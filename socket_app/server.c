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

struct node
{
  //int rollno;
  char* rollno;
  char *ipaddress;
  char *result;
}client;


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

  client.ipaddress = (char *)malloc(MAX*sizeof(char));
  inet_ntop(AF_INET, &(cli.sin_addr), client.ipaddress, len);
  printf("client ip:%s\n", client.ipaddress);

}

// Function designed for chat between client and server.
void func(int sockfd)
{
  char buff[MAX];
  int count;
	int i;

  fptr = fopen("database.txt", "a");
  if(NULL == fptr)
    perror("error in opening file");

  fwrite( "client ipaddress:", sizeof(char), 18, fptr);
  count = fwrite( client.ipaddress, sizeof(char), 9, fptr);
  //printf("return value of fwrite is %d\n", count);
  count = fwrite( "\n", sizeof(char), 1, fptr);
  //printf("return value of fwrite is %d\n", count);

	// infinite loop for chat
  //for (i=0;i<5;i++) {
  //for(; ;){
		bzero(buff, MAX);

		// read the message from client and copy it in buffer
		read(sockfd, buff, sizeof(buff));
    printf("buff %s\n", buff);

    client.rollno = (char*)malloc(MAX*sizeof(char));
    // client.rollno = atoi(buff);
    client.rollno = buff;
    // print buffer which contains the client contents
    printf("client rollno: %s\n", client.rollno);

    // appending to the file
    fwrite( "client rollno:", sizeof(char), 15, fptr);
    count = fwrite( client.rollno,sizeof(char),4, fptr);
    printf("return value of fwrite is %d\n", count);
    count = fwrite( "\n", sizeof(char), 1, fptr);
    printf("return value of fwrite is %d\n", count);
		bzero(buff, MAX);

    // read the message from client and copy it in buffer
    read(sockfd, buff, sizeof(buff));
    printf("buff %s\n", buff);

    client.result = (char*)malloc(MAX*sizeof(char));
    // client.rollno = atoi(buff);
    client.result = buff;
    // print buffer which contains the client contents
    printf("client result: %s\n", client.result);

    // appending to the file
    fwrite( "client result:", sizeof(char), 15, fptr);
    count = fwrite( client.result,sizeof(char),4, fptr);
    printf("return value of fwrite is %d\n", count);
    count = fwrite( "\n", sizeof(char), 1, fptr);
    printf("return value of fwrite is %d\n", count);
    bzero(buff, MAX);
    //sleep(10);
  //}
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
