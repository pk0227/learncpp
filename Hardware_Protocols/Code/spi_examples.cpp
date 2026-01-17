/*
 * SPI Protocol - Production-Grade C++ Examples
 * For 7+ Years Embedded C++ Developer Interviews
 *
 * Compile: g++ -std=c++20 -Wall -Wextra -o spi_examples spi_examples.cpp
 * Run (requires root): sudo ./spi_examples
 */

#include <chrono>
#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <iomanip>
#include <iostream>
#include <linux/spi/spidev.h>
#include <stdexcept>
#include <sys/ioctl.h>
#include <thread>
#include <unistd.h>
#include <vector>

// ============================================================================
// Example 1: Basic SPI Wrapper Class (RAII, Mode Configuration)
// ============================================================================

class SPIDevice {
private:
  int fd{-1};
  uint32_t speed_hz;
  uint8_t bits_per_word;
  uint8_t mode;
  std::string device_path;

public:
  enum class Mode {
    MODE_0 = SPI_MODE_0, // CPOL=0, CPHA=0
    MODE_1 = SPI_MODE_1, // CPOL=0, CPHA=1
    MODE_2 = SPI_MODE_2, // CPOL=1, CPHA=0
    MODE_3 = SPI_MODE_3  // CPOL=1, CPHA=1
  };

  SPIDevice(const std::string &device, uint32_t speed = 1000000,
            Mode spi_mode = Mode::MODE_0, uint8_t bits = 8)
      : speed_hz(speed), bits_per_word(bits),
        mode(static_cast<uint8_t>(spi_mode)), device_path(device) {

    fd = open(device.c_str(), O_RDWR);
    if (fd < 0) {
      throw std::runtime_error("Failed to open SPI device: " + device);
    }

    // Set SPI mode
    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0) {
      close(fd);
      throw std::runtime_error("Failed to set SPI mode");
    }

    // Set bits per word
    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word) < 0) {
      close(fd);
      throw std::runtime_error("Failed to set bits per word");
    }

    // Set max speed
    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed_hz) < 0) {
      close(fd);
      throw std::runtime_error("Failed to set max speed");
    }

    std::cout << "[SPI] Opened " << device << " @ " << speed_hz << " Hz, Mode "
              << static_cast<int>(spi_mode) << ", " << static_cast<int>(bits)
              << " bits\n";
  }

  ~SPIDevice() {
    if (fd >= 0) {
      close(fd);
      std::cout << "[SPI] Closed " << device_path << "\n";
    }
  }

  // Delete copy, allow move
  SPIDevice(const SPIDevice &) = delete;
  SPIDevice &operator=(const SPIDevice &) = delete;
  SPIDevice(SPIDevice &&other) noexcept
      : fd(other.fd), speed_hz(other.speed_hz),
        bits_per_word(other.bits_per_word), mode(other.mode),
        device_path(std::move(other.device_path)) {
    other.fd = -1;
  }

  // Full-duplex transfer (simultaneous TX/RX)
  bool transfer(const std::vector<uint8_t> &tx_data,
                std::vector<uint8_t> &rx_data) {
    rx_data.resize(tx_data.size());

    struct spi_ioc_transfer tr {};
    tr.tx_buf = reinterpret_cast<uintptr_t>(tx_data.data());
    tr.rx_buf = reinterpret_cast<uintptr_t>(rx_data.data());
    tr.len = tx_data.size();
    tr.speed_hz = speed_hz;
    tr.bits_per_word = bits_per_word;
    tr.delay_usecs = 0;
    tr.cs_change = 0; // Keep CS asserted

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 0) {
      std::cerr << "[SPI] Transfer failed: " << strerror(errno) << "\n";
      return false;
    }

    return true;
  }

  // Simple write (ignore RX data)
  bool write(const std::vector<uint8_t> &data) {
    std::vector<uint8_t> dummy_rx;
    return transfer(data, dummy_rx);
  }

  // Simple read (send dummy 0x00)
  bool read(std::vector<uint8_t> &data, size_t length) {
    std::vector<uint8_t> tx_dummy(length, 0x00);
    return transfer(tx_dummy, data);
  }

  // Multi-transfer (for complex transactions with CS control)
  bool multiTransfer(const std::vector<std::vector<uint8_t>> &segments,
                     std::vector<std::vector<uint8_t>> &rx_segments) {
    rx_segments.resize(segments.size());

    std::vector<struct spi_ioc_transfer> transfers(segments.size());

    for (size_t i = 0; i < segments.size(); ++i) {
      rx_segments[i].resize(segments[i].size());

      transfers[i] = {};
      transfers[i].tx_buf = reinterpret_cast<uintptr_t>(segments[i].data());
      transfers[i].rx_buf = reinterpret_cast<uintptr_t>(rx_segments[i].data());
      transfers[i].len = segments[i].size();
      transfers[i].speed_hz = speed_hz;
      transfers[i].bits_per_word = bits_per_word;
      transfers[i].delay_usecs = 0;
      transfers[i].cs_change = (i < segments.size() - 1) ? 1 : 0;
    }

    if (ioctl(fd, SPI_IOC_MESSAGE(transfers.size()), transfers.data()) < 0) {
      std::cerr << "[SPI] Multi-transfer failed\n";
      return false;
    }

    return true;
  }
};

// ============================================================================
// Example 2: SPI Flash Memory (W25Q64) - 8MB NOR Flash
// ============================================================================

class W25Q_Flash {
private:
  SPIDevice &spi;

  // Command set
  static constexpr uint8_t CMD_WRITE_ENABLE = 0x06;
  static constexpr uint8_t CMD_WRITE_DISABLE = 0x04;
  static constexpr uint8_t CMD_READ_STATUS1 = 0x05;
  static constexpr uint8_t CMD_READ_STATUS2 = 0x35;
  static constexpr uint8_t CMD_PAGE_PROGRAM = 0x02;
  static constexpr uint8_t CMD_QUAD_PAGE_PROGRAM = 0x32;
  static constexpr uint8_t CMD_SECTOR_ERASE = 0x20;
  static constexpr uint8_t CMD_BLOCK_ERASE_32K = 0x52;
  static constexpr uint8_t CMD_BLOCK_ERASE_64K = 0xD8;
  static constexpr uint8_t CMD_CHIP_ERASE = 0xC7;
  static constexpr uint8_t CMD_READ_DATA = 0x03;
  static constexpr uint8_t CMD_FAST_READ = 0x0B;
  static constexpr uint8_t CMD_READ_JEDEC_ID = 0x9F;
  static constexpr uint8_t CMD_POWER_DOWN = 0xB9;
  static constexpr uint8_t CMD_RELEASE_POWER_DOWN = 0xAB;

  static constexpr size_t PAGE_SIZE = 256;
  static constexpr size_t SECTOR_SIZE = 4096;

public:
  W25Q_Flash(SPIDevice &device) : spi(device) {}

  // Read JEDEC ID (Manufacturer + Device)
  bool readID(uint8_t &manufacturer, uint16_t &device_id) {
    std::vector<uint8_t> tx = {CMD_READ_JEDEC_ID, 0x00, 0x00, 0x00};
    std::vector<uint8_t> rx;

    if (!spi.transfer(tx, rx)) {
      return false;
    }

    manufacturer = rx[1];
    device_id = (rx[2] << 8) | rx[3];

    std::cout << "[W25Q] Manufacturer: 0x" << std::hex << (int)manufacturer
              << ", Device: 0x" << device_id << std::dec << "\n";
    return true;
  }

  // Read status register
  uint8_t readStatus() {
    std::vector<uint8_t> tx = {CMD_READ_STATUS1, 0x00};
    std::vector<uint8_t> rx;

    if (!spi.transfer(tx, rx)) {
      return 0xFF;
    }

    return rx[1];
  }

  // Check if busy (WIP = Work In Progress bit)
  bool isBusy() { return (readStatus() & 0x01) != 0; }

  // Wait until ready
  bool waitReady(uint32_t timeout_ms = 1000) {
    auto start = std::chrono::steady_clock::now();

    while (isBusy()) {
      auto now = std::chrono::steady_clock::now();
      auto elapsed =
          std::chrono::duration_cast<std::chrono::milliseconds>(now - start);

      if (elapsed.count() > timeout_ms) {
        std::cerr << "[W25Q] Timeout waiting for ready\n";
        return false;
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return true;
  }

  // Write enable
  bool writeEnable() {
    std::vector<uint8_t> tx = {CMD_WRITE_ENABLE};
    return spi.write(tx);
  }

  // Read data from address
  bool readData(uint32_t address, std::vector<uint8_t> &data, size_t length) {
    std::vector<uint8_t> tx;
    tx.push_back(CMD_READ_DATA);
    tx.push_back((address >> 16) & 0xFF);
    tx.push_back((address >> 8) & 0xFF);
    tx.push_back(address & 0xFF);

    // Add dummy bytes for read
    for (size_t i = 0; i < length; ++i) {
      tx.push_back(0x00);
    }

    std::vector<uint8_t> rx;
    if (!spi.transfer(tx, rx)) {
      return false;
    }

    // Extract data (skip command and address)
    data.assign(rx.begin() + 4, rx.end());
    return true;
  }

  // Page program (write up to 256 bytes)
  bool pageProgram(uint32_t address, const std::vector<uint8_t> &data) {
    if (data.empty() || data.size() > PAGE_SIZE) {
      std::cerr << "[W25Q] Invalid page size\n";
      return false;
    }

    // Must not cross page boundary
    if ((address % PAGE_SIZE) + data.size() > PAGE_SIZE) {
      std::cerr << "[W25Q] Write crosses page boundary\n";
      return false;
    }

    if (!writeEnable())
      return false;

    std::vector<uint8_t> tx;
    tx.push_back(CMD_PAGE_PROGRAM);
    tx.push_back((address >> 16) & 0xFF);
    tx.push_back((address >> 8) & 0xFF);
    tx.push_back(address & 0xFF);
    tx.insert(tx.end(), data.begin(), data.end());

    if (!spi.write(tx))
      return false;

    return waitReady();
  }

  // Erase 4KB sector
  bool sectorErase(uint32_t address) {
    if (!writeEnable())
      return false;

    std::vector<uint8_t> tx = {CMD_SECTOR_ERASE,
                               static_cast<uint8_t>((address >> 16) & 0xFF),
                               static_cast<uint8_t>((address >> 8) & 0xFF),
                               static_cast<uint8_t>(address & 0xFF)};

    if (!spi.write(tx))
      return false;

    return waitReady(5000); // Sector erase takes ~400ms
  }

  // Chip erase (erase entire flash)
  bool chipErase() {
    if (!writeEnable())
      return false;

    std::vector<uint8_t> tx = {CMD_CHIP_ERASE};
    if (!spi.write(tx))
      return false;

    return waitReady(60000); // Chip erase takes ~40s
  }
};

// ============================================================================
// Example 3: MCP3008 - 8-Channel 10-bit ADC
// ============================================================================

class MCP3008_ADC {
private:
  SPIDevice &spi;

public:
  enum class Channel {
    CH0 = 0,
    CH1 = 1,
    CH2 = 2,
    CH3 = 3,
    CH4 = 4,
    CH5 = 5,
    CH6 = 6,
    CH7 = 7
  };

  MCP3008_ADC(SPIDevice &device) : spi(device) {}

  // Read single-ended channel (10-bit result)
  bool readChannel(Channel channel, uint16_t &value) {
    uint8_t ch = static_cast<uint8_t>(channel);

    // Build command: Start bit + SGL/DIFF + D2 D1 D0
    std::vector<uint8_t> tx = {
        0x01,                                     // Start bit
        static_cast<uint8_t>((0x80 | (ch << 4))), // SGL=1, D2-D0
        0x00                                      // Don't care
    };

    std::vector<uint8_t> rx;
    if (!spi.transfer(tx, rx)) {
      return false;
    }

    // Extract 10-bit result from rx[1] and rx[2]
    value = ((rx[1] & 0x03) << 8) | rx[2];
    return true;
  }

  // Read all 8 channels
  bool readAllChannels(std::vector<uint16_t> &values) {
    values.resize(8);

    for (int i = 0; i < 8; ++i) {
      if (!readChannel(static_cast<Channel>(i), values[i])) {
        return false;
      }
    }

    return true;
  }
};

// ============================================================================
// Example 4: MAX7219 - 8-Digit LED Display Driver
// ============================================================================

class MAX7219_Display {
private:
  SPIDevice &spi;

  // Register addresses
  enum class Register : uint8_t {
    NO_OP = 0x00,
    DIGIT0 = 0x01,
    DIGIT1 = 0x02,
    DIGIT2 = 0x03,
    DIGIT3 = 0x04,
    DIGIT4 = 0x05,
    DIGIT5 = 0x06,
    DIGIT6 = 0x07,
    DIGIT7 = 0x08,
    DECODE_MODE = 0x09,
    INTENSITY = 0x0A,
    SCAN_LIMIT = 0x0B,
    SHUTDOWN = 0x0C,
    DISPLAY_TEST = 0x0F
  };

public:
  MAX7219_Display(SPIDevice &device) : spi(device) {
    // Initialize display
    writeRegister(Register::DECODE_MODE, 0xFF);  // BCD decode for all digits
    writeRegister(Register::SCAN_LIMIT, 0x07);   // Scan all 8 digits
    writeRegister(Register::INTENSITY, 0x08);    // Medium brightness
    writeRegister(Register::SHUTDOWN, 0x01);     // Normal operation
    writeRegister(Register::DISPLAY_TEST, 0x00); // Normal mode

    clear();
  }

  // Write to register
  bool writeRegister(Register reg, uint8_t value) {
    std::vector<uint8_t> tx = {static_cast<uint8_t>(reg), value};
    return spi.write(tx);
  }

  // Set digit (0-7) to value (0-9, or special chars)
  bool setDigit(uint8_t digit, uint8_t value, bool decimal_point = false) {
    if (digit > 7)
      return false;

    uint8_t data = value & 0x0F;
    if (decimal_point) {
      data |= 0x80; // Set DP bit
    }

    Register reg =
        static_cast<Register>(static_cast<uint8_t>(Register::DIGIT0) + digit);
    return writeRegister(reg, data);
  }

  // Display integer number
  bool displayNumber(int32_t number) {
    bool negative = number < 0;
    if (negative)
      number = -number;

    for (int i = 0; i < 8; ++i) {
      if (number == 0 && i > 0 && !negative) {
        setDigit(i, 0x0F); // Blank
      } else {
        setDigit(i, number % 10);
        number /= 10;
      }
    }

    return true;
  }

  // Clear display
  bool clear() {
    for (uint8_t i = 0; i < 8; ++i) {
      setDigit(i, 0x0F); // Blank character
    }
    return true;
  }

  // Set brightness (0-15)
  bool setBrightness(uint8_t level) {
    if (level > 15)
      level = 15;
    return writeRegister(Register::INTENSITY, level);
  }
};

// ============================================================================
// Main: Demonstration
// ============================================================================

int main() {
  std::cout << "=== SPI Protocol - Production Examples ===\n\n";

  try {
    // Example 1: Flash Memory
    std::cout << "--- SPI Flash Example (W25Q64) ---\n";
    SPIDevice flash_spi("/dev/spidev0.0", 10000000, SPIDevice::Mode::MODE_0);
    W25Q_Flash flash(flash_spi);

    uint8_t mfr;
    uint16_t dev_id;
    if (flash.readID(mfr, dev_id)) {
      std::cout << "[Flash] Detected: Manufacturer 0x" << std::hex << (int)mfr
                << ", Device 0x" << dev_id << std::dec << "\n";

      // Read some data
      std::vector<uint8_t> data;
      if (flash.readData(0x0000, data, 16)) {
        std::cout << "[Flash] Data at 0x0000: ";
        for (auto byte : data) {
          std::cout << std::hex << std::setw(2) << std::setfill('0')
                    << (int)byte << " ";
        }
        std::cout << std::dec << "\n";
      }
    }

    // Example 2: ADC
    std::cout << "\n--- SPI ADC Example (MCP3008) ---\n";
    SPIDevice adc_spi("/dev/spidev0.1", 1000000, SPIDevice::Mode::MODE_0);
    MCP3008_ADC adc(adc_spi);

    std::vector<uint16_t> adc_values;
    if (adc.readAllChannels(adc_values)) {
      std::cout << "[ADC] All channels:\n";
      for (size_t i = 0; i < adc_values.size(); ++i) {
        float voltage = (adc_values[i] / 1024.0f) * 3.3f;
        std::cout << "  CH" << i << ": " << adc_values[i] << " (~ "
                  << std::fixed << std::setprecision(2) << voltage << " V)\n";
      }
    }

    // Example 3: LED Display
    std::cout << "\n--- SPI LED Display Example (MAX7219) ---\n";
    SPIDevice display_spi("/dev/spidev0.2", 10000000, SPIDevice::Mode::MODE_0);
    MAX7219_Display display(display_spi);

    display.displayNumber(12345678);
    std::cout << "[Display] Showing: 12345678\n";

    std::this_thread::sleep_for(std::chrono::seconds(2));

    display.setBrightness(3);
    std::cout << "[Display] Brightness reduced\n";

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  std::cout << "\n=== SPI Examples Complete ===\n";
  return 0;
}

/*
 * SENIOR INTERVIEW TALKING POINTS:
 *
 * 1. Full-Duplex: SPI transfer() shows simultaneous TX/RX
 * 2. CPOL/CPHA Modes: Enum class for Mode 0-3 selection
 * 3. CS Management: ioctl handles CS automatically (cs_change for
 * multi-transfer)
 * 4. Speed Configuration: Per-device speed setting
 * 5. Page Boundaries: W25Q_Flash checks page crossing (common pitfall)
 * 6. Wait/Polling: waitReady() pattern for flash status polling
 * 7. 10-bit ADC: Bit manipulation to extract MCP3008 result
 * 8. Command Protocol: Each device has different command set
 * 9. Error Handling: All operations return bool for error checking
 * 10. RAII: SPIDevice manages fd lifecycle properly
 */
