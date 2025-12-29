#include <cstdio>
#include <cstring>
#include <iostream>
#include <iterator>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    const char* parent_fifo { "/tmp/parent_fifo" };
    const char* child_fifo { "/tmp/child_fifo" };

    unlink(parent_fifo);
    unlink(child_fifo);

    if( (mkfifo(parent_fifo, 0666) == -1) || (mkfifo(child_fifo, 0666)) )
    {
        perror("error in making fifos");
        return 1;
    }

    pid_t pid { fork() };
    if(pid == 0) // child
    {
        int ch_r { open(parent_fifo, O_RDONLY) };
        int ch_w { open(child_fifo, O_WRONLY) };

        if( (ch_r == -1) || (ch_w == -1) )
        {
            perror("open fifo error in child");
            exit(1);
        }

        char buf[256];
        size_t count{};
        ssize_t cr{}, cw{};

        while(1)
        {
            cr = cw = 0;
            std::memset(buf, 0, std::size(buf));
            cr = read(ch_r, buf, std::size(buf));
            if(cr > 0)
            {
                std::cout << "child [received from parent] : " << buf << "\n";
                std::memset(buf, 0, std::size(buf));
                std::cout << "child : ";
                std::cin.getline(buf, std::size(buf));
                count = std::strlen(buf);
                if(count)  ++count;
                cw = write(ch_w, buf, count);
            }
            else if(cr == -1)
            {
                perror("reading error in child");
                close(ch_r);
                close(ch_w);
                exit(1);
            }

            if(cw == -1)
            {
                perror("error writing from child");
                close(ch_r);
                close(ch_w);
                exit(1);
            }
            else if(cw == 0)
                break;
        }
        
        close(ch_r);
        close(ch_w);
        exit(0);
    }
    else // parent 
    {
        int p_w { open(parent_fifo, O_WRONLY) };
        int p_r { open(child_fifo, O_RDONLY) };
        
        if( (p_r == -1) || (p_w == -1) )
        {
            perror("open fifo error in parent");
            exit(1);
        }
        
        char buf[256];
        size_t count{};
        ssize_t cr{}, cw{};
        
        while(1)
        {
            cr = cw = 0;
            std::memset(buf, 0, std::size(buf));
            std::cout << "parent : ";
            std::cin.getline(buf, std::size(buf));
            count = std::strlen(buf);
            if(count)  ++count;
            cw = write(p_w, buf, count);
            
            if(cw > 0)
            {             
                std::memset(buf, 0, std::size(buf));
                cr = read(p_r, buf, std::size(buf));
                if(cr > 0)
                    std::cout << "parent [received from child] : " << buf << "\n";
            }
            else if(cw == -1)
            {
                perror("writing error in parent");
                break;
            }

            if(cr == -1)
            {
                perror("error reading in parent");
                break;
            }
            else if(cr == 0)
                break;

        }
        
        close(p_r);
        close(p_w);
        waitpid(pid, nullptr, 0);
    }

    unlink(parent_fifo);
    unlink(child_fifo);

    return 0;
}
