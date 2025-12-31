#include <iostream>
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>

struct shared_data
{
    sem_t mutex{};
    int count{};
};

using SHM_DATA = struct shared_data;

int main()
{
    const char* SHM_NAME { "/shm_memory" };
    shm_unlink(SHM_NAME);

    int shm_fd { shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666) };
    if(shm_fd == -1)
    {
        perror("shm_fd failed");
        return -1;
    }

    if(ftruncate(shm_fd, sizeof(SHM_DATA)))
    {
        perror("ftruncate failed");
        close(shm_fd);
        return -1;
    }

    SHM_DATA *shm_ptr { static_cast<SHM_DATA*>(mmap(nullptr, sizeof(SHM_DATA), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0)) };
    
    sem_init(&(shm_ptr->mutex), 1, 1);

    for(int it{}; it < 100; it++)
    {
        for(int chd{}; chd < 100; chd++)
        {
            pid_t pid { fork() };
            if(pid == 0) // children
            {
                for(int i{}; i < 100; i++)
                {
                    sem_wait(&(shm_ptr->mutex));
                    (shm_ptr->count)++;
                    sem_post(&(shm_ptr->mutex));
                    //sched_yield();
                } 
                
                munmap(shm_ptr, sizeof(SHM_DATA));
                close(shm_fd);
                exit(0);
            }
        }
        
        // parents
        {
            for(int chd{}; chd < 100; chd++)
                wait(nullptr);
        }

        std::cout << "count : " << shm_ptr->count << "\n";
        shm_ptr->count = 0;
    }
   
    sem_destroy(&(shm_ptr->mutex));
    munmap(shm_ptr, sizeof(SHM_DATA));
    close(shm_fd);
    shm_unlink(SHM_NAME);

    return 0;
}
