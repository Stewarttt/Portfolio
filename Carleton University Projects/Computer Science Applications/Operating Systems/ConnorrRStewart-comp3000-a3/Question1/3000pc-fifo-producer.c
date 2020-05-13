/* 3000pc-fifo.c  Simple producer-consumer with a fifo
 * Original Version Copyright (C) 2017  Anil Somayaji
 * Modified Version Copyright (C) 2020  William Findlay
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>. */

/* You really shouldn't be incorporating parts of this in any other code,
   it is meant for teaching, not production */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define QUEUESIZE 32
#define WORDSIZE 16

const int wordlist_size = 27;
const char *wordlist[] = {
        "Alpha",
        "Bravo",
        "Charlie",
        "Delta",
        "Echo",
        "Foxtrot",
        "Golf",
        "Hotel",
        "India",
        "Juliet",
        "Kilo",
        "Lima",
        "Mike",
        "November",
        "Oscar",
        "Papa",
        "Quebec",
        "Romeo",
        "Sierra",
        "Tango",
        "Uniform",
        "Victor",
        "Whiskey",
        "X-ray",
        "Yankee",
        "Zulu",
        "Dash"
};

void report_error(char *error)
{
        fprintf(stderr, "Error: %s\n", error);
}

void usage_exit(char *progname)
{
        fprintf(stderr,
                "Usage: %s <event count> <prod interval int> <con interval int>\n",
                progname);
        exit(-1);
}

void pick_word(char *word)
{
	int pick;

        pick = random() % wordlist_size;

        strcpy(word, wordlist[pick]);
}

int queue_word(char *word, int pipefd_write)
{
        if (write(pipefd_write, word, WORDSIZE) == -1)
        {
                fprintf(stderr, "Error: Unable to write to pipe: %s\n", strerror(errno));
                return -1;
        }

        return 0;
}

void producer(int event_count, int pipefd_write, int prod_interval)
{
        char word[WORDSIZE];
        int i;

        for (i=0; i < event_count; i++)
        {
                pick_word(word);
                queue_word(word, pipefd_write);

                /* Don't sleep if interval <= 0 */
                if (prod_interval <= 0)
                        continue;
                /* Sleep if we hit our interval */
                if (prod_interval > 0 && i % prod_interval == 0)
                {
                        fprintf(stderr, "Producer sleeping for 1 second...\n");
                        sleep(1);
                }
        }

        close(pipefd_write);
        fprintf(stderr, "Producer finished.\n");
        exit(0);
}

int main(int argc, char *argv[])
{
        int count, interval;
        int pipefd;

        srandom(time(NULL));

        if (argc < 4)
        {
                if (argc < 1)
                {
                        report_error("no command line");
                        usage_exit(argv[0]);
                }
                else
                {
                        report_error("Not enough arguments");
                        usage_exit(argv[0]);
                }
        }

	pipefd = open(argv[3], O_WRONLY);

        count = atoi(argv[1]);
        interval = atoi(argv[2]);

	producer(count, pipefd, interval);
        return -1;
}

