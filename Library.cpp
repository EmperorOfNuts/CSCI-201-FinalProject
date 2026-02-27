#include "Library.hpp"
#include <algorithm>
#include <iostream>
#include <filesystem>
#include <ranges>
#include <cctype>

Book* parseBookLine(const std::string& line) {
    std::stringstream ss(line);
    std::string genreStr, title, author, type, extraData;

    std::getline(ss, genreStr, '|');
    std::getline(ss, title, '|');
    std::getline(ss, author, '|');
    std::getline(ss, type, '|');
    std::getline(ss, extraData, '|');

    const Book::Genre genre = Book::stringToGenre(genreStr);

    if (type == "EBook") {
        const double fileSize = std::stod(extraData);
        return new EBook(title, author, genre, fileSize);
    }
    if (type == "PrintedBook") {
        const int pages = std::stoi(extraData);
        return new PrintedBook(title, author, genre, pages);
    }

    return new Book(title, author, genre);
}

std::string bookToString(const Book* book) {
    std::string result = Book::genreToString(book->getGenre()) + "|" + book->getTitle() + "|" + book->getAuthor() + "|";

    if (auto* ebook = dynamic_cast<const EBook*>(book)) result += "EBook|" + std::to_string(ebook->getFileSize());
    else if (auto* printed = dynamic_cast<const PrintedBook*>(book)) result += "PrintedBook|" + std::to_string(printed->getPageCount());

    return result;
}

Patron parsePatronLine(const std::string& line) {
    std::stringstream ss(line);
    std::string idStr, name;

    std::getline(ss, idStr, '|');
    std::getline(ss, name, '|');

    int id = std::stoi(idStr);
    return {name, id};
}

std::string patronToString(const Patron& patron) {
    return std::to_string(patron.getId()) + "|" + patron.getName();
}

Transaction parseTransactionLine(const std::string& line) {
    std::stringstream ss(line);
    std::string patronIdStr, bookTitle, transactionTypeStr, dateStr;

    std::getline(ss, patronIdStr, '|');
    std::getline(ss, bookTitle, '|');
    std::getline(ss, transactionTypeStr, '|');
    std::getline(ss, dateStr, '|');

    int patronId = std::stoi(patronIdStr);

    int day, month, year;
    char delimiter;
    std::stringstream dateStream(dateStr);
    dateStream >> day >> delimiter >> month >> delimiter >> year;

    Date transactionDate(day, month, year);

    TransactionType type = Transaction::stringToType(transactionTypeStr);

    return {patronId, bookTitle, type, transactionDate};
}

std::string transactionToString(const Transaction& transaction) {
    return std::to_string(transaction.getPatronID()) + "|" +
           transaction.getBookTitle() + "|" +
           transaction.typeToString() + "|" +
           transaction.getDate().toString();
}

Library::~Library() {
    for (const auto book : books) delete book;
}

void Library::loadBooks(const std::string& filename) {
    // Delete loaded book if refreshing
    for (const auto* book : books) delete book;
    books.clear();

    // Load new books
    loadFromFile(books, filename, parseBookLine);
}

void Library::loadPatrons(const std::string& filename) {
    loadFromFile(patrons, filename, parsePatronLine);
}

void Library::loadTransactions(const std::string& filename) {
    loadFromFile(transactions, filename, parseTransactionLine);
}

void Library::saveBooks(const std::string& filename) const {
    saveToFile(books, filename, bookToString);
}

void Library::savePatrons(const std::string& filename) const {
    saveToFile(patrons, filename, patronToString);
}

void Library::saveTransactions(const std::string& filename) const {
    saveToFile(transactions, filename, transactionToString);
}

void Library::loadData() {
    try {
        loadBooks();
        loadPatrons();
        loadTransactions();
        std::cout << "\nAll data loaded successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error loading data: " << e.what() << std::endl;
    }
}

void Library::saveData() const {
    try {
        saveBooks();
        savePatrons();
        saveTransactions();
        std::cout << "\nAll data saved successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error saving data: " << e.what() << std::endl;
    }
}

void Library::addBook(Book* b) {
    if (!b) throw std::invalid_argument("Cannot add null book.");
    books.push_back(b);
    std::cout << "Book '" << b->getTitle() << "' by " << b->getAuthor() << " added to library." << std::endl;
}

void Library::addPatron(const Patron& p) {
    if (findPatron(p.getId()) != nullptr) throw std::runtime_error("Patron with ID " + std::to_string(p.getId()) + " already exists.");
    patrons.push_back(p);
    std::cout << "Patron '" << p.getName() << "' added to library." << std::endl;
}

void Library::checkoutBook(int patronId, const std::string& title) {
    Patron* patron = findPatron(patronId);
    if (!patron) throw std::runtime_error("Patron with ID " + std::to_string(patronId) + " not found.");

    Book* book = findBook(title);
    if (!book) throw std::runtime_error("Book '" + title + "' not found.");

    patron->borrowBook(book);
    transactions.emplace_back(patronId, title, TransactionType::Checkout);
}

void Library::returnBook(int patronId, const std::string& title) {
    Patron* patron = findPatron(patronId);
    if (!patron) throw std::runtime_error("Patron with ID " + std::to_string(patronId) + " not found.");

    Book* book = findBook(title);
    if (!book) throw std::runtime_error("Book '" + title + "' not found.");

    patron->returnBook(book);
    transactions.emplace_back(patronId, title, TransactionType::Return);
}

Book* Library::findBook(const std::string& title) {
    const auto it = std::ranges::find_if(books,
        [&title](const Book* b) { return b->getTitle() == title; });
    return (it != books.end()) ? *it : nullptr;
}

Patron* Library::findPatron(int id) {
    const auto it = std::ranges::find_if(patrons,
        [id](const Patron& p) { return p.getId() == id; });
    return (it != patrons.end()) ? &(*it) : nullptr;
}

std::vector<Book*> Library::searchBooksByAuthor(const std::string& author) const {
    std::vector<Book*> results;
    std::string searchAuthor = author;
    std::ranges::transform(searchAuthor, searchAuthor.begin(), ::tolower);

    std::ranges::copy_if(books, std::back_inserter(results),
        [&searchAuthor](const Book* b) {
            std::string bookAuthor = b->getAuthor();
            std::ranges::transform(bookAuthor, bookAuthor.begin(), ::tolower);
            return bookAuthor.find(searchAuthor) != std::string::npos;
        });

    return results;
}

std::vector<Book*> Library::searchBooksByGenre(Book::Genre genre) const {
    std::vector<Book*> results;
    std::ranges::copy_if(books, std::back_inserter(results),
        [genre](const Book* b) { return b->getGenre() == genre; });
    return results;
}

std::vector<Book*> Library::searchBooksByTitle(const std::string& title) const {
    std::vector<Book*> results;
    std::string searchTitle = title;
    std::ranges::transform(searchTitle, searchTitle.begin(), ::tolower);

    std::ranges::copy_if(books, std::back_inserter(results),
        [&searchTitle](const Book* b) {
            std::string bookTitle = b->getTitle();
            std::ranges::transform(bookTitle, bookTitle.begin(), ::tolower);
            return bookTitle.find(searchTitle) != std::string::npos;
        });

    return results;
}