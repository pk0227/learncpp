/**
 * @file 03_isp_concepts_range_hierarchy.cpp
 * @brief Demonstrates Static Interface Segregation using C++20 Concepts (The STL Iterator Paradigm).
 * 
 * The Ultimate Real-World ISP Architecture: C++20 Iterator Concepts
 *   - If the C++ committee had designed a single monolithic "Iterator" interface containing
 *     `operator++`, `operator--`, `operator[]`, and `data()`, then single-pass input streams
 *     (like network sockets or stdin) would be forced to provide dummy implementations of `operator--`!
 *   - Instead, the STL segregates iterator capabilities into fine-grained, composable concepts:
 *       `input_iterator` -> `forward_iterator` -> `bidirectional_iterator` -> `random_access_iterator` -> `contiguous_iterator`.
 * 
 * This example demonstrates custom streaming pipeline concepts segregating:
 *   1. `ReadableStream`: Basic forward sequential read.
 *   2. `SeekableStream`: Random access position seeking.
 *   3. `FlushableStream`: Buffer write synchronization.
 * 
 * @standard C++20
 */

#include <iostream>
#include <concepts>
#include <span>
#include <vector>
#include <string_view>
#include <sstream>
#include <cstdint>

// ============================================================================
// 1. FINE-GRAINED C++20 CONCEPTS (Static Interface Segregation)
// ============================================================================

template <typename S>
concept ReadableStream = requires(S stream, void* buffer, std::size_t bytes) {
    { stream.read(buffer, bytes) } -> std::same_as<std::size_t>;
};

template <typename S>
concept SeekableStream = requires(S stream, std::streampos offset) {
    { stream.seek(offset) } -> std::same_as<bool>;
    { stream.tell() } -> std::same_as<std::streampos>;
};

template <typename S>
concept FlushableStream = requires(S stream) {
    { stream.flush() } -> std::same_as<void>;
};

// Composed Concept: ReadWriteSeekable (Composed without a fat base class!)
template <typename S>
concept FullFeaturedStorageStream = ReadableStream<S> && SeekableStream<S> && FlushableStream<S>;

// ============================================================================
// 2. CONCRETE STREAM TYPES
// ============================================================================

// Network Socket Stream: Can be read, but CANNOT be seeked backwards!
class NetworkSocketStream {
public:
    std::size_t read(void* /*buffer*/, std::size_t bytes) {
        std::cout << "[SocketStream] Streaming " << bytes << " bytes from TCP recv buffer.\n";
        return bytes;
    }
    // Zero seek() method defined! Perfectly segregated.
};

// Disk File Stream: Supports sequential reading, random seeking, and flushing!
class DiskFileStream {
private:
    std::streampos pos_{0};

public:
    std::size_t read(void* /*buffer*/, std::size_t bytes) {
        std::cout << "[DiskFileStream] Reading " << bytes << " bytes from disk offset " << pos_ << ".\n";
        pos_ += bytes;
        return bytes;
    }

    bool seek(std::streampos offset) {
        std::cout << "[DiskFileStream] Seeking to position " << offset << ".\n";
        pos_ = offset;
        return true;
    }

    [[nodiscard]] std::streampos tell() const noexcept {
        return pos_;
    }

    void flush() {
        std::cout << "[DiskFileStream] Flushing OS page cache to NVMe drive.\n";
    }
};

// ============================================================================
// 3. GENERIC CONSUMER PIPELINES (Constrained to minimal required concept!)
// ============================================================================

// Sequential reader: Requires ONLY ReadableStream
template <ReadableStream S>
void consumeHeader(S& stream) {
    uint8_t header[16];
    stream.read(header, sizeof(header));
}

// Random-access scanner: Requires BOTH ReadableStream AND SeekableStream
template <typename S>
    requires ReadableStream<S> && SeekableStream<S>
void inspectFileIndex(S& stream) {
    stream.seek(1024); // Jump to index table
    uint8_t indexEntry[32];
    stream.read(indexEntry, sizeof(indexEntry));
}

int main() {
    std::cout << "=== Static ISP via C++20 Concepts (STL Architecture) ===\n\n";

    NetworkSocketStream tcpSocket;
    DiskFileStream localFile;

    // 1. Sequential reader accepts both (both are ReadableStream)
    std::cout << "--- 1. Sequential Read Pipeline ---\n";
    consumeHeader(tcpSocket); // OK
    consumeHeader(localFile); // OK

    // 2. Random-access inspection: Socket is rejected AT COMPILE TIME!
    std::cout << "\n--- 2. Random Access Seek Pipeline ---\n";
    inspectFileIndex(localFile); // OK

    // The following will fail compilation cleanly with a readable concept error:
    // inspectFileIndex(tcpSocket); // ERROR: NetworkSocketStream does not satisfy SeekableStream!

    // Static concept verification
    static_assert(ReadableStream<NetworkSocketStream>);
    static_assert(!SeekableStream<NetworkSocketStream>, "TCP Socket cannot satisfy SeekableStream");
    static_assert(FullFeaturedStorageStream<DiskFileStream>, "DiskFileStream must satisfy full storage concept");

    std::cout << "\nStatic assertion confirmed: NetworkSocketStream successfully rejected from Seekable requirements!\n";
    return 0;
}
