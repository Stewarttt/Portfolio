/* 3000run.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 1<<16

int result_status;
int result_pid;
int child_exited = 0;

char *find_env(char *envvar, char *notfound, char *envp[])
{
        const int MAXPATTERN = 128;
        int i, p;
        char c;
        char pattern[MAXPATTERN];
        char *value = NULL;

        p = 0;
        while ((c = envvar[p])) {
                pattern[p] = c;
                p++;
                if (p == (MAXPATTERN - 2)) {
                        break;
                }
        }
        pattern[p] = '=';
        p++;
        pattern[p] = '\0';

        i = 0;
        while (envp[i] != NULL) {
                if (strncmp(pattern, envp[i], p) == 0) {
                        value = envp[i] + p;
                }
                i++;
        }

        if (value == NULL) {
                return notfound;
        } else {
                return value;
        }
}


/*Code provided from 3000shell by Anil, used for question 5*/
void find_binary(char *name, char *path, char *fn, int fn_size) {
        char *n, *p;
        int r, stat_return;

        struct stat file_status;

        if (name[0] == '.' || name[0] == '/') {
                strncpy(fn, name, fn_size);
                return;
        }

        p = path;
        while (*p != '\0') {

                r = 0;
                while (*p != '\0' && *p != ':' && r < fn_size - 1) {
                        fn[r] = *p;
                        r++;
                        p++;
                }

                fn[r] = '/';
                r++;

                n = name;
                while (*n != '\0' && r < fn_size) {
                        fn[r] = *n;
                        n++;
                        r++;
                }
                fn[r] = '\0';


                stat_return = stat(fn, &file_status);

                if (stat_return == 0) {
                        return;
                }

                if (*p != '\0') {
                        p++;
                }
        }
}

void signal_handler(int the_signal)
{
        if (the_signal == SIGCHLD) {
                result_pid = wait(&result_status);
		if (result_pid == -1) {
                	/* nothing to wait for */
                	return;
        	}
		child_exited = 1;
                return;
        }
}

int parent(pid_t child_pid, char *command, char *input_fn, char *output_fn)
{

        int count = 0;
        const int bufsize = 256;
        char buf[bufsize];

	//replace the call to wait in parent with a sleep for three seconds
        sleep(3); //in sleep each digit represents a second by default
	//wait(&result_status); comment this out

	if (!child_exited) {
		kill(child_pid, SIGTERM);
		sleep(3); //sleep for another three seconds
	}

        if (result_pid == child_pid) {
                count += snprintf(buf + count, bufsize,
                                  "Child %d terminated ", child_pid);
                if (WIFEXITED(result_status)) {
                        count += snprintf(buf + count, bufsize,
                                          "normally with status %d\n",
                                          WEXITSTATUS(result_status));
                }
		else if (WIFSIGNALED(result_status)) { //check if the child terminated with a status
			//find out what signal was present when the child terminated
			count += snprintf(buf + count, bufsize, " with signal of signal number %d", WTERMSIG(result_status));
		}
		else {
                        count += snprintf(buf + count, bufsize,"\n");
                }
                puts(buf);

                return 0;
        } else if (result_pid == -1) {
                printf("No child to wait for.\n");
                return -1;
        } else {
                printf("wait returned unknown value %d\n", result_pid);
                return result_pid;
        }
}

int child(char *command, char *input_fn, char *output_fn, char * args[], int numArgs)
{
	child_exited = 0;
        extern char **environ;
        char *argv[255];
        int fd;
	int inputFile;

        argv[0] = command;

	for (int i = 1; i < numArgs+1; i++) {
		argv[i] = args[i+3];
	}

        argv[numArgs+1] = NULL;

	/*
	for (int i = 0; i < 3; i++) {
		printf("%s\n", environ[i]);
	}
	*/

        close(1);
        fd = creat(output_fn, 0644);
	inputFile = open(input_fn, O_RDONLY); //open the input file to be read
        if (fd == -1) {
                fprintf(stderr, "Could not create %s for output.\n",
                        output_fn);
                exit(-3);
        }
        dup2(fd, 1);
	dup2(inputFile, 0); //set the input file to be the standard input

        execve(command, argv, environ);
        fprintf(stderr, "execve of %s failed.\n", command);
        return(-1);
}

int main(int argc, char *argv[], char *envp[])
{
	struct sigaction signal_handler_struct;

        char commandBuffer[BUFFER_SIZE];
        char *input_fn;
        char *output_fn;
	char *command;
	char * path;

        pid_t pid;

	/* Here we observe how envp and environ both point to the same source
	extern char **environ;
	for (int i = 0; i < 3; i++) {
		printf("%s\n", envp[i]);
		printf("%s\n", environ[i]);
	}
	printf("\n");
	envp[1] = "";
	for (int i = 0; i < 3; i++) {
		printf("%s\n", envp[i]);
		printf("%s\n", environ[i]);
	}
	printf("\n");
	*/

	memset (&signal_handler_struct, 0, sizeof(signal_handler_struct));
        signal_handler_struct.sa_handler = signal_handler;
        signal_handler_struct.sa_flags = SA_RESTART;

	if (sigaction(SIGCHLD, &signal_handler_struct, NULL)) {
                fprintf(stderr, "Couldn't register SIGCHLD handler.\n");
        }

        if (sigaction(SIGHUP, &signal_handler_struct, NULL)) {
                fprintf(stderr, "Couldn't register SIGHUP handler.\n");
        }

	if (argc < 4) {
                fprintf(stderr,
                        "Usage: %s <command> <input file> <output file>\n",
                        argv[0]);
                exit(-1);
        }

	char *default_path = "/usr/bin:/bin";
        command = argv[1];
        input_fn = argv[2];
        output_fn = argv[3];

	path = find_env("PATH", default_path, envp);
	find_binary(command, path, commandBuffer, BUFFER_SIZE);

        pid = fork();

        if (pid == -1) {
                fprintf(stderr, "Fork failed.\n");
                exit(-2);
        } else if (pid == 0) {
                return child(commandBuffer, input_fn, output_fn, argv, argc-4);
        } else {
                return parent(pid, commandBuffer, input_fn, output_fn);
        }
}
