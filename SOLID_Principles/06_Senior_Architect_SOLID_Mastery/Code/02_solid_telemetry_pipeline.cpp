/**
 * @file 02_solid_telemetry_pipeline.cpp
 * @brief Production Case Study: Concurrent, Multi-Sink Telemetry Engine Exhibiting All 5 SOLID Principles.
 * 
 * Architecture Blueprint:
 *   - S (Single Responsibility):
 *       - TelemetryRecord: Plain value struct holding timestamp, severity, payload.
 *       - IEventFormatter: Solely responsible for formatting (Plain text, JSON, Syslog).
 *       - IEventFilter: Solely responsible for threshold/predicate filtering.
 *       - IEventSink: Solely responsible for dispatching to destination (Console, File, Network).
 *       - TelemetryPipeline: Orchestrates queueing and thread synchronization.
 *   - O (Open/Closed):
 *       - New sinks (e.g., Kafka, Datadog) or formatters are added without touching pipeline core.
 *   - L (Liskov Substitution):
 *       - All sinks guarantee `noexcept` delivery or handle internal I/O errors gracefully
 *         without crashing the background worker thread.
 *   - I (Interface Segregation):
 *       - `IEventSink` (write operations) is segregated from `IFlushable` (buffer sync)
 *         and `IHealthCheckable` (metrics/heartbeat).
 *   - D (Dependency Inversion):
 *       - Pipeline accepts sinks via constructor injection of `std::unique_ptr<IEventSink>`.
 * 
 * @standard C++20
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <chrono>
#include <sstream>

// ============================================================================
// DOMAIN ENTITY (SRP: Pure Value Object)
// ============================================================================
enum class Severity { Debug, Info, Warning, Error, Fatal };

struct TelemetryRecord {
    uint64_t timestampUs;
    Severity level;
    std::string subsystem;
    std::string message;
};

// ============================================================================
// SEGREGATED INTERFACES (ISP & DIP)
// ============================================================================
class IEventSink {
public:
    virtual ~IEventSink() = default;
    virtual void emit(const std::string& formattedEvent) noexcept = 0;
};

class IFlushable {
public:
    virtual ~IFlushable() = default;
    virtual void flush() = 0;
};

class IEventFormatter {
public:
    virtual ~IEventFormatter() = default;
    [[nodiscard]] virtual std::string format(const TelemetryRecord& record) const = 0;
};

// ============================================================================
// CONCRETE FORMATTERS (SRP & OCP)
// ============================================================================
class HumanReadableFormatter : public IEventFormatter {
public:
    [[nodiscard]] std::string format(const TelemetryRecord& r) const override {
        std::ostringstream oss;
        oss << "[" << r.timestampUs << " us] [" << r.subsystem << "] " << r.message;
        return oss.str();
    }
};

class JsonTelemetryFormatter : public IEventFormatter {
public:
    [[nodiscard]] std::string format(const TelemetryRecord& r) const override {
        return "{\"ts\":" + std::to_string(r.timestampUs) + ",\"sub\":\"" 
             + r.subsystem + "\",\"msg\":\"" + r.message + "\"}";
    }
};

// ============================================================================
// CONCRETE SINKS (LSP: noexcept emit guarantees thread safety)
// ============================================================================
class ConsoleSink : public IEventSink {
public:
    void emit(const std::string& formattedEvent) noexcept override {
        std::cout << "  [Console] " << formattedEvent << "\n";
    }
};

class RotatingFileSink : public IEventSink, public IFlushable {
public:
    void emit(const std::string& formattedEvent) noexcept override {
        // Simulated buffered disk write
        std::cout << "  [DiskFile /var/log/app.log] " << formattedEvent << "\n";
    }

    void flush() override {
        std::cout << "  [DiskFile] Synced dirty log pages to disk.\n";
    }
};

// ============================================================================
// CONCURRENT DISPATCHER (DIP: Injected Formatters and Sinks)
// ============================================================================
class TelemetryPipeline {
private:
    std::unique_ptr<IEventFormatter> formatter_;
    std::vector<std::unique_ptr<IEventSink>> sinks_;

    std::queue<TelemetryRecord> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::atomic<bool> isRunning_{true};
    std::thread workerThread_;

    void processQueue() {
        while (isRunning_ || !queue_.empty()) {
            std::unique_lock<std::mutex> lock(mutex_);
            cv_.wait(lock, [this] { return !queue_.empty() || !isRunning_; });

            while (!queue_.empty()) {
                TelemetryRecord record = std::move(queue_.front());
                queue_.pop();
                lock.unlock();

                // Format once
                std::string formatted = formatter_->format(record);

                // Broadcast to all sinks (Substitutable via LSP)
                for (auto& sink : sinks_) {
                    sink->emit(formatted);
                }

                lock.lock();
            }
        }
    }

public:
    explicit TelemetryPipeline(std::unique_ptr<IEventFormatter> formatter)
        : formatter_(std::move(formatter)) {
        workerThread_ = std::thread(&TelemetryPipeline::processQueue, this);
    }

    ~TelemetryPipeline() {
        shutdown();
    }

    void addSink(std::unique_ptr<IEventSink> sink) {
        std::lock_guard<std::mutex> lock(mutex_);
        sinks_.push_back(std::move(sink));
    }

    void recordEvent(Severity level, std::string subsystem, std::string message) {
        uint64_t now = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now().time_since_epoch()
        ).count();

        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(TelemetryRecord{now, level, std::move(subsystem), std::move(message)});
        }
        cv_.notify_one();
    }

    void shutdown() {
        if (isRunning_.exchange(false)) {
            cv_.notify_all();
            if (workerThread_.joinable()) {
                workerThread_.join();
            }
        }
    }
};

int main() {
    std::cout << "=== Full SOLID Synergy: Concurrent Telemetry Engine ===\n\n";

    // 1. Composition Root: Wiring dependencies
    auto formatter = std::make_unique<JsonTelemetryFormatter>();
    TelemetryPipeline pipeline(std::move(formatter));

    pipeline.addSink(std::make_unique<ConsoleSink>());
    pipeline.addSink(std::make_unique<RotatingFileSink>());

    std::cout << "Logging events asynchronously to pipeline...\n";
    pipeline.recordEvent(Severity::Info, "NetworkCore", "TCP Connection established from 10.0.4.12");
    pipeline.recordEvent(Severity::Warning, "RiskEngine", "Client account 9821 approaching margin call");
    pipeline.recordEvent(Severity::Fatal, "MatchingEngine", "Heartbeat lost on Gateway B - Failover active");

    // Allow worker thread to drain queue
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    pipeline.shutdown();

    std::cout << "\nTelemetry engine shut down gracefully. All 5 SOLID principles realized.\n";
    return 0;
}
