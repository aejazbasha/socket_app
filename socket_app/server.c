#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>      // inet_addr
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>

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

  // To retrive client IP address
  client_ip = (char *)malloc(MAX*sizeof(char));
  inet_ntop(AF_INET, &(cli.sin_addr), client_ip, len);
}

// Function designed for chat between client and server.
void func(int sockfd)
{
  int ret;

  ret = read(sockfd, &client, sizeof(client));
  if ( -1==ret )
    perror("Read");

  fptr = fopen("database.txt", "a");
  if(NULL == fptr)
    perror("error in opening file");

  printf("client ip %s client rollno: %d result: %d\n", client_ip, client.rollno, client.result);

  // appending client IP to the file
  fprintf( fptr, "client ip %s\tclient rollno:%d\tclient result:%d\n", client_ip,client.rollno, client.result);

  fclose(fptr);
}

void initialize_socket()
{
  create_socket();
  bind_socket();
  listen_connection();
}

// Driver function
int main()
{
  initialize_socket();

  for(; ;)
  {
    accept_connection();
    func(connfd);
  }

//	func(connfd);

	// close the socket
	close(sockfd);
}
