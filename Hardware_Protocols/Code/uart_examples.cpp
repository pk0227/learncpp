/*
 * UART Protocol - Production-Grade C++ Examples
 * For 7+ Years Embedded C++ Developer Interviews
 *
 * Compile: g++ -std=c++20 -Wall -Wextra -o uart_examples uart_examples.cpp
 * Run: ./uart_examples
 */

#include <chrono>
#include <cstring>
#include <fcntl.h>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/ioctl.h>
#include <termios.h>
#include <thread>
#include <unistd.h>
#include <vector>

// ============================================================================
// Example 1: Basic UART Wrapper Class (termios Configuration)
// ============================================================================

class UARTDevice {
private:
  int fd{-1};
  std::string port_path;

public:
  enum class BaudRate {
    BAUD_9600 = B9600,
    BAUD_19200 = B19200,
    BAUD_38400 = B38400,
    BAUD_57600 = B57600,
    BAUD_115200 = B115200,
    BAUD_230400 = B230400,
    BAUD_460800 = B460800,
    BAUD_921600 = B921600
  };

  enum class Parity { NONE, EVEN, ODD };

  UARTDevice(const std::string &port, BaudRate baud = BaudRate::BAUD_115200,
             uint8_t data_bits = 8, uint8_t stop_bits = 1,
             Parity parity = Parity::NONE)
      : port_path(port) {

    // Open port
    fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0) {
      throw std::runtime_error("Failed to open UART port: " + port + " (" +
                               strerror(errno) + ")");
    }

    // Configure port
    struct termios options;
    if (tcgetattr(fd, &options) < 0) {
      close(fd);
      throw std::runtime_error("Failed to get UART attributes");
    }

    // Set baud rate
    cfsetispeed(&options, static_cast<speed_t>(baud));
    cfsetospeed(&options, static_cast<speed_t>(baud));

    // Data bits
    options.c_cflag &= ~CSIZE;
    switch (data_bits) {
    case 5:
      options.c_cflag |= CS5;
      break;
    case 6:
      options.c_cflag |= CS6;
      break;
    case 7:
      options.c_cflag |= CS7;
      break;
    case 8:
      options.c_cflag |= CS8;
      break;
    default:
      options.c_cflag |= CS8;
    }

    // Stop bits
    if (stop_bits == 2) {
      options.c_cflag |= CSTOPB;
    } else {
      options.c_cflag &= ~CSTOPB;
    }

    // Parity
    switch (parity) {
    case Parity::EVEN:
      options.c_cflag |= PARENB;
      options.c_cflag &= ~PARODD;
      break;
    case Parity::ODD:
      options.c_cflag |= PARENB;
      options.c_cflag |= PARODD;
      break;
    case Parity::NONE:
    default:
      options.c_cflag &= ~PARENB;
      break;
    }

    // Hardware control
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~CRTSCTS; // Disable hardware flow control

    // Raw mode
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    options.c_iflag &=
        ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
    options.c_oflag &= ~OPOST;

    // Read timeout (deciseconds)
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 10; // 1 second timeout

    // Apply settings
    if (tcsetattr(fd, TCSANOW, &options) < 0) {
      close(fd);
      throw std::runtime_error("Failed to set UART attributes");
    }

    // Flush buffers
    tcflush(fd, TCIOFLUSH);

    std::cout << "[UART] Opened " << port << " @ " << getBaudRateValue(baud)
              << " baud, " << static_cast<int>(data_bits)
              << parityToChar(parity) << static_cast<int>(stop_bits) << "\n";
  }

  ~UARTDevice() {
    if (fd >= 0) {
      close(fd);
      std::cout << "[UART] Closed " << port_path << "\n";
    }
  }

  // Delete copy, allow move
  UARTDevice(const UARTDevice &) = delete;
  UARTDevice &operator=(const UARTDevice &) = delete;
  UARTDevice(UARTDevice &&other) noexcept
      : fd(other.fd), port_path(std::move(other.port_path)) {
    other.fd = -1;
  }

  // Write data
  ssize_t write(const std::vector<uint8_t> &data) {
    ssize_t n = ::write(fd, data.data(), data.size());
    if (n < 0) {
      std::cerr << "[UART] Write failed: " << strerror(errno) << "\n";
    }
    return n;
  }

  // Write string
  ssize_t write(const std::string &str) {
    ssize_t n = ::write(fd, str.c_str(), str.length());
    if (n < 0) {
      std::cerr << "[UART] Write failed: " << strerror(errno) << "\n";
    }
    return n;
  }

  // Read data (non-blocking, returns immediately)
  ssize_t read(std::vector<uint8_t> &buffer, size_t max_bytes) {
    buffer.resize(max_bytes);
    ssize_t n = ::read(fd, buffer.data(), max_bytes);
    if (n > 0) {
      buffer.resize(n);
    } else if (n < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
      std::cerr << "[UART] Read failed: " << strerror(errno) << "\n";
    }
    return n;
  }

  // Read line (until delimiter)
  bool readLine(std::string &line, char delimiter = '\n',
                size_t max_bytes = 256) {
    line.clear();
    char c;

    for (size_t i = 0; i < max_bytes; ++i) {
      ssize_t n = ::read(fd, &c, 1);
      if (n == 1) {
        if (c == delimiter) {
          return true;
        }
        if (c != '\r') { // Ignore CR
          line += c;
        }
      } else if (n == 0) {
        // Timeout or no data
        if (!line.empty()) {
          return true; // Partial line
        }
        return false;
      } else if (n < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
        return false;
      }
    }
    return !line.empty();
  }

  // Read with timeout
  ssize_t readWithTimeout(std::vector<uint8_t> &buffer, size_t max_bytes,
                          uint32_t timeout_ms) {
    auto start = std::chrono::steady_clock::now();
    buffer.clear();

    while (buffer.size() < max_bytes) {
      uint8_t byte;
      ssize_t n = ::read(fd, &byte, 1);

      if (n == 1) {
        buffer.push_back(byte);
      } else {
        auto now = std::chrono::steady_clock::now();
        auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(now - start);

        if (elapsed.count() > timeout_ms) {
          break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }
    }

    return buffer.size();
  }

  // Check bytes available
  int bytesAvailable() {
    int bytes = 0;
    ioctl(fd, FIONREAD, &bytes);
    return bytes;
  }

  // Flush buffers
  void flush() { tcflush(fd, TCIOFLUSH); }

  // Send break signal
  void sendBreak(int duration_ms = 250) {
    tcsendbreak(fd, duration_ms / 100); // duration in 0.1s units
  }

private:
  static uint32_t getBaudRateValue(BaudRate baud) {
    switch (baud) {
    case BaudRate::BAUD_9600:
      return 9600;
    case BaudRate::BAUD_19200:
      return 19200;
    case BaudRate::BAUD_38400:
      return 38400;
    case BaudRate::BAUD_57600:
      return 57600;
    case BaudRate::BAUD_115200:
      return 115200;
    case BaudRate::BAUD_230400:
      return 230400;
    case BaudRate::BAUD_460800:
      return 460800;
    case BaudRate::BAUD_921600:
      return 921600;
    default:
      return 0;
    }
  }

  static char parityToChar(Parity p) {
    switch (p) {
    case Parity::EVEN:
      return 'E';
    case Parity::ODD:
      return 'O';
    case Parity::NONE:
    default:
      return 'N';
    }
  }
};

// ============================================================================
// Example 2: GPS Module (NMEA Parser)
// ============================================================================

class GPS_NMEA {
private:
  UARTDevice &uart;

public:
  struct GPSData {
    double latitude{0.0};
    double longitude{0.0};
    double altitude{0.0};
    int satellites{0};
    std::string time;
    bool valid{false};
  };

  GPS_NMEA(UARTDevice &device) : uart(device) {}

  // Read and parse NMEA sentence
  bool readSentence(std::string &sentence) {
    // NMEA sentences start with $ and end with \r\n
    std::string line;
    if (uart.readLine(line, '\n', 256)) {
      if (!line.empty() && line[0] == '$') {
        sentence = line;
        return true;
      }
    }
    return false;
  }

  // Parse GGA sentence (Global Positioning System Fix Data)
  bool parseGGA(const std::string &sentence, GPSData &data) {
    if (sentence.find("$GPGGA") != 0 && sentence.find("$GNGGA") != 0) {
      return false;
    }

    // Example:
    // $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
    std::vector<std::string> fields = split(sentence, ',');

    if (fields.size() < 15) {
      return false;
    }

    // Field 1: Time (HHMMSS.sss)
    data.time = fields[1];

    // Fields 2-3: Latitude (DDMM.MMMM, N/S)
    if (!fields[2].empty() && !fields[3].empty()) {
      data.latitude = parseCoordinate(fields[2], fields[3][0]);
    }

    // Fields 4-5: Longitude (DDDMM.MMMM, E/W)
    if (!fields[4].empty() && !fields[5].empty()) {
      data.longitude = parseCoordinate(fields[4], fields[5][0]);
    }

    // Field 6: Fix quality (0 = invalid, 1 = GPS fix, 2 = DGPS fix, etc.)
    data.valid = (!fields[6].empty() && fields[6][0] > '0');

    // Field 7: Number of satellites
    if (!fields[7].empty()) {
      data.satellites = std::stoi(fields[7]);
    }

    // Field 9: Altitude
    if (!fields[9].empty()) {
      data.altitude = std::stod(fields[9]);
    }

    return data.valid;
  }

private:
  std::vector<std::string> split(const std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;

    for (char c : str) {
      if (c == delimiter) {
        tokens.push_back(token);
        token.clear();
      } else if (c != '\r' && c != '\n') {
        token += c;
      }
    }

    if (!token.empty()) {
      tokens.push_back(token);
    }

    return tokens;
  }

  double parseCoordinate(const std::string &coord, char direction) {
    if (coord.empty())
      return 0.0;

    // DDMM.MMMM or DDDMM.MMMM format
    size_t dot_pos = coord.find('.');
    if (dot_pos == std::string::npos || dot_pos < 2)
      return 0.0;

    // Extract degrees and minutes
    int degree_digits = (coord.length() > 10) ? 3 : 2;
    int degrees = std::stoi(coord.substr(0, degree_digits));
    double minutes = std::stod(coord.substr(degree_digits));

    // Convert to decimal degrees
    double decimal = degrees + (minutes / 60.0);

    // Apply direction
    if (direction == 'S' || direction == 'W') {
      decimal = -decimal;
    }

    return decimal;
  }
};

// ============================================================================
// Example 3: AT Command Interface (Bluetooth/WiFi Modules)
// ============================================================================

class ATCommandDevice {
private:
  UARTDevice &uart;
  static constexpr uint32_t DEFAULT_TIMEOUT_MS = 1000;

public:
  ATCommandDevice(UARTDevice &device) : uart(device) {}

  // Send AT command and wait for response
  bool sendCommand(const std::string &command, std::string &response,
                   uint32_t timeout_ms = DEFAULT_TIMEOUT_MS) {
    // Clear buffers
    uart.flush();

    // Send command with \r\n
    std::string cmd = command + "\r\n";
    if (uart.write(cmd) < 0) {
      return false;
    }

    std::cout << "[AT] Sent: " << command << "\n";

    // Read response
    auto start = std::chrono::steady_clock::now();
    response.clear();
    std::string line;

    while (true) {
      if (uart.readLine(line, '\n', 256)) {
        // Skip echo of command
        if (line == command) {
          continue;
        }

        response += line + "\n";

        // Check for final response
        if (line.find("OK") == 0 || line.find("ERROR") == 0 ||
            line.find(">") == 0) {
          std::cout << "[AT] Response: " << line << "\n";
          return (line.find("OK") == 0);
        }
      }

      auto now = std::chrono::steady_clock::now();
      auto elapsed =
          std::chrono::duration_cast<std::chrono::milliseconds>(now - start);

      if (elapsed.count() > timeout_ms) {
        std::cerr << "[AT] Timeout waiting for response\n";
        return false;
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  }

  // Test communication
  bool test() {
    std::string response;
    return sendCommand("AT", response);
  }

  // Get version
  bool getVersion(std::string &version) {
    return sendCommand("AT+GMR", version);
  }
};

// ============================================================================
// Example 4: Modbus RTU (Industrial Protocol)
// ============================================================================

class ModbusRTU {
private:
  UARTDevice &uart;

  // Calculate CRC16 for Modbus
  uint16_t calculateCRC(const std::vector<uint8_t> &data) {
    uint16_t crc = 0xFFFF;

    for (uint8_t byte : data) {
      crc ^= byte;
      for (int i = 0; i < 8; ++i) {
        if (crc & 0x0001) {
          crc >>= 1;
          crc ^= 0xA001;
        } else {
          crc >>= 1;
        }
      }
    }

    return crc;
  }

public:
  ModbusRTU(UARTDevice &device) : uart(device) {}

  // Read holding registers (Function code 0x03)
  bool readHoldingRegisters(uint8_t slave_id, uint16_t start_addr,
                            uint16_t count, std::vector<uint16_t> &values) {
    if (count > 125)
      return false;

    // Build request
    std::vector<uint8_t> request = {slave_id,
                                    0x03, // Function code
                                    static_cast<uint8_t>(start_addr >> 8),
                                    static_cast<uint8_t>(start_addr & 0xFF),
                                    static_cast<uint8_t>(count >> 8),
                                    static_cast<uint8_t>(count & 0xFF)};

    // Add CRC
    uint16_t crc = calculateCRC(request);
    request.push_back(crc & 0xFF);
    request.push_back(crc >> 8);

    // Send request
    uart.flush();
    if (uart.write(request) < 0) {
      return false;
    }

    // Wait for response (slave processes, then responds)
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Read response
    std::vector<uint8_t> response;
    uart.readWithTimeout(response, 5 + count * 2, 500);

    if (response.size() < 5) {
      std::cerr << "[Modbus] Response too short\n";
      return false;
    }

    // Verify CRC
    std::vector<uint8_t> response_data(response.begin(), response.end() - 2);
    uint16_t expected_crc = calculateCRC(response_data);
    uint16_t received_crc =
        response[response.size() - 2] | (response[response.size() - 1] << 8);

    if (expected_crc != received_crc) {
      std::cerr << "[Modbus] CRC mismatch\n";
      return false;
    }

    // Extract data
    uint8_t byte_count = response[2];
    values.clear();

    for (size_t i = 0; i < byte_count / 2; ++i) {
      uint16_t value = (response[3 + i * 2] << 8) | response[4 + i * 2];
      values.push_back(value);
    }

    return true;
  }
};

// ============================================================================
// Main: Demonstration
// ============================================================================

int main() {
  std::cout << "=== UART Protocol - Production Examples ===\n\n";

  try {
    // Example 1: GPS Module
    std::cout << "--- GPS Module Example (NMEA) ---\n";
    UARTDevice gps_uart("/dev/ttyUSB0", UARTDevice::BaudRate::BAUD_9600);
    GPS_NMEA gps(gps_uart);

    std::cout << "[GPS] Waiting for NMEA sentences...\n";
    for (int i = 0; i < 5; ++i) {
      std::string sentence;
      if (gps.readSentence(sentence)) {
        std::cout << "[GPS] Sentence: " << sentence << "\n";

        GPS_NMEA::GPSData data;
        if (gps.parseGGA(sentence, data)) {
          std::cout << "  Lat: " << data.latitude << ", Lon: " << data.longitude
                    << ", Alt: " << data.altitude << "m"
                    << ", Sats: " << data.satellites << "\n";
        }
      }
    }

    // Example 2: AT Commands (Bluetooth/WiFi)
    std::cout << "\n--- AT Command Example (ESP32/HC-05) ---\n";
    UARTDevice at_uart("/dev/ttyUSB1", UARTDevice::BaudRate::BAUD_115200);
    ATCommandDevice at(at_uart);

    if (at.test()) {
      std::cout << "[AT] Module responding\n";

      std::string version;
      if (at.getVersion(version)) {
        std::cout << "[AT] Version info:\n" << version;
      }
    }

    // Example 3: Modbus RTU
    std::cout << "\n--- Modbus RTU Example ---\n";
    UARTDevice modbus_uart("/dev/ttyUSB2", UARTDevice::BaudRate::BAUD_9600, 8,
                           1, UARTDevice::Parity::EVEN);
    ModbusRTU modbus(modbus_uart);

    std::vector<uint16_t> registers;
    if (modbus.readHoldingRegisters(1, 0, 10, registers)) {
      std::cout << "[Modbus] Read 10 registers from slave 1:\n";
      for (size_t i = 0; i < registers.size(); ++i) {
        std::cout << "  Reg[" << i << "] = " << registers[i] << "\n";
      }
    }

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  std::cout << "\n=== UART Examples Complete ===\n";
  return 0;
}

/*
 * SENIOR INTERVIEW TALKING POINTS:
 *
 * 1. termios Configuration: Proper 8N1 setup with raw mode
 * 2. Baud Rate Matching: Both sides must agree exactly
 * 3. Timeout Handling: VTIME for read timeout
 * 4. Line-Based Protocols: readLine() handles \n, \r\n, \r
 * 5. NMEA Parsing: Real GPS sentence parsing with coordinate conversion
 * 6. AT Commands: Request-response pattern with timeout
 * 7. Modbus RTU: CRC16 calculation for data integrity
 * 8. Flow Control: Can add RTS/CTS with CRTSCTS flag
 * 9. Error Detection: No built-in ACK, use parity + application CRC
 * 10. Buffer Flushing: tcflush() to clear stale data
 */
