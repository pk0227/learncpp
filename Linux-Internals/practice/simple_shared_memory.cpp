#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>


int main()
{
    const char* SHM_NAME {"/shm_file"};
    const int SHM_SIZE {4096};

    shm_unlink(SHM_NAME);
    int shm_fd { shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0666) };
    if(shm_fd == -1)
    {
        perror("shm_fd error");
        return -1;
    }

    if(ftruncate(shm_fd, SHM_SIZE) == -1)
    {
        perror("ftruncate error");
        close(shm_fd);
        return -1;
    }
    
    char *shm_ptr { static_cast<char*>(mmap(nullptr, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0)) };

    std::cout << "shared memory has been created and is now ready to use.\n";

    pid_t pid { fork() };
    if(pid == 0)  //child
    {
        sleep(1);
        std::cout << "Child read : " << shm_ptr << "\n";
        munmap(shm_ptr, SHM_SIZE);
        close(shm_fd);
        exit(0);
    }
    else   //parent
    {
        std::strcpy(shm_ptr, "greetings from parent process...!");
        wait(nullptr);
    }

    munmap(shm_ptr, SHM_SIZE);
    close(shm_fd);
    shm_unlink(SHM_NAME);
    
    return 0;
}
