#include <cstdio>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>

const char* MQ_NAME {"/mq_1"};
const int MQ_MSGSIZE {256};
const int MQ_MAXMSG {10};

std::ostream& operator<<(std::ostream& out, struct mq_attr &at)
{
    out << "mq_flags : " << at.mq_flags << "\n";
    out << "mq_curmsgs : " << at.mq_curmsgs << "\n";
    out << "mq_msgsize : " << at.mq_msgsize << "\n";
    out << "mq_maxmsg : " << at.mq_maxmsg << "\n";

    return out;
}

int main()
{
    mq_unlink(MQ_NAME);

    struct mq_attr attr{};
    attr.mq_flags = 0;
    attr.mq_msgsize = MQ_MSGSIZE;
    attr.mq_maxmsg = MQ_MAXMSG;
    attr.mq_curmsgs = 0;

    mqd_t mq_d { mq_open(MQ_NAME, O_CREAT | O_EXCL, 0666, &attr) };
    if(mq_d == -1)
    {
        perror("mq_open error main");
        return -1;
    }
    
    std::cout << "current attrs\n=================================================\n";
    struct mq_attr at{};
    mq_getattr(mq_d, &at);
    std::cout << at << "\n";

    pid_t pid { fork() };
    if(pid == 0)  //child
    {
        mqd_t mq_rd { mq_open(MQ_NAME, O_RDONLY) };
        if(mq_rd == -1)
        {
            perror("error mq_rd in child");
            exit(1);
        }

        char buf[MQ_MSGSIZE]{};
        unsigned int priority{};

        ssize_t cr { mq_receive(mq_rd, buf, std::size(buf), &priority) };
        if(cr >= 0)
        {
            if(cr < std::size(buf))
                buf[cr] = 0;
            else
             buf[std::size(buf)-1] = 0;

            std::cout << "msg received : " << buf << " with priority : " << priority << "\n";
        }
        else 
        {
            perror("mq_receive error");
        }

        mq_close(mq_rd);
        exit(0);
    }
    else  //parent 
    {
        mqd_t mq_wd { mq_open(MQ_NAME, O_WRONLY) };
        if(mq_wd == -1)
        {
            perror("mq_open error parent");
            return -1;
        }

        const char* msg {"msg from parent"};

        ssize_t cw {mq_send(mq_wd, msg, std::strlen(msg)+1, 123)};
        if(cw == -1)
        {
            perror("error cw in parent");
        }

        close(mq_wd);
        wait(nullptr);
    }

    close(mq_d);
    mq_unlink(MQ_NAME);
    std::cout << "message queue has been removed\n";

    return 0;
}
