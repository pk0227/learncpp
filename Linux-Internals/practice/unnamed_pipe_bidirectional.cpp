#include <cstddef>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <thread>
#include <chrono>
#include <string>



int main()
{
  int parent_to_child[2]{};
  int child_to_parent[2]{};

  if((pipe(parent_to_child) == -1) || (pipe(child_to_parent) == -1))
  {
    perror("pipe failed..!");
    return 1;
  }

  pid_t pid = fork();

  if(pid == 0)  // child
  {
    close(parent_to_child[1]);
    close(child_to_parent[0]);
    
    char buf[256];
    ssize_t cr{}, cw{};

    while(true)
    {
      std::memset(buf, 0, std::size(buf));
      cr = cw = 0;
      cr = read(parent_to_child[0], buf, std::size(buf));
      if(cr>0)
      {
        std::cout << "child  [received from parent] : " << buf << "\n";

        std::memset(buf, 0, std::size(buf));
        std::cout << "child : ";
        std::cin.getline(buf, std::size(buf));
        size_t count {std::strlen(buf)};
        if(count) ++count;
        cw = write(child_to_parent[1], buf, count);
      }
      else if(cr == -1)
      {
        perror("pipe error from child");
        exit(1);
      }
      
      if(cw<=0)
        break;
    }

    close(parent_to_child[0]);
    close(child_to_parent[1]);
    exit(0);
  }
  else  // parent
  {
    close(parent_to_child[0]);
    close(child_to_parent[1]);
    
    char buf[256];
    ssize_t cr{}, cw{};
    
    while(true)
    {
      std::memset(buf, 0, std::size(buf));
      cr = cw = 0;
      std::cout << "parent : ";
      std::cin.getline(buf, std::size(buf));
      size_t count {std::strlen(buf)};
      if(count) ++count;
      cw = write(parent_to_child[1], buf, count);
      if(cw>0)
      {   
        std::memset(buf, 0, std::size(buf));
        cr = read(child_to_parent[0], buf, std::size(buf));
        if(cr)
          std::cout << "parent  [received from child] : " << buf << "\n";

      }
      else if(cw == -1)
      {
          perror("pipe error from parent");
          exit(1);
      }

      if(cr<=0)
        break;
    }

    close(parent_to_child[1]);
    close(child_to_parent[0]);
    wait(nullptr);
  }

  return 0;
}

