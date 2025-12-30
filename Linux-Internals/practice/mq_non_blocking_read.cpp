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

    mqd_t mq_d { mq_open(MQ_NAME, O_CREAT | O_EXCL | O_RDONLY | O_NONBLOCK, 0666, &attr) };
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

    char buf[256]{};
    unsigned int priority{};

    ssize_t cr { mq_receive(mq_d, buf, std::size(buf), &priority) };
    if(cr == -1)
    {
        perror("mq_receive error");
    }

    mq_close(mq_d);
    mq_unlink(MQ_NAME);

    return 0;
}
