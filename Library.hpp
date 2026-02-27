#ifndef FINAL_PROJECT_LIBRARY_H
#define FINAL_PROJECT_LIBRARY_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <memory>
#include <type_traits>
#include "Book/Book.hpp"
#include "Book/EBook.hpp"
#include "Book/PrintedBook.hpp"
#include "Transaction/Patron.hpp"
#include "Transaction/Transaction.hpp"

/* Templates... templates...
 * Why does this assignment force you upon me?
 * Every time I see you I want to flee!
 * I'd rather learn JavaFX without Deepseek,
 * Or bathe in an icy creek! */

template<typename T>
void saveToFile(const std::vector<T*>& items, const std::string& filename,
                std::string (*itemToString)(const T*)) {

    std::ofstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Failed to open file for writing: " + filename);

    for (const auto& item : items) file << itemToString(item) << std::endl;

    file.close();
    std::cout << "Saved " << items.size() << " items to " << filename << std::endl;
}

template<typename T>
void saveToFile(const std::vector<T>& items, const std::string& filename,
                std::string (*itemToString)(const T&)) {

    std::ofstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Failed to open file for writing: " + filename);

    for (const auto& item : items) file << itemToString(item) << std::endl;

    file.close();
    std::cout << "Saved " << items.size() << " items to " << filename << std::endl;
}

template<typename T>
void loadFromFile(std::vector<T*>& items, const std::string& filename,
                  T* (*parseLine)(const std::string&)) {

    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Failed to open file: " + filename);

    std::string line;
    int lineNum = 0;
    int successCount = 0;

    while (std::getline(file, line)) {
        lineNum++;
        if (line.empty()) continue;

        try {
            T* item = parseLine(line);
            items.push_back(item);
            successCount++;
        } catch (const std::exception& e) {
            std::cerr << "Error parsing line " << lineNum << ": " << e.what() << std::endl;
        }
    }

    file.close();
    std::cout << "Loaded " << successCount << " items from " << filename << std::endl;
}

template<typename T>
void loadFromFile(std::vector<T>& items, const std::string& filename,
                  T (*parseLine)(const std::string&)) {

    std::ifstream file(filename);
    if (!file.is_open()) {
        if constexpr (std::is_same_v<T, Transaction>) {
            std::cout << "No transactions file found, starting with empty log." << std::endl;
            return;
        }
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string line;
    int lineNum = 0;
    int successCount = 0;

    while (std::getline(file, line)) {
        lineNum++;
        if (line.empty()) continue;

        try {
            T item = parseLine(line);
            items.push_back(item);
            successCount++;
        } catch (const std::exception& e) {
            std::cerr << "Error parsing line " << lineNum << ": " << e.what() << std::endl;
        }
    }

    file.close();
    std::cout << "Loaded " << successCount << " items from " << filename << std::endl;
}

class Library {
private:
    std::vector<Book*> books;
    std::vector<Patron> patrons;
    std::vector<Transaction> transactions;

public:
    ~Library();

    // File I/O methods
    void loadBooks(const std::string& filename = "Data/Books.txt");
    void loadPatrons(const std::string& filename = "Data/Patrons.txt");
    void loadTransactions(const std::string& filename = "Data/Transactions.txt");
    void saveBooks(const std::string& filename = "Data/Books.txt") const;
    void savePatrons(const std::string& filename = "Data/Patrons.txt") const;
    void saveTransactions(const std::string& filename = "Data/Transactions.txt") const;
    void loadData();
    void saveData() const;

    // Core operations
    void addBook(Book* b);
    void addPatron(const Patron& p);
    void checkoutBook(int patronId, const std::string& title);
    void returnBook(int patronId, const std::string& title);
    Book* findBook(const std::string& title);
    Patron* findPatron(int id);

    // Getters for GUI
    [[nodiscard]] const std::vector<Book*>& getBooks() const { return books; }
    [[nodiscard]] const std::vector<Transaction>& getTransactions() const { return transactions; }
    [[nodiscard]] const std::vector<Patron>& getPatrons() const { return patrons; }

    // Search methods for GUI
    [[nodiscard]] std::vector<Book*> searchBooksByAuthor(const std::string& author) const;
    [[nodiscard]] std::vector<Book*> searchBooksByGenre(Book::Genre genre) const;
    [[nodiscard]] std::vector<Book*> searchBooksByTitle(const std::string& title) const;
};

#endif