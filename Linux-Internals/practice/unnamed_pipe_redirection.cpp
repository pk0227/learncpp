#include <iostream>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char* argv[])
{
  if (argc < 3)
  {
    std::cerr << "Usage: " << argv[0] << " cmd1 cmd2\n";
    return 1;
  }
  
  int fd[2]{};

  if(pipe(fd) ==  -1)
  {
    perror("pipe failed");
    return -1;
  }

  pid_t pid1 {fork()};
  if(pid1 == 0)
  {
    close(fd[0]);
    dup2(fd[1], STDOUT_FILENO);
    close(fd[1]);

    execlp(argv[1], argv[1], "-l", nullptr);
    perror("execlp ls error");
    exit(1);
  }

  pid_t pid2 {fork()};
  if(pid2 == 0)
  {
    close(fd[1]);
    dup2(fd[0], STDIN_FILENO);
    close(fd[0]);

    execlp(argv[2], argv[2], "-l", nullptr);
    perror("execlp wc error");
    exit(1);
  }

  close(fd[0]);
  close(fd[1]);

  waitpid(pid1, nullptr, 0);
  waitpid(pid2, nullptr, 0);

  return 0;
}

