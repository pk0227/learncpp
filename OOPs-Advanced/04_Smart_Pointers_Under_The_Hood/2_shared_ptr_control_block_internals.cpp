/**
 * @file 2_shared_ptr_control_block_internals.cpp
 * @brief Demonstrates std::shared_ptr Control Block and Aliasing Constructor:
 *        - Two-pointer footprint (raw pointer + control block pointer).
 *        - Atomic reference counting lifecycle.
 *        - The aliasing constructor keeping parent object alive while exposing a member.
 */

#include <iostream>
#include <memory>
#include <string>

struct SensorData {
    std::string device_name;
    int reading;

    SensorData(std::string name, int val) 
        : device_name(std::move(name)), reading(val) {
        std::cout << "  SensorData [\"" << device_name << "\"] allocated.\n";
    }

    ~SensorData() {
        std::cout << "  SensorData [\"" << device_name << "\"] DEALLOCATED!\n";
    }
};

int main() {
    std::cout << "=== std::shared_ptr Physical Layout ===\n";
    std::shared_ptr<int> sp_test;
    std::cout << "sizeof(std::shared_ptr<T>): " << sizeof(sp_test) 
              << " bytes (Exactly 2 pointers: Raw Resource Ptr + Control Block Ptr)\n\n";

    std::cout << "=== Aliasing Constructor Mechanics ===\n";
    std::shared_ptr<int> sp_reading_only;

    {
        // 1. Allocate complete SensorData object
        auto sp_sensor = std::make_shared<SensorData>("LIDAR_Front", 1024);
        std::cout << "sp_sensor use_count: " << sp_sensor.use_count() << "\n";

        // 2. Aliasing Constructor: Owns sp_sensor's control block, but points to &reading!
        sp_reading_only = std::shared_ptr<int>(sp_sensor, &sp_sensor->reading);
        std::cout << "After aliasing constructor:\n";
        std::cout << "  sp_sensor use_count:       " << sp_sensor.use_count() << "\n";
        std::cout << "  sp_reading_only use_count: " << sp_reading_only.use_count() << "\n";
        std::cout << "  sp_reading_only value:     " << *sp_reading_only << "\n";

        std::cout << "Exiting inner scope (sp_sensor goes out of scope)...\n";
    }

    std::cout << "In outer scope: sp_sensor is gone, but sp_reading_only remains alive!\n";
    std::cout << "sp_reading_only use_count: " << sp_reading_only.use_count() << "\n";
    std::cout << "sp_reading_only value:     " << *sp_reading_only << "\n";
    std::cout << "Notice that SensorData was NOT deallocated yet!\n\n";

    std::cout << "Resetting sp_reading_only:\n";
    sp_reading_only.reset(); // SensorData deallocates HERE!

    return 0;
}
