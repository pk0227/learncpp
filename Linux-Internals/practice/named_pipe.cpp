#include <chrono>
#include <cstddef>
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
  const char *fifo_file {"/tmp/fifo_file"};

  unlink(fifo_file);
  if(mkfifo(fifo_file, 0666) == -1)
  {
    perror("mkfifo creation failed...!");
    return -1;
  }

  pid_t pid { fork() };

  if(pid == 0) //child
  {
    int rd { open(fifo_file, O_RDONLY) };
    if(rd == -1)
    {
      perror("opening a fifo failed in child");
      exit(1);
    }

    char buf[256]{};

    while(read(rd, buf, std::size(buf)))
    {
      std::cout << "Read : " << buf << "\n";
      std::memset(buf, 0, std::size(buf));
    }

    close(rd);
    exit(0);
  }
  else  // parent
  {
    int wd { open(fifo_file, O_WRONLY) };
    if(wd == -1)
    {
      perror("opening a fifo failed in parent");
      exit(1);
    }

    char buf[256];
    size_t count{};

    do
    {   
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      std::memset(buf, 0, std::size(buf));
      std::cout << "Enter msg : ";
      std::cin.getline(buf, std::size(buf));
      count = std::strlen(buf);
      if(count)  ++count;
    }while( write(wd, buf, count) );

    close(wd);
    waitpid(pid, nullptr, 0);
  }

  unlink(fifo_file);

  return 0;
}
