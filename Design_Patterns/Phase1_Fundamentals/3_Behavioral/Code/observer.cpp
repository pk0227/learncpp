/**
 * @file observer.cpp
 * @brief Observer Pattern
 *
 * SCENARIO:
 * Weather Station (Subject).
 * PhoneDisplay, WindowDisplay (Observers).
 * When temperature changes, all displays update automatically.
 */

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// ===================================
// 1. Observer Interface
// ===================================
class IObserver {
public:
  virtual ~IObserver() = default;
  virtual void update(float temp, float humidity) = 0;
};

// ===================================
// 2. Subject Interface
// ===================================
class ISubject {
public:
  virtual ~ISubject() = default;
  virtual void attach(IObserver *observer) = 0;
  virtual void detach(IObserver *observer) = 0;
  virtual void notify() = 0;
};

// ===================================
// 3. Concrete Subject
// ===================================
class WeatherData : public ISubject {
  std::vector<IObserver *> observers_;
  float temp_ = 0.0f;
  float humidity_ = 0.0f;

public:
  void attach(IObserver *observer) override { observers_.push_back(observer); }

  void detach(IObserver *observer) override {
    // std::erase(observers_, observer); // C++20 standard way
    // Manual way for portability:
    auto it = std::remove(observers_.begin(), observers_.end(), observer);
    observers_.erase(it, observers_.end());
  }

  void notify() override {
    for (auto *obs : observers_) {
      obs->update(temp_, humidity_);
    }
  }

  // Business Logic
  void setMeasurements(float temp, float humidity) {
    temp_ = temp;
    humidity_ = humidity;
    std::cout << "\n[Weather Station] New Data: " << temp << "C, " << humidity
              << "% Humidity\n";
    notify();
  }
};

// ===================================
// 4. Concrete Observers
// ===================================
class PhoneDisplay : public IObserver {
public:
  void update(float temp, float humidity) override {
    std::cout << "  [Phone] Notification: It is now " << temp << "C\n";
  }
};

class WindowDisplay : public IObserver {
public:
  void update(float temp, float humidity) override {
    // Window display might only care about humidity
    if (humidity > 80.0f) {
      std::cout << "  [Window] Alert: Close windows! It's raining soon.\n";
    } else {
      std::cout << "  [Window] Updates received.\n";
    }
  }
};

// ===================================
// DEMONSTRATION
// ===================================
int main() {
  std::cout << "--- Observer Pattern Demo ---\n";

  WeatherData weatherStation;

  PhoneDisplay myPhone;
  WindowDisplay myWindow;

  // Subscribe
  weatherStation.attach(&myPhone);
  weatherStation.attach(&myWindow);

  // Weather changes
  weatherStation.setMeasurements(25.0f, 60.0f);
  weatherStation.setMeasurements(22.0f, 90.0f);

  // Unsubscribe Phone
  std::cout << "\n(Phone detached...)\n";
  weatherStation.detach(&myPhone);

  // Weather changes again
  weatherStation.setMeasurements(18.0f, 95.0f);

  return 0;
}
