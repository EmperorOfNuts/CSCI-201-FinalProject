#ifndef FINAL_PROJECT_TRANSACTION_H
#define FINAL_PROJECT_TRANSACTION_H
#include <string>
#include "Date.hpp"
#include <optional>

enum class TransactionType {
    Checkout,
    Return
};

class Transaction {
    int patronID;
    std::string bookTitle;
    TransactionType type;
    Date date;
    mutable std::optional<Date> returnDate; // Cached return date

public:
    Transaction(int pid, std::string bookTitle, TransactionType type);
    Transaction(int pid, std::string bookTitle, TransactionType type, const Date& date);

    void displayTransaction() const;

    [[nodiscard]] int getPatronID() const { return patronID; }
    [[nodiscard]] std::string getBookTitle() const { return bookTitle; }
    [[nodiscard]] TransactionType getType() const { return type; }
    [[nodiscard]] Date getDate() const { return date; }
    [[nodiscard]] std::optional<Date> getReturnDate() const;
    [[nodiscard]] std::string typeToString() const;

    static TransactionType stringToType(const std::string& str);
};

#endif