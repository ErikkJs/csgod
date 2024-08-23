
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>      /* for gethostname() */
#include <arpa/inet.h>   /* for IP address structures and functions */

#include <errno.h>
#include <sys/select.h>

#define SELECT_TIMEOUT   30
#define ULIMIT          255

int main(int argc, char *argv[], char *envp[])
{
  int  sock, msgsock;  /* Sockets are integer file descriptors on Linux */
  struct  sockaddr_in  name, caller;
  char  chrline[80];
  char  buf[2048];
  int  i, size, length, ret, k;

  /* The following variables are for the select() function */
  fd_set readmask, writemask, exceptmask;
  struct timeval timeout;  /* contains tv_sec and tv_usec member variables */

  /* Process the command line for the buffer size, if given */
  if(argc > 1)
  {
    size = atoi(argv[1]);
    /* Validate that the given argument is between 1 and sizeof(buf) - 1 
     * Set to the default size if argument is invalid */
    if(size < 1  ||  size > sizeof(buf) - 1) 
      size = sizeof(buf) - 1;
  }
  else
  {
    size = sizeof(buf) - 1;  /* Default size */
  }


  /* Create the listen socket. This is a TCP socket, so use SOCK_STREAM 
   * Exit if the socket cannot be created */
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock < 0) 
  {
    perror("receiver: socket() failed. ");
    return -1;
  }

  /* Bind the socket to an IP address and port. We will use the IP address
   * INADDR_ANY, which tells the system to assign the IP address, and the
   * port number 0, which tells the system to assign a random port number.
   *
   * First we have to set the fields in the sockaddr_in object "name" and then
   * we can call bind(). Again, exit if bind() fails. */
  name.sin_family = AF_INET;         /* TCP/IP family */
  name.sin_addr.s_addr = INADDR_ANY; /* INADDR_ANY = assigned by system */
  name.sin_port = htons(0);           /* 0 = assigned by system */
  ret = bind(sock,(struct sockaddr *)&name,sizeof name);
  if(ret < 0)
  {
    perror("receiver: bind() failed. ");
    return -1;
  }

  /* In order to use vcsend to send data to this program, we need to know
   * what port number the system just assigned this program. So this segment
   * calls getsockname() to update the sockaddr_in object "name" with the
   * system assigned values and then print that info to the screen. */
  length = sizeof name;
  ret = getsockname(sock, (struct sockaddr *)&name, (socklen_t *)&length);
  if(ret < 0)
  {
    perror("receiver: getsockname() failed. ");
    return -1;
  }

  sleep(1);  /* pause for clean screen display */
  printf("\nreceiver: process id: %d ", (int)getpid());
  printf("\nreceiver: IP address: %d.%d.%d.%d",
        (ntohl(name.sin_addr.s_addr) & 0xff000000) >> 24,
        (ntohl(name.sin_addr.s_addr) & 0x00ff0000) >> 16,
        (ntohl(name.sin_addr.s_addr) & 0x0000ff00) >>  8,
        (ntohl(name.sin_addr.s_addr) & 0x000000ff));
  printf("\nreceiver: port number: %hu", ntohs(name.sin_port));
  printf("\n");
  fflush(stdout);

  /* Now we will call listen() and wait for a client to connect. The
   * accept() function will block until there is a client or an error. */

  listen(sock,5);    /* up to 5 clients can connect. only 1st is accepted */
  k = sizeof caller;
  msgsock = accept(sock, (struct sockaddr *)&caller, (socklen_t *)&k);

  /* We only reach this point when there is an error or a client. We can 
   * check the value of msgsock (the data socket) to see which has happened */

  if(msgsock == -1)
  {
    perror("receiver: accept() failed. ");
  }
  else
  {
    printf("\nreceiver: Valid connection received.\n");
    printf("receiver: Sending handshake.\n");
    fflush(stdout);

    /* let vcsend know we are ready by sending a single character */
    buf[0]= '0';    
    send(msgsock, buf, 1, 0);

    printf("receiver: Waiting for client....\n");

    while(1)
    {
      printf("enter a line> ");
      fflush(stdout);
      bzero(buf, sizeof(buf)); /* Clear out the buffer */

      /* The first step to using select is to zero out all the file descriptors
       * in each mask */
      FD_ZERO(&readmask);
      FD_ZERO(&writemask);
      FD_ZERO(&exceptmask);

     FD_SET(0, &readmask);         /* activiate stdin read */
      FD_SET(0, &exceptmask);       /* activiate stdin exception */
      FD_SET(msgsock, &readmask);   /* watch socket for arriving data */
      FD_SET(msgsock, &exceptmask); /* watch socket for exceptions */

     timeout.tv_sec = SELECT_TIMEOUT;    /* seconds */
      timeout.tv_usec = 0;                /* microseconds */

     ret = select(ULIMIT, &readmask, &writemask, &exceptmask, &timeout);

      if(ret == 0)  /* timeout */
      {
        printf(" timeout!\n");
      }
      else if(ret < 0)  /* error or signal interupt */
      {
        if(errno == EINTR)
        {
          printf("Signal interupted select!\n");
        }
        else
        {
          printf("Exception on select(), exiting loop...\n");
          break;
        }
      }
      /* Check if there was an exception on stdin */
      else if(FD_ISSET(0, &exceptmask))
      {
        printf("Exception on stdin, exiting loop...\n");
        break;
      }
      /* Check if there was an exception on socket */
      else if(FD_ISSET(msgsock, &exceptmask))
      {
        printf("Exception on socket, exiting loop...\n");
        break;
      }
      /* Check if there's data on stdin waiting to be read */
      else if(FD_ISSET(0, &readmask))
      {
        fgets(chrline, sizeof(chrline) - 1, stdin);
        /* Check for the period to end the connection and also convert any
         * newlines into null characters */
        for(i = 0 ; i < (sizeof(chrline) - 1) ; i++)
        {
          if( (chrline[i] == '\n') || (chrline[i] == '\0') )
            break;
        }
        if(chrline[i] == '\n')
          chrline[i] = '\0';   /* get rid of newline */
  
        length = strlen(chrline);
        if(chrline[0] == '.')    /* end of stream */
        {
          printf("receiver: termination requested \n");
          break; /* Exit out of loop */
        }

        ret = send(msgsock, chrline, length + 1, 0);

        if(ret < 0)
        {
          perror("receiver: write() failed. ");
          break;  /* Exit out of loop */
        }
      }
      /* Check if there's data on the socket */
      else if(FD_ISSET(msgsock, &readmask))
      {
        /* recv() will block until the client sends information, the client
         * closes the connection or an error occurs on the data socket. */
        ret = recv(msgsock, buf, size, 0);
        if(ret < 0)
        {
          perror("receiver: recv() failed. ");
        }
        if(ret == 0)
        {
          printf("        received-->sender has ended connection \n");
          break;
        }
        else
        {
          printf("        received-->%s \n",buf);
        }
      }
      fflush(stdout);
    }
  }


  printf("receiver: ending session also and exiting. \n");
  close(msgsock);    /* close data socket */
  close(sock);      /* close listen socket */

  return 0;
}  /* end of main */
