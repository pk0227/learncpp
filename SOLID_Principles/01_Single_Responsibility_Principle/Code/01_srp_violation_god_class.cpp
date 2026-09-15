/**
 * @file 01_srp_violation_god_class.cpp
 * @brief Demonstrates a severe Single Responsibility Principle (SRP) violation: The "God Class".
 * 
 * In this anti-pattern, OrderManager conflates:
 *   1. Core Domain Logic (Order calculation, discount rules, tax computation)
 *   2. Data Persistence (SQL execution, database transaction management)
 *   3. Serialization (Formatting domain objects into JSON strings)
 *   4. Notification / Network I/O (SMTP email dispatch, socket transmission)
 * 
 * Why this fails in senior-level production systems:
 *   - Multiple Actors / Reasons to Change: Financial analysts (tax changes), DBAs (schema changes),
 *     API consumers (JSON schema changes), and Sysadmins (SMTP server changes) all force edits
 *     to the exact same class and header file.
 *   - Recompilation Cascades: A minor database index hint or column rename causes recompilation
 *     of unrelated financial calculations across the entire system.
 *   - Untestable Logic: Calculating a discount cannot be unit-tested in isolation without
 *     mocking or connecting to a live SQL database and network socket.
 * 
 * @standard C++20
 */

#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <iomanip>

struct OrderItem {
    std::string id;
    std::string name;
    double unitPrice;
    int quantity;
};

// ============================================================================
// ANTI-PATTERN: God Object / Monolithic Class
// ============================================================================
class OrderManager {
private:
    std::string orderId_;
    std::string customerEmail_;
    std::vector<OrderItem> items_;
    double totalAmount_{0.0};
    bool isPaid_{false};

public:
    OrderManager(std::string orderId, std::string customerEmail)
        : orderId_(std::move(orderId)), customerEmail_(std::move(customerEmail)) {}

    void addItem(const std::string& id, const std::string& name, double price, int qty) {
        items_.push_back({id, name, price, qty});
    }

    // --- Responsibility 1: Core Domain / Financial Logic ---
    void calculateTotal(double taxRate, double discountPercent) {
        double subtotal = 0.0;
        for (const auto& item : items_) {
            subtotal += item.unitPrice * item.quantity;
        }
        double discount = subtotal * (discountPercent / 100.0);
        double tax = (subtotal - discount) * taxRate;
        totalAmount_ = (subtotal - discount) + tax;
        std::cout << "[Domain] Calculated total for Order " << orderId_ << ": $" << totalAmount_ << "\n";
    }

    // --- Responsibility 2: Data Persistence / SQL Transactions ---
    // Reason to change: Database schema updates, migrating from PostgreSQL to SQLite, connection pools.
    bool saveToDatabase() {
        std::cout << "[Database] Connecting to Postgres pool...\n";
        std::cout << "[Database] BEGIN TRANSACTION;\n";
        std::cout << "[Database] INSERT INTO orders (id, email, total, paid) VALUES ('"
                  << orderId_ << "', '" << customerEmail_ << "', " << totalAmount_ 
                  << ", " << (isPaid_ ? "TRUE" : "FALSE") << ");\n";
        for (const auto& item : items_) {
            std::cout << "[Database] INSERT INTO order_items (order_id, item_id, qty, price) VALUES ('"
                      << orderId_ << "', '" << item.id << "', " << item.quantity 
                      << ", " << item.unitPrice << ");\n";
        }
        std::cout << "[Database] COMMIT;\n";
        return true;
    }

    // --- Responsibility 3: Presentation / Serialization ---
    // Reason to change: API contract changes, switching from JSON to Protocol Buffers or XML.
    std::string serializeToJson() const {
        std::string json = "{\n";
        json += "  \"order_id\": \"" + orderId_ + "\",\n";
        json += "  \"customer_email\": \"" + customerEmail_ + "\",\n";
        json += "  \"total_amount\": " + std::to_string(totalAmount_) + ",\n";
        json += "  \"items\": [\n";
        for (size_t i = 0; i < items_.size(); ++i) {
            json += "    {\"id\": \"" + items_[i].id + "\", \"qty\": " 
                 + std::to_string(items_[i].quantity) + ", \"price\": " 
                 + std::to_string(items_[i].unitPrice) + "}" 
                 + (i + 1 < items_.size() ? "," : "") + "\n";
        }
        json += "  ]\n}";
        return json;
    }

    // --- Responsibility 4: Network / Notification Dispatch ---
    // Reason to change: Switching from SMTP to SendGrid API, TLS updates, email template changes.
    void sendConfirmationEmail(const std::string& smtpServerHost) {
        std::cout << "[SMTP] Connecting to mail server at: " << smtpServerHost << ":587\n";
        std::cout << "[SMTP] Handshake: STARTTLS\n";
        std::cout << "[SMTP] Sending email to: " << customerEmail_ << "\n";
        std::cout << "[SMTP] Subject: Confirmation for Order #" << orderId_ << "\n";
        std::cout << "[SMTP] Body: Thank you for your purchase of $" << totalAmount_ << "!\n";
        std::cout << "[SMTP] Message dispatched successfully.\n";
    }

    double getTotalAmount() const { return totalAmount_; }
    void markAsPaid() { isPaid_ = true; }
};

int main() {
    std::cout << "=== SRP Violation: Monolithic God Class Demo ===\n\n";

    OrderManager order("ORD-2026-9901", "alice@enterprise.com");
    order.addItem("SKU-101", "High-Performance C++ Guide", 59.99, 2);
    order.addItem("SKU-202", "Mechanical Keyboard", 149.50, 1);

    // Conflated workflow
    order.calculateTotal(0.08, 10.0);
    order.markAsPaid();
    order.saveToDatabase();

    std::cout << "\nGenerated Payload:\n" << order.serializeToJson() << "\n\n";

    order.sendConfirmationEmail("smtp.internal-relay.corp");

    std::cout << "\nNote how testing calculateTotal() requires pulling in database and SMTP dependencies!\n";
    return 0;
}
