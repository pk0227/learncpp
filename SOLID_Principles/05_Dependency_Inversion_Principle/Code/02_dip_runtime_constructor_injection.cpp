/**
 * @file 02_dip_runtime_constructor_injection.cpp
 * @brief Demonstrates proper Runtime Dependency Inversion Principle with Constructor Injection & Mocks.
 * 
 * Inverted Architecture:
 *   - Both High-Level (`OrderProcessingService`) and Low-Level (`PostgresRepository`, `AwsSesNotifier`)
 *     depend on pure abstract interfaces (`IDatabaseRepository`, `INotificationService`).
 * 
 * Modern C++ Ownership Considerations:
 *   - Non-Owning Dependencies (`T&`): Preferred when dependencies outlive the service (e.g., connection pools).
 *   - Exclusive Ownership (`std::unique_ptr<T>`): Preferred when the service owns the lifecycle.
 *   - Shared Ownership (`std::shared_ptr<T>`): Used ONLY when multiple asynchronous threads truly share lifetime.
 * 
 * The Architectural Victory: Hermetic Unit Testing
 *   - We can instantiate `MockDatabase` and `MockNotifier` in test suites to verify business logic
 *     with 100% deterministic assertions, zero network sockets, and sub-millisecond execution.
 * 
 * @standard C++20
 */

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <cassert>

// ============================================================================
// 1. ABSTRACTIONS (Neither high-level nor low-level owns these)
// ============================================================================
class IDatabaseRepository {
public:
    virtual ~IDatabaseRepository() = default;
    virtual void insertRecord(const std::string& table, const std::string& record) = 0;
};

class INotificationService {
public:
    virtual ~INotificationService() = default;
    virtual void notifyUser(const std::string& recipient, const std::string& message) = 0;
};

// ============================================================================
// 2. HIGH-LEVEL MODULE: Inverted Dependency on Pure Abstractions
// ============================================================================
class OrderProcessingService {
private:
    // Non-owning borrowed references: Explicit lifetime dependency via Constructor Injection
    IDatabaseRepository& db_;
    INotificationService& notifier_;

public:
    OrderProcessingService(IDatabaseRepository& db, INotificationService& notifier) noexcept
        : db_(db), notifier_(notifier) {}

    bool processOrder(const std::string& orderId, const std::string& email, double amount) {
        std::cout << "[Service] Processing order " << orderId << " for $" << amount << "\n";

        if (amount <= 0.0) {
            std::cout << "[Service] Rejected invalid amount.\n";
            return false;
        }

        db_.insertRecord("orders", "id=" + orderId + ",amount=" + std::to_string(amount));
        notifier_.notifyUser(email, "Confirmation for " + orderId);
        return true;
    }
};

// ============================================================================
// 3. LOW-LEVEL PRODUCTION DETAILS (Implement the Abstractions)
// ============================================================================
class PostgresRepository : public IDatabaseRepository {
public:
    void insertRecord(const std::string& table, const std::string& record) override {
        std::cout << "  -> [Postgres Prod] INSERT INTO " << table << " (" << record << ");\n";
    }
};

class AwsSesNotifier : public INotificationService {
public:
    void notifyUser(const std::string& recipient, const std::string& message) override {
        std::cout << "  -> [AWS SES Prod] Sending HTTPS REST email to " << recipient << ": " << message << "\n";
    }
};

// ============================================================================
// 4. TEST MOCKS (Enables 100% Hermetic Unit Testing)
// ============================================================================
class MockDatabase : public IDatabaseRepository {
public:
    std::vector<std::pair<std::string, std::string>> insertedRecords;

    void insertRecord(const std::string& table, const std::string& record) override {
        insertedRecords.emplace_back(table, record);
    }
};

class MockNotifier : public INotificationService {
public:
    std::vector<std::pair<std::string, std::string>> sentNotifications;

    void notifyUser(const std::string& recipient, const std::string& message) override {
        sentNotifications.emplace_back(recipient, message);
    }
};

// ============================================================================
// DEMO
// ============================================================================
int main() {
    std::cout << "=== DIP Runtime Constructor Injection & Hermetic Testing ===\n\n";

    // 1. Production Execution (Composition Root)
    std::cout << "--- 1. Production Deployment ---\n";
    PostgresRepository prodDb;
    AwsSesNotifier prodNotifier;

    OrderProcessingService prodService(prodDb, prodNotifier);
    prodService.processOrder("ORD-6001", "alice@enterprise.com", 450.00);

    // 2. Automated Hermetic Unit Testing (Zero Network / Zero Database!)
    std::cout << "\n--- 2. Hermetic Unit Test Execution ---\n";
    MockDatabase mockDb;
    MockNotifier mockNotifier;

    OrderProcessingService testService(mockDb, mockNotifier);
    bool success = testService.processOrder("ORD-TEST-01", "test@domain.com", 99.00);

    // Assertions verify business logic purely in memory
    assert(success == true);
    assert(mockDb.insertedRecords.size() == 1);
    assert(mockDb.insertedRecords[0].first == "orders");
    assert(mockNotifier.sentNotifications.size() == 1);
    assert(mockNotifier.sentNotifications[0].first == "test@domain.com");

    std::cout << "✅ All hermetic assertions passed! Zero external dependencies needed.\n";
    return 0;
}
