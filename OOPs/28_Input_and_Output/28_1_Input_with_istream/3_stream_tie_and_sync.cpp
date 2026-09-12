// Demonstrates C++ stream synchronization and tying mechanisms
// Shows how to decouple C++ streams from C stdio and untie cin from cout for high performance

#include <iostream>

void demonstrateStreamTying()
{
    std::cout << "--- 1. Stream Tying Demonstration ---\n";
    // By default, std::cin is tied to std::cout (std::cin.tie() == &std::cout).
    // This guarantees that any prompt sent to std::cout is flushed before std::cin blocks for input.
    std::ostream* defaultTie = std::cin.tie();
    if (defaultTie == &std::cout)
    {
        std::cout << "[INFO] std::cin is currently tied to std::cout by default.\n";
    }

    // Untying cin from cout:
    std::cin.tie(nullptr);
    std::cout << "[INFO] std::cin has been untied from std::cout (tie is now nullptr).\n";

    // Restoring default tie:
    std::cin.tie(&std::cout);
    std::cout << "[INFO] std::cin tie restored to &std::cout.\n";
}

void demonstrateStdioSync()
{
    std::cout << "\n--- 2. stdio Synchronization ---\n";
    // By default, C++ standard streams synchronize with C standard I/O (stdin, stdout, stderr).
    // Disabling synchronization avoids buffer synchronization overhead between C and C++ streams:
    std::ios_base::sync_with_stdio(false);
    std::cout << "[INFO] sync_with_stdio set to false for maximum I/O throughput.\n";
    std::cout << "[NOTE] After disabling synchronization, never interleave C stdio (printf/scanf) with C++ streams (cin/cout)!\n";
}

int main()
{
    demonstrateStreamTying();
    demonstrateStdioSync();

    return 0;
}
