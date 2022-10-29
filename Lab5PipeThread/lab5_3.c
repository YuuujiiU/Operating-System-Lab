#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/wait.h>
#include <math.h>

// check the format of the argument, only digit char allowed
int checkDigit(char *str)
{
	for (int i = 0; i < strlen(str); i++)
	{
		if (!isdigit(str[i]))
			return 0;
	}
	return 1;
}

// a recursive function that returns the factorial of a given number (n!)
long int factorial(int n)
{
	if (n == 0)
		return 1;
	return n * factorial(n - 1);
}

int main(int argc, char *argv[])
{
	int n;

	// validate the input parameter n
	if (argc != 2)
	{
		printf("usage: ./lab5_3 <n> \n");
		return -1;
	}

	// validate and obtain the input
	if (!checkDigit(argv[1]))
	{
		printf("integer parameters only!\n");
		return -1;
	}
	n = atoi(argv[1]);

	// create the ordinary pipes
	int fd[2];

	if (pipe(fd) == -1)
	{
		perror("Pipe failed!");
	}

	pid_t pid = fork();
	if (pid < 0)
	{
		printf("Fork failed!");
		return -1;
	}
	else if (pid == 0)
	{
		int c = 0, interval;
		long int val;

		// disable reading
		close(fd[0]);

		// set the seed
		srand((unsigned)time(NULL));

		while (c < n)
		{
			val = factorial(c);

			// write to the pipe
			write(fd[1], &val, sizeof(long int));

			// wait for a random interval of time (0 to 2.999 seconds)
			interval = rand() % 3000;
			usleep(interval * 1000);

			c++;
		}

		// disable writing
		close(fd[1]);
	}
	else
	{
		int c = 0;
		long int val;

		// disable writing
		close(fd[1]);

		while (c < n)
		{
			c++;

			// read from the pipe
			read(fd[0], &val, sizeof(long int));
			printf("%ld ", val);
			fflush(stdout);
		}

		printf("\n");
		fflush(stdout);

		// wait the child to finish
		wait(NULL);

		// disable reading
		close(fd[0]);
	}

	return 0;
}
