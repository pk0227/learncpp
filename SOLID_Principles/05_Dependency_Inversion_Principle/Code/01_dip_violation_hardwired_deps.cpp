/**
 * @file 01_dip_violation_hardwired_deps.cpp
 * @brief Demonstrates the Dependency Inversion Principle (DIP) violation: Hardwired Dependencies.
 * 
 * Formal Definition of DIP:
 *   1. High-level modules should not depend on low-level modules. Both should depend on abstractions.
 *   2. Abstractions should not depend on details. Details should depend on abstractions.
 * 
 * In this anti-pattern:
 *   - `OrderProcessingService` (High-level business policy) directly instantiates and depends upon
 *     `MySqlDatabase` and `SmtpClient` (Low-level I/O implementation details).
 * 
 * Why this fails in production C++:
 *   - Zero Hermetic Testability: Unit testing business rules requires a live MySQL database
 *     and live SMTP mail server; tests cannot run in offline CI/CD pipelines.
 *   - Inflexible Coupling: Switching from MySQL to PostgreSQL, or SMTP to AWS SES, requires
 *     editing and recompiling the core business logic.
 *   - Fragile Lifetimes: The service is tightly coupled to the exact constructor signatures
 *     of its dependencies.
 * 
 * @standard C++20
 */

#include <iostream>
#include <string>

// ============================================================================
// LOW-LEVEL INFRASTRUCTURE DETAILS
// ============================================================================
class MySqlDatabase {
private:
    std::string connectionString_;

public:
    explicit MySqlDatabase(std::string connStr) : connectionString_(std::move(connStr)) {
        std::cout << "[MySQL] Initializing connection pool to: " << connectionString_ << "\n";
    }

    void executeInsert(const std::string& table, const std::string& record) {
        std::cout << "[MySQL] INSERT INTO " << table << " VALUES ('" << record << "');\n";
    }
};

class SmtpClient {
private:
    std::string host_;
    int port_;

public:
    SmtpClient(std::string host, int port) : host_(std::move(host)), port_(port) {
        std::cout << "[SMTP] Connecting to socket " << host_ << ":" << port_ << "\n";
    }

    void sendMail(const std::string& to, const std::string& body) {
        std::cout << "[SMTP] Dispatched to: " << to << " | Body: " << body << "\n";
    }
};

// ============================================================================
// ANTI-PATTERN: High-Level Module Hardwiring Concrete Low-Level Details
// ============================================================================
class OrderProcessingService {
private:
    // HARDWIRED CONCRETE INSTANCES: Direct dependency on low-level details!
    MySqlDatabase database_{"tcp://db-prod.internal:3306/production"};
    SmtpClient mailer_{"smtp.mailgun.org", 587};

public:
    void processOrder(const std::string& orderId, const std::string& email, double amount) {
        std::cout << "\n[Business Logic] Validating order " << orderId << " for $" << amount << "...\n";

        // Business rule
        if (amount <= 0.0) {
            std::cout << "[Business Logic] Order rejected: non-positive amount.\n";
            return;
        }

        // Direct reliance on concrete MySQL
        database_.executeInsert("orders", orderId + ", " + std::to_string(amount));

        // Direct reliance on concrete SMTP
        mailer_.sendMail(email, "Your order #" + orderId + " is confirmed!");

        std::cout << "[Business Logic] Order processing completed.\n";
    }
};

int main() {
    std::cout << "=== DIP Violation: High-Level Module Coupled to Low-Level Details ===\n\n";

    OrderProcessingService service;
    service.processOrder("ORD-5001", "alice@enterprise.com", 299.99);

    std::cout << "\nProblem: Notice how impossible it is to unit test OrderProcessingService\n"
              << "without physically connecting to tcp://db-prod.internal:3306!\n";

    return 0;
}
