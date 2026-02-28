#include "Transaction.hpp"
#include <iostream>
#include <utility>
#include <algorithm>
#include <cctype>

Transaction::Transaction(const int pid, std::string bookTitle, const TransactionType type)
    : patronID(pid)
    , bookTitle(std::move(bookTitle))
    , type(type)
    , date() {}

Transaction::Transaction(const int pid, std::string bookTitle, const TransactionType type, const Date& date)
    : patronID(pid)
    , bookTitle(std::move(bookTitle))
    , type(type)
    , date(date) {}

void Transaction::displayTransaction() const {
    std::cout << "[" << date << "] Patron " << patronID << " "
              << typeToString() << " \"" << bookTitle << "\"" << std::endl;
}

std::string Transaction::typeToString() const {
    return type == TransactionType::Checkout ? "Check Out" : "Return";
}

// Annoying grammar lol
TransactionType Transaction::stringToType(const std::string& str) {
    std::string lower = str;
    std::ranges::transform(lower, lower.begin(), ::tolower);

    if (!lower.empty() && lower.back() == '.') lower.pop_back();

    if (lower == "checked out" || lower == "checkout" || lower == "checkout." || lower == "checked_out" || lower == "checked out.") return TransactionType::Checkout;
    if (lower == "return" || lower == "return." || lower == "returned") return TransactionType::Return;

    return TransactionType::Return;
}