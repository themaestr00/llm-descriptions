#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <numeric>
#include <algorithm>

struct OrderItem {
    std::string productId;
    int quantity;
    double pricePerUnit;
};

struct Order {
    std::string orderId;
    std::string customerId;
    std::vector<OrderItem> items;
    std::string status;
    double totalAmount;
    std::string shippingAddress;
    bool isPriority;
};

struct InventoryItem {
    std::string productId;
    int quantityAvailable;
    int reservedStock;
};

// Global inventory simulation
std::map<std::string, InventoryItem> inventory;

void log(const std::string& message) {
    std::cout << "[LOG]: " << message << std::endl;
}

/**
 * Processes a batch of orders.
 * This function handles inventory validation, calculates totals including taxes and discounts,
 * updates order status, and manages shipping logistics logic.
 *
 * @param orders A reference to a vector of orders to be processed.
 * @return A summary string of the processing results.
 */
std::string processOrders(std::vector<Order>& orders) {
    int processedCount = 0;
    int failedCount = 0;
    double totalRevenue = 0.0;

    log("Starting order processing batch...");

    for (auto& order : orders) {
        log("Processing Order ID: " + order.orderId);

        if (order.status != "PENDING") {
            log("Order " + order.orderId + " skipped (not PENDING).");
            continue;
        }

        double subtotal = 0.0;
        bool stockAvailable = true;
        std::vector<std::string> missingItems;

        // Step 1: Validate Inventory
        for (const auto& item : order.items) {
            if (inventory.find(item.productId) == inventory.end()) {
                stockAvailable = false;
                missingItems.push_back(item.productId + " (Unknown Product)");
                continue;
            }

            InventoryItem& invItem = inventory[item.productId];
            if (invItem.quantityAvailable - invItem.reservedStock < item.quantity) {
                stockAvailable = false;
                missingItems.push_back(item.productId + " (Insufficient Stock)");
            }
        }

        if (!stockAvailable) {
            order.status = "FAILED";
            failedCount++;
            std::string reason = "Missing items: ";
            for (const auto& msg : missingItems) reason += msg + "; ";
            log("Order " + order.orderId + " failed. " + reason);
            continue;
        }

        // Step 2: Reserve Stock and Calculate Subtotal
        for (const auto& item : order.items) {
            inventory[item.productId].reservedStock += item.quantity;
            subtotal += item.quantity * item.pricePerUnit;
        }

        // Step 3: Apply Discounts
        double discount = 0.0;
        if (subtotal > 1000.0) {
            discount = subtotal * 0.10; // 10% discount for large orders
        } else if (order.isPriority) {
            discount = subtotal * 0.05; // 5% discount for priority customers
        }

        // Seasonal discount simulation
        if (order.orderId.find("SUMMER") != std::string::npos) {
             discount += subtotal * 0.02;
        }

        double taxableAmount = subtotal - discount;

        // Step 4: Calculate Tax
        // Simplified tax logic based on shipping address region (mocked)
        double taxRate = 0.0;
        if (order.shippingAddress.find("NY") != std::string::npos) {
            taxRate = 0.08875;
        } else if (order.shippingAddress.find("CA") != std::string::npos) {
            taxRate = 0.0725;
        } else {
            taxRate = 0.05; // Default flat rate
        }

        double tax = taxableAmount * taxRate;
        order.totalAmount = taxableAmount + tax;

        // Step 5: Finalize Order
        // "Deduct" from inventory (commit reservation)
        for (const auto& item : order.items) {
            inventory[item.productId].quantityAvailable -= item.quantity;
            inventory[item.productId].reservedStock -= item.quantity;
        }

        order.status = "SHIPPED";
        processedCount++;
        totalRevenue += order.totalAmount;

        log("Order " + order.orderId + " successfully processed. Total: " + std::to_string(order.totalAmount));
    }

    std::string summary = "Batch Complete. Processed: " + std::to_string(processedCount) +
                          ", Failed: " + std::to_string(failedCount) +
                          ", Revenue: " + std::to_string(totalRevenue);
    log(summary);
    return summary;
}
