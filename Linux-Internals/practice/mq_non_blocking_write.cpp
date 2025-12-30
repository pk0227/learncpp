#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <mqueue.h>

std::ostream& operator<<(std::ostream& out, struct mq_attr &at)
{
    out << "curmsgs : " << at.mq_curmsgs << "\n";
    out << "maxmsg : " << at.mq_maxmsg << "\n";
    out << "msgsize : " << at.mq_msgsize << "\n";
    out << "flags : " << (at.mq_flags & O_NONBLOCK ? "O_NONBLOCK" : "Blocking") << "\n";
    out << "flags : " << at.mq_flags << "\n";

    return out;
}

int main()
{
    const char* MQ_NAME { "/mq_non_block" };
    const int MQ_MSGSIZE{256};
    const char MQ_MAXMSG{5};

    mq_unlink(MQ_NAME);

    struct mq_attr attr{};
    attr.mq_flags = 0;
    attr.mq_curmsgs = 0;
    attr.mq_maxmsg = MQ_MAXMSG;
    attr.mq_msgsize = MQ_MSGSIZE;

    mqd_t mq_d { mq_open(MQ_NAME, O_CREAT | O_EXCL | O_WRONLY | O_NONBLOCK, 0666, &attr) };
    //mqd_t mq_d { mq_open(MQ_NAME, O_CREAT | O_EXCL | O_RDWR | O_NONBLOCK, 0666, &attr) };
    //mqd_t mq_d { mq_open(MQ_NAME, O_CREAT | O_EXCL | O_RDWR, 0666, &attr) };
    
    if(mq_d == -1)
    {
        perror("mq_open error");
        return -1;
    }

    struct mq_attr at{};
    mq_getattr(mq_d, &at);
    std::cout << "current attr\n===============================================\n";
    std::cout << at << "\n";

    const char* msg[] {
        "msg 1",
        "msg 2",
        "msg 3",
        "msg 4",
        "msg 5"
    };

    unsigned int priority[]{5, 85, 32, 75, 9};
    
    for(int i{}; i < std::size(msg); i++)
    {
        ssize_t cw { mq_send(mq_d, msg[i], std::strlen(msg[i])+1, priority[1]) };
        if(cw == -1)
        {
            perror("mq_send error");
        }
    }

    const char* extra_msg { "extra message" };
    ssize_t cw { mq_send(mq_d, extra_msg, std::strlen(extra_msg)+1, 100) };
    if(cw == -1)
    {
        perror("mq_send extra msg error");
    }

    mq_close(mq_d);
    mq_unlink(MQ_NAME);

    return 0;
}
