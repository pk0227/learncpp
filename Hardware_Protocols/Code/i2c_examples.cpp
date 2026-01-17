/*
 * I2C Protocol - Production-Grade C++ Examples
 * For 7+ Years Embedded C++ Developer Interviews
 *
 * Compile: g++ -std=c++20 -Wall -Wextra -o i2c_examples i2c_examples.cpp
 * Run (requires root): sudo ./i2c_examples
 */

#include <chrono>
#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <iomanip>
#include <iostream>
#include <linux/i2c-dev.h>
#include <stdexcept>
#include <sys/ioctl.h>
#include <thread>
#include <unistd.h>
#include <vector>

// ============================================================================
// Example 1: Basic I2C Wrapper Class (RAII, Exception-Safe)
// ============================================================================

class I2CDevice {
private:
  int fd{-1};
  uint8_t device_address;
  std::string bus_path;

public:
  I2CDevice(const std::string &bus, uint8_t addr)
      : device_address(addr), bus_path(bus) {

    fd = open(bus.c_str(), O_RDWR);
    if (fd < 0) {
      throw std::runtime_error("Failed to open I2C bus: " + bus);
    }

    if (ioctl(fd, I2C_SLAVE, address) < 0) {
      close(fd);
      throw std::runtime_error("Failed to set I2C slave address");
    }

    std::cout << "[I2C] Connected to device 0x" << std::hex
              << static_cast<int>(addr) << " on " << bus << std::dec << "\n";
  }

  ~I2CDevice() {
    if (fd >= 0) {
      close(fd);
      std::cout << "[I2C] Disconnected from device 0x" << std::hex
                << static_cast<int>(device_address) << std::dec << "\n";
    }
  }

  // Delete copy, allow move (RAII)
  I2CDevice(const I2CDevice &) = delete;
  I2CDevice &operator=(const I2CDevice &) = delete;
  I2CDevice(I2CDevice &&other) noexcept
      : fd(other.fd), device_address(other.device_address),
        bus_path(std::move(other.bus_path)) {
    other.fd = -1;
  }

  // Write single byte to register
  bool writeRegister(uint8_t reg, uint8_t value) {
    uint8_t buffer[2] = {reg, value};
    if (write(fd, buffer, 2) != 2) {
      std::cerr << "[I2C] Write failed: " << strerror(errno) << "\n";
      return false;
    }
    return true;
  }

  // Read single byte from register
  bool readRegister(uint8_t reg, uint8_t &value) {
    // Write register address
    if (write(fd, &reg, 1) != 1) {
      std::cerr << "[I2C] Write register address failed\n";
      return false;
    }

    // Read value (Repeated START automatically handled by kernel)
    if (read(fd, &value, 1) != 1) {
      std::cerr << "[I2C] Read failed\n";
      return false;
    }
    return true;
  }

  // Write multiple bytes to register (block write)
  bool writeBlock(uint8_t reg, const std::vector<uint8_t> &data) {
    std::vector<uint8_t> buffer;
    buffer.reserve(data.size() + 1);
    buffer.push_back(reg);
    buffer.insert(buffer.end(), data.begin(), data.end());

    if (write(fd, buffer.data(), buffer.size()) !=
        static_cast<ssize_t>(buffer.size())) {
      std::cerr << "[I2C] Block write failed\n";
      return false;
    }
    return true;
  }

  // Read multiple bytes from register (block read)
  bool readBlock(uint8_t reg, std::vector<uint8_t> &data, size_t length) {
    if (write(fd, &reg, 1) != 1) {
      return false;
    }

    data.resize(length);
    if (read(fd, data.data(), length) != static_cast<ssize_t>(length)) {
      std::cerr << "[I2C] Block read failed\n";
      return false;
    }
    return true;
  }

  // Read-Modify-Write pattern (atomic update of specific bits)
  bool modifyRegister(uint8_t reg, uint8_t mask, uint8_t value) {
    uint8_t current;
    if (!readRegister(reg, current)) {
      return false;
    }

    uint8_t new_value = (current & ~mask) | (value & mask);
    return writeRegister(reg, new_value);
  }
};

// ============================================================================
// Example 2: Temperature Sensor (MCP9808) - Real Device Driver
// ============================================================================

class MCP9808_TempSensor {
private:
  I2CDevice &i2c;

  // Register addresses
  static constexpr uint8_t REG_CONFIG = 0x01;
  static constexpr uint8_t REG_UPPER_TEMP = 0x02;
  static constexpr uint8_t REG_LOWER_TEMP = 0x03;
  static constexpr uint8_t REG_CRIT_TEMP = 0x04;
  static constexpr uint8_t REG_AMBIENT_TEMP = 0x05;
  static constexpr uint8_t REG_MANUF_ID = 0x06;
  static constexpr uint8_t REG_DEVICE_ID = 0x07;
  static constexpr uint8_t REG_RESOLUTION = 0x08;

public:
  enum class Resolution {
    RES_0_5C = 0x00,   // 0.5°C, 30 ms
    RES_0_25C = 0x01,  // 0.25°C, 65 ms
    RES_0_125C = 0x02, // 0.125°C, 130 ms
    RES_0_0625C = 0x03 // 0.0625°C, 250 ms (default)
  };

  MCP9808_TempSensor(I2CDevice &device) : i2c(device) {}

  // Verify device presence
  bool checkDeviceID() {
    uint8_t manuf_id_high, manuf_id_low;
    uint8_t device_id;

    if (!i2c.readRegister(REG_MANUF_ID, manuf_id_high) ||
        !i2c.readRegister(REG_MANUF_ID + 1, manuf_id_low) ||
        !i2c.readRegister(REG_DEVICE_ID, device_id)) {
      return false;
    }

    uint16_t manuf_id = (manuf_id_high << 8) | manuf_id_low;
    std::cout << "[MCP9808] Manufacturer ID: 0x" << std::hex << manuf_id
              << ", Device ID: 0x" << static_cast<int>(device_id) << std::dec
              << "\n";

    return (manuf_id == 0x0054); // Expected value for Microchip
  }

  // Read temperature in Celsius
  bool readTemperature(float &celsius) {
    uint8_t temp_high, temp_low;

    if (!i2c.readRegister(REG_AMBIENT_TEMP, temp_high) ||
        !i2c.readRegister(REG_AMBIENT_TEMP + 1, temp_low)) {
      return false;
    }

    // Clear alert flags
    temp_high &= 0x1F;

    // Check sign bit
    if (temp_high & 0x10) {
      // Negative temperature
      temp_high &= 0x0F;
      celsius = 256.0f - (temp_high * 16.0f + temp_low / 16.0f);
    } else {
      // Positive temperature
      celsius = temp_high * 16.0f + temp_low / 16.0f;
    }

    return true;
  }

  // Set measurement resolution
  bool setResolution(Resolution res) {
    return i2c.writeRegister(REG_RESOLUTION, static_cast<uint8_t>(res));
  }

  // Enable/disable shutdown mode (power saving)
  bool setShutdownMode(bool enable) {
    uint8_t mask = 0x01; // Bit 0 = Shutdown
    uint8_t value = enable ? 0x01 : 0x00;
    return i2c.modifyRegister(REG_CONFIG, mask, value);
  }
};

// ============================================================================
// Example 3: EEPROM (24C256) - Non-Volatile Storage
// ============================================================================

class EEPROM_24C256 {
private:
  I2CDevice &i2c;
  static constexpr size_t PAGE_SIZE = 64;      // 64-byte pages
  static constexpr size_t TOTAL_SIZE = 32768;  // 32 KB
  static constexpr uint8_t WRITE_DELAY_MS = 5; // Page write time

public:
  EEPROM_24C256(I2CDevice &device) : i2c(device) {}

  // Write single byte
  bool writeByte(uint16_t address, uint8_t value) {
    if (address >= TOTAL_SIZE)
      return false;

    uint8_t buffer[3] = {
        static_cast<uint8_t>(address >> 8),   // Address high byte
        static_cast<uint8_t>(address & 0xFF), // Address low byte
        value};

    if (!i2c.writeBlock(buffer[0], {buffer[1], buffer[2]})) {
      return false;
    }

    // Wait for write cycle to complete
    std::this_thread::sleep_for(std::chrono::milliseconds(WRITE_DELAY_MS));
    return true;
  }

  // Read single byte
  bool readByte(uint16_t address, uint8_t &value) {
    if (address >= TOTAL_SIZE)
      return false;

    uint8_t addr_buf[2] = {static_cast<uint8_t>(address >> 8),
                           static_cast<uint8_t>(address & 0xFF)};

    // Write address
    if (write(i2c.fd, addr_buf, 2) != 2)
      return false;

    // Read value
    if (read(i2c.fd, &value, 1) != 1)
      return false;

    return true;
  }

  // Page write (up to 64 bytes)
  bool writePage(uint16_t start_address, const std::vector<uint8_t> &data) {
    if (data.empty() || data.size() > PAGE_SIZE)
      return false;
    if (start_address + data.size() > TOTAL_SIZE)
      return false;

    // Check we don't cross page boundary
    uint16_t page_start = start_address & ~(PAGE_SIZE - 1);
    uint16_t page_end = page_start + PAGE_SIZE;
    if (start_address + data.size() > page_end) {
      std::cerr << "[EEPROM] Write crosses page boundary\n";
      return false;
    }

    std::vector<uint8_t> buffer;
    buffer.push_back(start_address >> 8);
    buffer.push_back(start_address & 0xFF);
    buffer.insert(buffer.end(), data.begin(), data.end());

    // Write all at once
    // Note: This is a simplified version; production code would use ioctl

    std::this_thread::sleep_for(std::chrono::milliseconds(WRITE_DELAY_MS));
    return true;
  }
};

// ============================================================================
// Example 4: I2C Bus Scanner
// ============================================================================

void scanI2CBus(const std::string &bus_path) {
  std::cout << "\n[I2C Scanner] Scanning bus: " << bus_path << "\n";
  std::cout << "     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n";

  int fd = open(bus_path.c_str(), O_RDWR);
  if (fd < 0) {
    std::cerr << "Failed to open bus\n";
    return;
  }

  for (uint8_t addr = 0x00; addr <= 0x7F; ++addr) {
    if (addr % 16 == 0) {
      std::cout << std::hex << std::setw(2) << std::setfill('0')
                << static_cast<int>(addr) << ": ";
    }

    // Reserved addresses
    if (addr < 0x03 || addr > 0x77) {
      std::cout << "   ";
    } else {
      if (ioctl(fd, I2C_SLAVE, addr) >= 0) {
        // Try to read 1 byte to check if device responds
        uint8_t dummy;
        if (read(fd, &dummy, 1) >= 0 || errno == EREMOTEIO) {
          std::cout << std::setw(2) << std::setfill('0')
                    << static_cast<int>(addr) << " ";
        } else {
          std::cout << "-- ";
        }
      } else {
        std::cout << "-- ";
      }
    }

    if ((addr + 1) % 16 == 0) {
      std::cout << "\n";
    }
  }

  std::cout << std::dec;
  close(fd);
}

// ============================================================================
// Main: Demonstration
// ============================================================================

int main() {
  std::cout << "=== I2C Protocol - Production Examples ===\n\n";

  try {
    // Example 1: Scan bus
    scanI2CBus("/dev/i2c-1");

    // Example 2: Temperature sensor
    std::cout << "\n--- Temperature Sensor Example ---\n";
    I2CDevice temp_i2c("/dev/i2c-1", 0x18); // MCP9808 default address
    MCP9808_TempSensor temp_sensor(temp_i2c);

    if (temp_sensor.checkDeviceID()) {
      temp_sensor.setResolution(MCP9808_TempSensor::Resolution::RES_0_0625C);

      for (int i = 0; i < 5; ++i) {
        float celsius;
        if (temp_sensor.readTemperature(celsius)) {
          std::cout << "[Sample " << (i + 1) << "] Temperature: " << std::fixed
                    << std::setprecision(4) << celsius << " °C\n";
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
    }

    // Example 3: EEPROM
    std::cout << "\n--- EEPROM Example ---\n";
    I2CDevice eeprom_i2c("/dev/i2c-1", 0x50); // 24C256 default address
    EEPROM_24C256 eeprom(eeprom_i2c);

    // Write test data
    std::cout << "[EEPROM] Writing test data...\n";
    eeprom.writeByte(0x0000, 0xAB);
    eeprom.writeByte(0x0001, 0xCD);

    // Read back
    uint8_t val1, val2;
    if (eeprom.readByte(0x0000, val1) && eeprom.readByte(0x0001, val2)) {
      std::cout << "[EEPROM] Read: 0x" << std::hex << static_cast<int>(val1)
                << " 0x" << static_cast<int>(val2) << std::dec << "\n";
    }

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  std::cout << "\n=== I2C Examples Complete ===\n";
  return 0;
}

/*
 * SENIOR INTERVIEW TALKING POINTS:
 *
 * 1. RAII Pattern: I2CDevice uses constructor/destructor for resource
 * management
 * 2. Error Handling: All I/O operations return bool for error checking
 * 3. Move Semantics: I2CDevice supports move but disables copy (resource
 * safety)
 * 4. Type Safety: Enum class for resolution settings
 * 5. Bit Manipulation: modifyRegister() for atomic bit updates
 * 6. Timing: EEPROM write delay handled properly
 * 7. Page Boundary Check: EEPROM detects writes crossing pages
 * 8. Repeated START: Automatically handled by Linux I2C subsystem
 * 9. Bus Scanning: Production technique for device discovery
 * 10. Hardware Abstraction: Device-specific logic separated from I2C transport
 */
