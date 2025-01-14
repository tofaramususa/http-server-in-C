#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef struct s_client 
{
	int id; //fd of the client
	char *message; //message of the client
} t_client;

char receiver_buffer[120000];
char send_buffer[120000];
fd_set current_set, write_set, read_set;
//Then there is FD_CLR, FD_ISSET, FD_ZERO, FD_SET
int max_fds = 0;
int tracker = 0; 
t_client clients[2040];

//function to send messages to all clients
//function to send error message
void fatal_error(char *message)
{
	if(message)
		write(2, message, strlen(message));
	else
	{
		write(2, "Fatal Error\n", 12);
	}
	exit(1);
}

void send_to_all(int sender)
{
	for(int fd = 0; fd <= max_fds; fd++)
	{
		if(FD_ISSET(fd, &write_set) && fd!= sender)
		{
			if(send(fd, send_buffer, strlen(send_buffer), 0) == -1)
				fatal_error(NULL);
		}
	}
}

int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int	i;

	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
				return (-1);
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}

char *str_join(char *buf, char *add)
{
	char	*newbuf;
	int		len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}


int main(int ac, char **av) {
	int sockfd, connfd;
	socklen_t len;
	struct sockaddr_in servaddr; 
	struct sockaddr_in cli; 

	if(ac != 2)
	{
		fatal_error("Wrong number of arguments\n");
		exit(1);
	}
	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		fatal_error(NULL);
	}
	max_fds = sockfd; //add this line
	FD_ZERO(&current_set); //added this line -- use FD_ZERO to edit the 
	FD_SET(sockfd, &current_set); //added this line
	bzero(&servaddr, sizeof(servaddr)); 
	bzero(&clients, sizeof(clients)); //added this line

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(atoi(av[1])); //editted this line to take input

  
	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) { 
			fatal_error(NULL); //changed the error message
	} 
	if (listen(sockfd, 100) != 0) {
		fatal_error(NULL); //changed the error message
	}
	//put the while loop here
	while(1)
	{
		read_set = write_set = current_set; //this sets the read and write sets
		if(select(max_fds + 1, &read_set, &write_set, 0, 0) == -1) continue; //this gets those ready to read and write takes max_fds, read and write sets and 0 then -1
		for(int fd = 0; fd <= max_fds; fd++) //loop through the fds
		{
			if(!FD_ISSET(fd, &read_set)) //look for the one that is set to read if not then ignore it
				continue;
			if(fd == sockfd) //if its the sockfd
			{
				len = sizeof(cli);
				connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
				if(connfd == -1) continue;
				if(connfd > max_fds) max_fds = connfd; //increase max fds
				clients[connfd].id = tracker++; //set the client id
				FD_SET(connfd, &current_set); //add fd to the current set
				sprintf(send_buffer, "server: client %d just arrived\n", clients[connfd].id); //save message to a buffer
				send_to_all(connfd); //send message to everyone
				break;
			}
			else
			{
				int receive_bytes = recv(fd, receiver_buffer, sizeof(receiver_buffer), 0); //this sets message to receiver
				if(receive_bytes <= 0) //this means there is no connection anymore so remove and notify everyone
				{
					sprintf(send_buffer, "server: client %d just left/n", clients[fd].id); //create message
					send_to_all(fd); //send to all
					FD_CLR(fd, &current_set); //remove from fd set
					close(fd); //close the fd connection
					break;
				}
				else {
					char *temp_message; //temporary message store

					receiver_buffer[receive_bytes] = '\0'; //put an end to receiving string
					clients[fd].message = str_join(clients[fd].message, receiver_buffer); //join the client message with receiving message
					while(extract_message(&clients[fd].message, &temp_message)) //extract messages and send one by one
					{
						sprintf(send_buffer, "client %d: %s\n", clients[fd].id, temp_message); //add to sender buffer
						send_to_all(fd); //send to all
						free(temp_message); //free the message buffer
					}
				}
			}
		}
	}
}

// Notes:
// Functions l will actually be using - WRITE, ClOSE, SELECT, SOCKET, ACCEPT(just refactor whats been given),
// Continued -- SEND, RECV, FREE, SPRINTF, STRLEN(for the send function needs length), EXIT, 


//THE FUNCTIONS I NEED TO IMPLEMENT
// write error message with stderr and write fatal error with exit status 1 
// 1.fatal_error function
// 2. broadcast message function to send messages to the clients
// 3. register function to add a new client to the list
// 4. remove client to remove client from the list
// 5. Send function that uses broadcast message and notifies everyone else
// 6. Create socket function that returns fatal error in case of error 

//THE FD MACROS l need to use:
// FD_SET
// FD_CLR
// FD_ZERO
// FD_ISSET
// 