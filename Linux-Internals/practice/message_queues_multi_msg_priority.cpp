#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <mqueue.h>

int main()
{
    const char* MQ_NAME { "/mq_file" };
    const int MQ_MSGSIZE {256};
    const int MQ_MAXMSG {10};

    struct mq_attr attr{};
    attr.mq_curmsgs = 0;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MQ_MAXMSG;
    attr.mq_msgsize = MQ_MSGSIZE;

    mqd_t mq_d { mq_open(MQ_NAME, O_CREAT | O_EXCL | O_RDWR, 0666, &attr) };
    if(mq_d == -1)
    {
        perror("mq_open error");
        return -1;
    }

    const char* msgs[] {
        "low priority task",
        "medium priority task",
        "High priority task",
        "another low priority"
    };

    unsigned int priority[] {1, 5, 10, 2};

    for(int i{}; i < std::size(msgs); i++)
    {
        ssize_t cw { mq_send(mq_d, msgs[i], std::strlen(msgs[i])+1, priority[i]) };
        if(cw == -1)
        {
            perror("mq_send error");
            return -1;
        }
    }

    char buf[MQ_MSGSIZE];
    for(int i{}; i < std::size(msgs); i++)
    {
        unsigned int priority{};
        std::memset(buf, 0, std::size(buf));
        ssize_t cr { mq_receive(mq_d, buf, std::size(buf), &priority) };
        if(cr >= 0)
        {
            if(cr < std::size(buf))
                buf[cr] = 0;
            else
                buf[std::size(buf)-1] = 0;

            std::cout << "msg : " << buf << " with priority : " << priority << "\n";
        }
    }

    mq_close(mq_d);
    mq_unlink(MQ_NAME);

    return 0;
}
