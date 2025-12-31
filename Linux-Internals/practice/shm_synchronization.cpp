#include <iostream>
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>

int main()
{
    const char* SEM_NAME {"/sem_lock"};
    sem_unlink(SEM_NAME);

    sem_t* sem { sem_open(SEM_NAME, O_CREAT | O_EXCL, 0666, 1) };
    if(sem == SEM_FAILED)
    {
        perror("sem failed");
        return -1;
    }

    const char* SHM_NAME {"/shm_memory"};
    const int SHM_SIZE {4};
    shm_unlink(SHM_NAME);

    int shm_fd { shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666) };
    if(shm_fd == -1)
    {
        perror("shm_fd error");
        return -1;
    }
    
    if(ftruncate(shm_fd, SHM_SIZE))
    {
        perror("ftruncate failed");
        close(shm_fd);
        return -1;
    }

    int* shm_ptr { static_cast<int *>( mmap(nullptr, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0) ) };
    
    for(int it{}; it < 100; it++)
    {
        for(int chd{}; chd < 100; chd++)
        {
            pid_t pid { fork() };
            if(pid == 0)
            {
                for(int i{}; i < 100; i++)
                {
                    sem_wait(sem);
                    (*shm_ptr)++;
                    sem_post(sem);
                    sched_yield();
                }

                munmap(shm_ptr, SHM_SIZE);
                close(shm_fd);

                sem_close(sem);

                exit(0);
            }
        }

        {
            for(int chd{}; chd < 100; chd++)
                wait(nullptr);
        }

        std::cout << "count : " << *shm_ptr << "\n";
        *shm_ptr = 0;
    }

    munmap(shm_ptr, SHM_SIZE);
    close(shm_fd);
    shm_unlink(SHM_NAME);

    sem_close(sem);
    sem_unlink(SEM_NAME);
    
    return 0;
}
