variables:

we got the clients struct of 2040 with char *message and the id too
we got the receiver_buffer and sender_buffer of 120,000 array
we got the fd_set current_set, read_set, write_set
we got the max fds and also the tracker, both ints initialised to


function fatal_error
print error message and return exit one


send to all function:
-- takes the fd of sender
-- loops through all the fds and checks if fd is set in the write_set and that it is not the sender
-- then sends the message -- parameters are the fd, the send_buffer and the length and 0, if -1 then return fatal error


//int main(int ac, char **av)
{
	if(ac != 2)
	{
 		fatal_error("Wrong number of arguments\n") 
		exit(1)
	}

	then also need to set max_fd to the sock_fd
	then initialise the current_set with fd_zero -- takes current_set
	then fd_set the sockfd to the currentset
	then do bzero for clients using clients and sizeof(clients)

	then change the sin_port to take av[1]

	then change the listen to take 100 not just 10

	then the while loop starts
	while(1)
	{
		set read_set, write_set to current_set
		//then we use select now to get write and read sets that are ready
		select takes max_fds plus one, also takes read set and write set and 0,0 check if -1 then continue
		this is when we enter the for loop into all the fds
		check if FD_ISSET(fd, &read_set)
		if(fd == sockfd)
		{
			//then increase the max_fds
			//clients[connfd].id = tracker++
			//then FD_SET and add connfd 
			//then use sprintf takes --> send_buffer, clients[connfd]
			//send to all and break
		}
		else
		{
			int receive_bytes = recv(fd, receiver_buffer, sizeof(receiver_buffer), 0)
			if(receiver_bytes <= 0)
			{
				add to buffer that client has left and also send to all
				clear the FD_CLR
				close the connection and break
			}
			else
			{
				char *temp_messsage

				receiver_buffer[receive_bytes] = '\0'
				clients[fd].message = str_join(clients[fd].message, receiver_buffer)
				while(extract_message(&clients[fd].message, &temp_message))
				{
					add the message to sprintf
					sprintf(send_buffer,  "client %d %s\n", clients[fd].id, temp_message)
					send to all the guys
					free the temp message
				}
			}

		}
	}
}

