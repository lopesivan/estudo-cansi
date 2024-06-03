#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

void main()
{
    // fd = file decriptor
    // handle to opened file
    //+------+-------+
    //| read | write |
    //|  fd  |   fd  |
    //+------+-------+
    int pipefds[2];

    char buffer[256] = {0};

    printf ("parent pid: %d\n", getpid());
    pipe (pipefds);
    write (pipefds[1], "msg", 3);
    if (fork() > 0)
    {
        return;
    }

    read (pipefds[0], buffer, 3);
    printf ("got from pipe: %s\n", buffer);
    printf ("child pid: %d\n", getpid());

}
