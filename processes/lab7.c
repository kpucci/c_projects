// Katie Pucci, KEV19

#define _GNU_SOURCE
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		printf("uh, you gotta gimme an executable to run...\n");
		return 0;
	}

	if(fork() == 0)
	{
		execvp(argv[1], &argv[1]);

        perror("fork() error");
        exit(1);
	}
	else
	{
        signal(SIGINT,SIG_IGN);

		// This waits for a child to exit in some way. *How* it exited
		// will be put in the status variable.
		int status;
		int childpid = waitpid(-1, &status, 0);
		printf("----------\n");

        if(childpid == -1)
            perror("wait() error");
        else if(WIFEXITED(status))
            if(WEXITSTATUS(status) == 0)
                printf("Program exited normally\n");
            else
                printf("Exited with code %d\n", WEXITSTATUS(status));
        else if(WIFSIGNALED(status))
            printf("Program was terminated due to signal: %d\n", WTERMSIG(status));
        else if(WIFSTOPPED(status))
            printf("Exited due to stop signal %d\n", WSTOPSIG(status));
        else
            printf("Terminated some other way\n");
	}

	return 0;
}
