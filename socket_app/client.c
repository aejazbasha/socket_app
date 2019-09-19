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
};

int sockfd;
struct sockaddr_in servaddr;

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

static void fill_structure()
{
  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servaddr.sin_port = htons(PORT);
}

static void connect_server()
{
  // connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");
}

void start_socket()
{
  create_socket();
  fill_structure();
  connect_server();
}

void upload_result(int sockfd, struct data *d)
{
	int ret;
  int length;

  ret = write(sockfd, d, sizeof(struct data));
  if ( -1==ret )
    perror("Write");
    
}

int upload(struct data *d)
{
  start_socket();

	//printf("In client rollno:%d result:%d\n", d->rollno, d->result);
  upload_result(sockfd, d);

  // close the socket
	close(sockfd);
}
