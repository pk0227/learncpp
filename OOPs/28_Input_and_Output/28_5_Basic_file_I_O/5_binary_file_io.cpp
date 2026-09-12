// Demonstrates unformatted binary file I/O using ostream::write() and istream::read()
// Highlights std::ios::binary mode, reinterpret_cast, gcount(), and trivially-copyable checks

#include <iostream>
#include <fstream>
#include <type_traits>
#include <cstdint>
#include <string>

// Trivially copyable record struct: safe for direct binary disk persistence
struct PlayerRecord
{
    std::uint32_t id{};
    int32_t score{};
    double multiplier{};
    char rankCode{};
};

// Static check ensuring safety for binary memory copying
static_assert(std::is_trivially_copyable_v<PlayerRecord>, 
              "PlayerRecord must be trivially copyable for raw binary I/O");

int main()
{
    const std::string filename{ "player_data.bin" };

    // 1. Write binary data using ofstream::write()
    {
        std::ofstream out{ filename, std::ios::out | std::ios::binary };
        if (!out)
        {
            std::cerr << "Failed to open " << filename << " for writing!\n";
            return 1;
        }

        PlayerRecord p1{ 101, 8950, 1.75, 'S' };
        PlayerRecord p2{ 102, 6420, 1.25, 'A' };

        // Write raw byte buffer representation
        out.write(reinterpret_cast<const char*>(&p1), sizeof(PlayerRecord));
        out.write(reinterpret_cast<const char*>(&p2), sizeof(PlayerRecord));

        std::cout << "[SUCCESS] Wrote 2 PlayerRecord structs (" 
                  << 2 * sizeof(PlayerRecord) << " bytes) to " << filename << "\n";
    } // out is automatically flushed and closed here by its destructor

    // 2. Read binary data using ifstream::read()
    {
        std::ifstream in{ filename, std::ios::in | std::ios::binary };
        if (!in)
        {
            std::cerr << "Failed to open " << filename << " for reading!\n";
            return 1;
        }

        PlayerRecord readRecord{};
        int count{ 0 };

        while (in.read(reinterpret_cast<char*>(&readRecord), sizeof(PlayerRecord)))
        {
            ++count;
            std::cout << "Record " << count << ": ID=" << readRecord.id 
                      << ", Score=" << readRecord.score 
                      << ", Multiplier=" << readRecord.multiplier 
                      << ", Rank=" << readRecord.rankCode 
                      << " (Extracted bytes: " << in.gcount() << ")\n";
        }

        if (in.eof())
        {
            std::cout << "[SUCCESS] Reached end-of-file cleanly.\n";
        }
    }

    // Clean up temporary binary file
    std::remove(filename.c_str());

    return 0;
}
