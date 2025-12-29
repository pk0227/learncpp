#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <format>
#include <sys/wait.h>
#include <chrono>
#include <thread>

int main()
{
  int fd[2]{};

  if(pipe(fd) == -1)
  {
    perror("pipe failed..!");
    return 1;
  }

  pid_t pid {fork()};

  if(pid == 0)  //child
  {
    close(fd[1]);
    
    char buf[256]{};

    while(read(fd[0], buf, std::size(buf)))
    {
      std::cout << "Reader : " << buf << "\n";
      std::memset(buf, 0, std::size(buf));
    }
    
    close(fd[0]);
    
    std::cout << "Exiting from child\n";
    exit(0);
  }
  else  // parent
  {
    close(fd[0]);
    
    char buf[256];

    do 
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      std::memset(buf, 0, std::size(buf));
      std::cout << "Enter msg : ";
      std::cin.getline(buf, std::size(buf));
    }while(write(fd[1], buf, std::strlen(buf)));
  
    close(fd[1]);

    std::cout << "Waiting for child to join\n";
    wait(nullptr);
  }
  
    
  return 0;
}
