#include "Transaction.hpp"
#include <iostream>
#include <utility>
#include <algorithm>
#include <cctype>

Transaction::Transaction(const int pid, std::string bookTitle, const TransactionType type)
    : patronID(pid)
    , bookTitle(std::move(bookTitle))
    , type(type)
    , date()
    , returnDate(std::nullopt)
{}

Transaction::Transaction(const int pid, std::string bookTitle, const TransactionType type, const Date& date)
    : patronID(pid)
    , bookTitle(std::move(bookTitle))
    , type(type)
    , date(date)
    , returnDate(std::nullopt)
{}

std::optional<Date> Transaction::getReturnDate() const {
    if (type == TransactionType::Checkout) {
        if (!returnDate.has_value()) returnDate = date.addDays(30);
        return returnDate;
    }
    return std::nullopt;
}

std::string Transaction::typeToString() const {
    return type == TransactionType::Checkout ? "checked out" : "return";
}

void Transaction::displayTransaction() const {
    std::cout << "[" << date << "] Patron " << patronID << " "
              << typeToString() << " \"" << bookTitle << "\"";

    if (const auto retDate = getReturnDate(); retDate.has_value()) std::cout << " - Return by " << retDate.value();
    std::cout << std::endl;
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