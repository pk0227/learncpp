#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
    const char* fifo_name { "fifo_name" };

    unlink(fifo_name);

    if(mkfifo(fifo_name, 0666) == -1)
    {
        perror("error creating mkfifo");
        return -1;
    }

    pid_t pid { fork() };

    if(pid == 0)   // child
    {
        int wd { open(fifo_name, O_WRONLY) };
        if(wd == -1)
        {
            perror("opening fifo failed in child");
            exit(1);
        }

        dup2(wd, STDOUT_FILENO);
        close(wd);

        execlp(argv[1], argv[1], "-l", nullptr);
        perror("execlp ls failed");
        exit(1);
    }
    else //parent
    {
        int rd { open(fifo_name, O_RDONLY) };
        if(rd == -1)
        {
            perror("opening fifo failed in parent");
            goto wait_for_child;
        }

        dup2(rd, STDIN_FILENO);
        close(rd);

        execlp(argv[2], argv[2], "-l", nullptr);
        perror("execlp wc failed");

wait_for_child:
        //std::cout << "Waiting for child to join\n";
        //waitpid(pid, nullptr, 0);
    }

    //std::cout << "Waiting for child to join\n";
    //waitpid(pid, nullptr, 0);
    unlink(fifo_name);
    return 0;
}

