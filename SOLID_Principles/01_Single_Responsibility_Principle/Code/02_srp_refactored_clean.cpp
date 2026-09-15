/**
 * @file 02_srp_refactored_clean.cpp
 * @brief Demonstrates proper Single Responsibility Principle (SRP) refactoring.
 * 
 * Each class has one, and only one, reason to change:
 *   - Order: Pure domain entity holding state and maintaining entity invariants.
 *   - PricingEngine: Stateless domain calculator for tax, discounts, and totals (100% testable in isolation!).
 *   - OrderRepository: Encapsulates SQL persistence and transaction boundaries.
 *   - OrderJsonSerializer: Encapsulates presentation and wire serialization formats.
 *   - EmailNotificationService: Encapsulates network communication and email formatting.
 * 
 * Architectural Benefits:
 *   - High Cohesion: Every class's methods operate on the complete internal state of that class.
 *   - Zero-Mock Unit Testing: Financial calculations can be tested with deterministic pure functions.
 *   - Isolated Change: Changing the database schema has zero impact on JSON formatting or pricing.
 *   - Header Decoupling: Code calculating discounts does not include SQL client or SMTP network headers.
 * 
 * @standard C++20
 */

#include <iostream>
#include <string>
#include <vector>
#include <string_view>
#include <memory>
#include <iomanip>

// ============================================================================
// 1. DOMAIN MODEL (Value / State Holder)
// Reason to change: Changes in business domain entity attributes.
// ============================================================================
struct OrderItem {
    std::string id;
    std::string name;
    double unitPrice;
    int quantity;
};

class Order {
private:
    std::string orderId_;
    std::string customerEmail_;
    std::vector<OrderItem> items_;
    double totalAmount_{0.0};
    bool isPaid_{false};

public:
    Order(std::string orderId, std::string customerEmail)
        : orderId_(std::move(orderId)), customerEmail_(std::move(customerEmail)) {}

    void addItem(std::string id, std::string name, double price, int qty) {
        items_.push_back(OrderItem{std::move(id), std::move(name), price, qty});
    }

    void setTotalAmount(double total) noexcept { totalAmount_ = total; }
    [[nodiscard]] double getTotalAmount() const noexcept { return totalAmount_; }

    void markAsPaid() noexcept { isPaid_ = true; }
    [[nodiscard]] bool isPaid() const noexcept { return isPaid_; }

    [[nodiscard]] const std::string& getId() const noexcept { return orderId_; }
    [[nodiscard]] const std::string& getCustomerEmail() const noexcept { return customerEmail_; }
    [[nodiscard]] const std::vector<OrderItem>& getItems() const noexcept { return items_; }
};

// ============================================================================
// 2. DOMAIN LOGIC / PRICING ENGINE
// Reason to change: Changes in financial rules, discounting tiers, or tax laws.
// Pure logic: Zero I/O, zero network, zero database dependencies!
// ============================================================================
class PricingEngine {
public:
    struct PricingBreakdown {
        double subtotal{0.0};
        double discountAmount{0.0};
        double taxAmount{0.0};
        double finalTotal{0.0};
    };

    [[nodiscard]] PricingBreakdown calculate(const Order& order, double taxRate, double discountPercent) const {
        double subtotal = 0.0;
        for (const auto& item : order.getItems()) {
            subtotal += item.unitPrice * item.quantity;
        }

        double discount = subtotal * (discountPercent / 100.0);
        double taxableAmount = subtotal - discount;
        double tax = taxableAmount * taxRate;
        double finalTotal = taxableAmount + tax;

        return PricingBreakdown{subtotal, discount, tax, finalTotal};
    }
};

// ============================================================================
// 3. DATA PERSISTENCE / REPOSITORY
// Reason to change: Database schema updates, ORM configuration, SQL query tuning.
// ============================================================================
class OrderRepository {
public:
    bool save(const Order& order) {
        std::cout << "[Repository] Connecting to DB connection pool...\n";
        std::cout << "[Repository] BEGIN TRANSACTION;\n";
        std::cout << "[Repository] INSERT INTO orders (id, email, total, paid) VALUES ('"
                  << order.getId() << "', '" << order.getCustomerEmail() << "', " 
                  << order.getTotalAmount() << ", " << (order.isPaid() ? "TRUE" : "FALSE") << ");\n";
        
        for (const auto& item : order.getItems()) {
            std::cout << "[Repository] INSERT INTO order_items (order_id, item_id, qty, price) VALUES ('"
                      << order.getId() << "', '" << item.id << "', " << item.quantity 
                      << ", " << item.unitPrice << ");\n";
        }
        std::cout << "[Repository] COMMIT;\n";
        return true;
    }
};

// ============================================================================
// 4. SERIALIZATION / PRESENTATION
// Reason to change: Wire protocol shifts (JSON -> Protobuf -> Avro), API contracts.
// ============================================================================
class OrderJsonSerializer {
public:
    [[nodiscard]] std::string serialize(const Order& order) const {
        std::string json = "{\n";
        json += "  \"order_id\": \"" + order.getId() + "\",\n";
        json += "  \"customer_email\": \"" + order.getCustomerEmail() + "\",\n";
        json += "  \"total_amount\": " + std::to_string(order.getTotalAmount()) + ",\n";
        json += "  \"paid\": " + std::string(order.isPaid() ? "true" : "false") + ",\n";
        json += "  \"items\": [\n";
        const auto& items = order.getItems();
        for (size_t i = 0; i < items.size(); ++i) {
            json += "    {\"id\": \"" + items[i].id + "\", \"qty\": " 
                 + std::to_string(items[i].quantity) + ", \"price\": " 
                 + std::to_string(items[i].unitPrice) + "}" 
                 + (i + 1 < items.size() ? "," : "") + "\n";
        }
        json += "  ]\n}";
        return json;
    }
};

// ============================================================================
// 5. NOTIFICATION SERVICE
// Reason to change: Email vendor migrations, SMS fallback, template updates.
// ============================================================================
class EmailNotificationService {
private:
    std::string smtpHost_;

public:
    explicit EmailNotificationService(std::string smtpHost)
        : smtpHost_(std::move(smtpHost)) {}

    void sendOrderConfirmation(const Order& order) const {
        std::cout << "[Notification] Connecting to SMTP server: " << smtpHost_ << "\n";
        std::cout << "[Notification] To: " << order.getCustomerEmail() << "\n";
        std::cout << "[Notification] Subject: Order #" << order.getId() << " Confirmed\n";
        std::cout << "[Notification] Body: Your order for $" << order.getTotalAmount() 
                  << " has been successfully processed.\n";
    }
};

// ============================================================================
// DEMO / INTEGRATION
// ============================================================================
int main() {
    std::cout << "=== SRP Clean Refactoring: Cohesive Single-Purpose Components ===\n\n";

    // 1. Create Domain Entity
    Order order("ORD-2026-1002", "bob@systems-architect.io");
    order.addItem("SKU-PRO-01", "C++ System Design Course", 99.00, 1);
    order.addItem("SKU-PRO-02", "Low-Latency C++ Workshop", 199.00, 1);

    // 2. Pure Business Logic execution (Easily unit-testable!)
    PricingEngine pricing;
    auto breakdown = pricing.calculate(order, 0.08, 10.0);
    order.setTotalAmount(breakdown.finalTotal);
    order.markAsPaid();

    std::cout << "[Main] Subtotal: $" << breakdown.subtotal 
              << ", Discount: -$" << breakdown.discountAmount 
              << ", Tax: +$" << breakdown.taxAmount 
              << ", Final: $" << order.getTotalAmount() << "\n\n";

    // 3. Persistence
    OrderRepository repository;
    repository.save(order);

    // 4. Serialization
    OrderJsonSerializer serializer;
    std::cout << "\nSerialized JSON Payload:\n" << serializer.serialize(order) << "\n\n";

    // 5. Notification
    EmailNotificationService notifier("smtp.cloud-mail.net");
    notifier.sendOrderConfirmation(order);

    return 0;
}
