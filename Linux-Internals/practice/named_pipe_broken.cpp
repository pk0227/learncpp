#include <chrono>
#include <csignal>
#include <cstring>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    const char* fifo_name { "/tmp/fifo" };
    unlink(fifo_name);

    if(mkfifo(fifo_name, 0666) == -1)
    {
        perror("fifo creation failed");
        return -1;
    }
    signal(SIGPIPE, SIG_IGN);

    pid_t pid { fork() };

    if(pid == 0)  //child
    {
  
        int rd { open(fifo_name, O_RDONLY | O_NONBLOCK) };
        if(rd == -1)
        {
            perror("rd open failed");
            return -1;
        }
        
/*
        char buf[256]{};
        ssize_t cr { read(rd, buf, std::size(buf)) };
        if(cr == -1)
        {
            perror("read failed");
            return -1;
        }
*/
    
        close(rd);
        exit(0);
    }

    int wd { open(fifo_name, O_WRONLY) };
    if(wd == -1)
    {
        perror("wd open failed");
        return -1;
    }

    const char* msg {"pipe is working."};
    ssize_t cw { write(wd, msg, std::strlen(msg)+1) };
    if(cw == -1)
    {
        perror("write failed");
    }
    close(wd);

    unlink(fifo_name);
    wait(nullptr);

    return 0;
}
