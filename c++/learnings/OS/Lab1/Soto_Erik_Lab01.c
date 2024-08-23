/*  cmps 3600
 *  Erik Soto
 *  lab 01
 *  compile and run with strace:
 *   
 *  $ gcc -o fork1 fork1.c
 *  $ strace -c -f -o log ./fork1   # counts calls, follows child, redirects
 *                                  # stderr to log 
 */

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static int nsecs = 2;

int main() {

    /* duplicate yourself - fork returns PID of the child to the parent 
     *and 0 to the child or -1 to the parent on failure */

    pid_t cpid = fork();  /* clone a new process */
    pid_t ppid = getppid();      /* Will get the ID or parent process*/

    switch(cpid){
    case -1:
    { 
        printf("ERROR\n");
        exit(0);
    }
    case 0:
    {
        printf("Child ID: %d\n", cpid);
        alarm(30); /* set alarm to kill yourself in 10 seconds */
        pause();      /* suspend yourself until SIGALRM signal comes in */
        printf("Ms keisa ms keisha, omg she dead\n");
        exit(0);      /* exit code 0 means success */ 

    }
    default:
    {

        /* parent waits for child to die; harvests child's exit code */
        ppid = waitpid(ppid, &nsecs, 0);
        printf("PARENT PROCESS BEGAN WITH ID %d\n", ppid);
        ppid = waitpid(ppid, &nsecs, 0);
       


        alarm(1); /* pause to view child termination in top */ 
        
        printf("PARENT PROCESS ENDING\n"); 
        exit (EXIT_SUCCESS);
    }
    }
    return (0);
}

