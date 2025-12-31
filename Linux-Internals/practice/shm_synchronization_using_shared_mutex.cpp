#include <chrono>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <pthread.h>
#include <sched.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

struct shared_data
{
    pthread_mutex_t shm_mutex{};
    int shm_count{};
};

int main()
{
    const char* SHM_NAME{ "/shm_memory" };

    shm_unlink(SHM_NAME);
    int shm_fd { shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666) };
    if(shm_fd == -1)
    {
        perror("shm_fd open error");
        return -1;
    }

    if(ftruncate(shm_fd, sizeof(struct shared_data)) == -1)
    {
        perror("ftruncate failed");
        close(shm_fd);
        return -1;
    }

    struct shared_data *shm_ptr { static_cast<struct shared_data*>(mmap(nullptr, sizeof(struct shared_data), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0))};
    
    pthread_mutexattr_t attr{};
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);

    pthread_mutex_init(&(shm_ptr->shm_mutex), &attr);
    pthread_mutexattr_destroy(&attr);

    for(int it{}; it < 100; it++)
    {
        for(int chd{}; chd < 100; chd++)
        {
            pid_t pid { fork() };
            if(pid == 0)  //child
            {
                for(int i{}; i < 100; i++)
                {
                    pthread_mutex_lock(&(shm_ptr->shm_mutex));
                    shm_ptr->shm_count++;
                    pthread_mutex_unlock(&(shm_ptr->shm_mutex));
                    sched_yield();
                }
                munmap(shm_ptr, sizeof(struct shared_data));
                close(shm_fd);
                exit(0);
            }
        }

        //parent
        {
            for(int chd{}; chd < 100; chd++)
            {
                wait(nullptr);
            }
        }
        std::cout << "count : " << shm_ptr->shm_count << "\n";
        shm_ptr->shm_count = 0;
    }

    pthread_mutex_destroy(&(shm_ptr->shm_mutex));
    munmap(shm_ptr, sizeof(struct shared_data));
    close(shm_fd);
    shm_unlink(SHM_NAME);

    return 0;
}
