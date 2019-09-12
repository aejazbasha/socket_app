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

// void rollno(int sockfd)
// {
// 	char buff[MAX];
// 	int n,i;
// 	//for (i=0;i<5;i++) {
// 	//for(; ;){
// 		bzero(buff, sizeof(buff));
// 		printf("Enter your rollno:\n");
// 		n = 0;
// 		while ((buff[n++] = getchar()) != '\n')
// 			;
// 		write(sockfd, buff, sizeof(buff));
// 		bzero(buff, sizeof(buff));
// 	//}
// }

void upload_rollno(int sockfd, int rollno)
{
	int ret;
	char *temp;
	int length;

	//printf("result is: %d\n", result);

	length = snprintf( NULL, 0, "%d", rollno);
	temp = malloc(length+1);
	snprintf(temp, length+1, "%d", rollno);

	//itoa(result, temp, 10);
	//sprintf(temp, result, 42);
	ret = write(sockfd, temp, sizeof(temp));
	//printf("return of write: %d\n", ret);
	free(temp);
}

void upload_result(int sockfd, int result)
{
	int ret;
	char *temp;
	int length;

	//printf("result is: %d\n", result);

	length = snprintf( NULL, 0, "%d", result);
	temp = malloc(length+1);
	snprintf(temp, length+1, "%d", result);

	//itoa(result, temp, 10);
	//sprintf(temp, result, 42);
	ret = write(sockfd, temp, sizeof(temp));
	//printf("return of write: %d\n", ret);
	free(temp);
}

int upload(int rollno, int result)
{
  start_socket();

	// function for chat
	upload_rollno(sockfd, rollno);
	upload_result(sockfd, result);
	// close the socket
	close(sockfd);
}
