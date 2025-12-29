#include <csignal>
#include <cstring>
#include <iostream>
#include <unistd.h>


int main()
{
  int fd[2]{};

  if(pipe(fd) == -1)
  {
    perror("pipe error");
    return 1;
  }

  signal(SIGPIPE, SIG_IGN);

  //close(fd[1]);
  close(fd[0]);

  //char buf[256]{};
  //ssize_t result { read(fd[0], buf, std::size(buf)) };
  const char* msg {"hello"};
  ssize_t result { write(fd[1], msg, std::strlen(msg)+1) };

  if(result <= 0)
  {
    //std::cout << "read done. if any error: " << errno << "\n";
    std::cout << "write done. if any error: " << errno << "\n";
  }
  
  return 0;
}
