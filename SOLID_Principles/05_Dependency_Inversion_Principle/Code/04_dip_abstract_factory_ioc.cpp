/**
 * @file 04_dip_abstract_factory_ioc.cpp
 * @brief Demonstrates Inversion of Control (IoC), Abstract Factory, & The Composition Root Pattern.
 * 
 * Crucial Senior Architecture Disambiguation:
 *   - DIP (Dependency Inversion Principle): The design principle stating high-level modules
 *     must depend on abstractions, not concrete details.
 *   - DI (Dependency Injection): The tactical pattern of passing dependencies into a client
 *     (via constructor, setter, or interface) rather than having the client create them.
 *   - IoC (Inversion of Control): The overarching architectural paradigm where a framework or
 *     composition layer orchestrates control flow and component lifecycles.
 *   - Composition Root: The single location in an application (typically near `main()`) where
 *     the object dependency graph is instantiated and wired together.
 * 
 * @standard C++20
 */

#include <iostream>
#include <string>
#include <memory>
#include <vector>

// ============================================================================
// 1. ABSTRACT COMPONENTS
// ============================================================================
class ITransportChannel {
public:
    virtual ~ITransportChannel() = default;
    virtual void send(const std::string& packet) = 0;
};

class ITelemetrySink {
public:
    virtual ~ITelemetrySink() = default;
    virtual void recordEvent(const std::string& event) = 0;
};

// ============================================================================
// 2. ABSTRACT FACTORY: Decouples Component Creation from Usage
// ============================================================================
class IInfrastructureFactory {
public:
    virtual ~IInfrastructureFactory() = default;
    virtual std::unique_ptr<ITransportChannel> createTransport() const = 0;
    virtual std::unique_ptr<ITelemetrySink> createTelemetry() const = 0;
};

// ============================================================================
// 3. HIGH-LEVEL DOMAIN CLIENT (Zero knowledge of concrete factories/classes)
// ============================================================================
class TradingGateway {
private:
    std::unique_ptr<ITransportChannel> transport_;
    std::unique_ptr<ITelemetrySink> telemetry_;

public:
    // Pure Constructor Injection via Move Semantics (Exclusive Ownership)
    TradingGateway(std::unique_ptr<ITransportChannel> transport,
                   std::unique_ptr<ITelemetrySink> telemetry) noexcept
        : transport_(std::move(transport)), telemetry_(std::move(telemetry)) {}

    void executeOrder(const std::string& orderDetails) {
        telemetry_->recordEvent("Order execution initiated: " + orderDetails);
        transport_->send("FIX.4.4:ORDER_NEW:" + orderDetails);
        telemetry_->recordEvent("Order dispatched to matching engine.");
    }
};

// ============================================================================
// 4. CONCRETE INFRASTRUCTURE SUITES (Production vs Simulator)
// ============================================================================

// --- Production Implementation Suite ---
class TcpSocketTransport : public ITransportChannel {
public:
    void send(const std::string& packet) override {
        std::cout << "  -> [Production TCP] Sent to Exchange Gateway: " << packet << "\n";
    }
};

class PrometheusTelemetrySink : public ITelemetrySink {
public:
    void recordEvent(const std::string& event) override {
        std::cout << "  -> [Prometheus TimeSeries] Logged metric: " << event << "\n";
    }
};

class ProductionInfrastructureFactory : public IInfrastructureFactory {
public:
    std::unique_ptr<ITransportChannel> createTransport() const override {
        return std::make_unique<TcpSocketTransport>();
    }
    std::unique_ptr<ITelemetrySink> createTelemetry() const override {
        return std::make_unique<PrometheusTelemetrySink>();
    }
};

// --- Offline Simulator Suite ---
class LoopbackTransport : public ITransportChannel {
public:
    void send(const std::string& packet) override {
        std::cout << "  -> [Simulator Loopback] Captured in-memory: " << packet << "\n";
    }
};

class ConsoleTelemetrySink : public ITelemetrySink {
public:
    void recordEvent(const std::string& event) override {
        std::cout << "  -> [Console Debug] " << event << "\n";
    }
};

class SimulationInfrastructureFactory : public IInfrastructureFactory {
public:
    std::unique_ptr<ITransportChannel> createTransport() const override {
        return std::make_unique<LoopbackTransport>();
    }
    std::unique_ptr<ITelemetrySink> createTelemetry() const override {
        return std::make_unique<ConsoleTelemetrySink>();
    }
};

// ============================================================================
// 5. THE COMPOSITION ROOT
// The sole place where concrete dependencies are assembled!
// ============================================================================
std::unique_ptr<TradingGateway> buildGateway(const IInfrastructureFactory& factory) {
    return std::make_unique<TradingGateway>(
        factory.createTransport(),
        factory.createTelemetry()
    );
}

int main() {
    std::cout << "=== DIP Architectural Pattern: Composition Root & Abstract Factory ===\n\n";

    // Scenario A: Production Deployment
    std::cout << "--- Initializing Production Environment ---\n";
    ProductionInfrastructureFactory prodFactory;
    auto prodGateway = buildGateway(prodFactory);
    prodGateway->executeOrder("BUY 100 GOOG @ 185.50");

    // Scenario B: Offline Simulation
    std::cout << "\n--- Initializing Simulation / Backtest Environment ---\n";
    SimulationInfrastructureFactory simFactory;
    auto simGateway = buildGateway(simFactory);
    simGateway->executeOrder("SELL 500 AAPL @ 220.00");

    std::cout << "\nTradingGateway logic remained 100% identical and decoupled across both environments!\n";
    return 0;
}
