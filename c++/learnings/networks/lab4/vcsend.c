/* This code is derived from the CMPS 376 networking code by Marc Thomas
 *
 * The purpose of this code is to create a very simple Internet client which
 * reads data from the keyboard and passes it to vcrec over a socket. 
 *
 * Command line usage:
 *   $ vcsend <hostname> <portnumber>
 *
 *   hostname must be the name or IP address of vcrec, e.g. sleipnir
 *   portnumber must be the port than vcrec printed out
 *
 * TCP/IP socket functions used by this program:
 * 
 *   socket()        Create a byte stream for sending/receiving data
 *   connect()       Send a connection request to the server
 *   send()          Send data to the server
 *   recv()          Receive data from the server
 *   close()         Close the socket
 *
 * Connection process:
 *
 * vcsend                      vcrec
 * -------------------------   ------------------------- 
 *  
 * socket() (client socket)    socket() (listen socket)
 *                             bind()
 *                             getsockname()
 *                             listen()
 * connect() --------------->  accept() (creates 2nd data socket)
 *
 * (data exchange phase can be response-driven but if more
 *  than one descriptor must be managed use of select() is
 *  strongly recommended)
 * 
 * send() --------------------> recv()
 * recv() <-------------------- send()
 * 
 * close() (client socket)     close() (both data and listen socket)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>      /* for gethostname() */
#include <arpa/inet.h>   /* for IP address structures and functions */
#include <netdb.h>       /* for gethostbyname() */

#include <errno.h>
#include <sys/select.h>

#define SELECT_TIMEOUT   30
#define ULIMIT          255

int main(int argc, char *argv[], char *envp[])
{
  int  msgsock;  /* The data socket for the client */
  struct  sockaddr_in  name;
  struct  hostent  *hp;  /* Used to resolve hostname to IP address */
  char  chrline[80];
  char  buf[2048];
  int  size, i, ret, length;

  /* The following variables are for the select() function */
  fd_set readmask, writemask, exceptmask;
  struct timeval timeout;  /* contains tv_sec and tv_usec member variables */

  /* Make sure to set size to the number of characters in buf */
  size = sizeof(buf) - 1;

  /* We must give the hostname and portnumber on the command line. This
   * checks that the user has typed both. */
  if(argc < 3)
  {
    printf("error: Invoke this program via: \n");
    printf("       vcsend <hostname> <portnumber> \n");
    return -1;
  }

  /* Since we are allowing a domain name as the hostname, we must call
   * gethostbyname() to convert the hostname into an IP address. */
  hp = gethostbyname(argv[1]);
  if(hp == NULL)
  {
    printf("sender: gethostbyname() failed.\n");
    printf("        Are you sure the hostname is correct? \n");
    return -1;
  }
  printf("\nsender: hostname has been resolved.\n");

  /* Copy the resolved IP address over into the name structure along with 
   * the portnumber given on the command line */
  memcpy((char *)&name.sin_addr, (char *)hp->h_addr, hp->h_length);
  name.sin_family = AF_INET;
  name.sin_port = htons((short)atoi(argv[2]));  

  /* Allocate the data socket and attempt to connect to vcrec. Exit the 
   * program if either socket() or connect() fails. */
  printf("sender: Attempting to connect to server.\n");
  msgsock = socket(AF_INET, SOCK_STREAM, 0);
  if(msgsock < 0)
  {
    perror("sender: socket() failed. ");
    return -1;
  }

  ret = connect(msgsock, (struct sockaddr *)&name, sizeof name);
  if(ret != 0)
  {
    perror("sender: connect() failed. ");
    fprintf(stderr, 
    "\n        are you sure that the portnumber is correct? \n");
    fflush(stderr);
    return -1;
  }

  /* Wait for the handshake from vcrec before continuing */
  printf("sender: Waiting for handshake from vcrec...\n");
  sleep(1);  /* pause for clean screen display */
  fflush(stdout);
  recv(msgsock, chrline, sizeof(chrline) - 1, 0); /* Handshake character */
  printf("sender: Handshake received. Entering data loop.\n");

  printf("\nEnter a line of characters at the prompt.\n");
  printf("To exit the program, type just a period and hit enter.\n");

  /* Go into an infinite loop. We'll use the "break" statement to exit
   * the loop if the user types a period or send() fails */
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

    /* The second step is to set all the file descriptors you wish to watch
     * for each mask. Recall that the file descriptor for stdin is 0 and the
     * file descriptor for stdout is 1. For a socket, the file descriptor is
     * the integer returned by the socket() function */
    FD_SET(0, &readmask);         /* activiate stdin read */
    FD_SET(0, &exceptmask);       /* activiate stdin exception */
    FD_SET(msgsock, &readmask);   /* watch socket for arriving data */
    FD_SET(msgsock, &exceptmask); /* watch socket for exceptions */

    /* The third step is to set the timeout values. This is how long select()
     * will wait for activity before giving up and returning */
    timeout.tv_sec = SELECT_TIMEOUT;    /* seconds */
    timeout.tv_usec = 0;                /* microseconds */

    /* Now we are ready to call select(). We will wait here until something
     * happens or the function times out */
    ret = select(ULIMIT, &readmask, &writemask, &exceptmask, &timeout);

    /* Now process the return value from timeout to see what happened */
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
        printf("sender: termination requested \n");
        break; /* Exit out of loop */
      }

      ret = send(msgsock, chrline, length + 1, 0);

      if(ret < 0)
      {
        perror("sender: write() failed. ");
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
        perror("sender: recv() failed. ");
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
  /* Closing the data socket from vcsend will trigger recv() on vcrec to
   * return 0, which will also cause vcrec to close its sockets and exit */
  close(msgsock);

  return 0;
}  /* end of main */

