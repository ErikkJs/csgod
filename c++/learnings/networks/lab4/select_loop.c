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

    break;
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
