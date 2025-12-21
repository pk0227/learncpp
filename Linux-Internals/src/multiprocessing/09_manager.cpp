/*
 * PROCESS MANAGER PATTERN WITH FILE-BASED IPC
 * 
 * Demonstrates coordinating multiple processes using filesystem for communication.
 * 
 * ARCHITECTURE:
 * - Manager: Creates and coordinates worker processes
 * - Workers: Append data to shared file
 * - File: Acts as shared communication medium
 * 
 * DESIGN PATTERN:
 * - Manager forks N worker processes
 * - Each worker writes to shared file independently  
 * - Manager waits for all workers to complete
 * - Manager reads and displays collected results
 * 
 * FILE-BASED IPC:
 * - std::ofstream with std::ios_base::app (append mode)
 * - Each write creates new line in file
 * - Persistent: Data survives process termination
 * 
 * **POTENTIAL ISSUE:**
 * - No synchronization on file writes!
 * - Multiple processes writing simultaneously
 * - OS may interleave writes (though line-buffered usually safe)
 * - Production code should use flock() or file locking
 * 
 * WHEN TO USE FILE-BASED IPC:
 * - Need persistent data after processes exit
 * - Performance not critical
 * - Simple logging or data collection
 * - When other IPC mechanisms unavailable
 * 
 * BETTER ALTERNATIVES FOR PERFORMANCE:
 * - Shared memory (fastest)
 * - Message queues (structured)
 * - Pipes (streaming)
 * - Sockets (network-capable)
 * 
 * INTERVIEW INSIGHTS:
 * - Pros/cons of file-based IPC
 * - When is file locking necessary?
 * - Comparison of IPC mechanisms
 * - How to handle partial writes
 */

#include <fstream>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

const std::string shared_data_file = "shared_data.txt";

class Manager {
public:
  Manager(int num_processes)
      : num_processes_(num_processes), pids_(num_processes) {}

  void start() {
    for (int i = 0; i < num_processes_; ++i) {
      pids_[i] = fork();
      if (pids_[i] == 0) {
        worker(i);
        exit(0);
      }
    }

    for (const auto &pid : pids_) {
      int status;
      waitpid(pid, &status, 0);
    }
  }

  void print_results() {
    std::cout
        << "All processes have completed. The contents of the shared file are:"
        << std::endl;
    std::ifstream infile(shared_data_file);
    std::string line;
    while (std::getline(infile, line)) {
      std::cout << line << std::endl;
    }
    infile.close();
  }

private:
  int num_processes_;
  std::vector<pid_t> pids_;

  void worker(int id) {
    std::ofstream outfile(shared_data_file, std::ios_base::app);
    outfile << "Process " << id << " is writing to the shared file."
            << std::endl;
    outfile.close();
  }
};

int main() {
  const int num_processes = 3;

  Manager manager(num_processes);
  manager.start();
  manager.print_results();

  return 0;
}
