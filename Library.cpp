#include "Library.hpp"
#include <algorithm>
#include <iostream>
#include <filesystem>
#include <ranges>
#include <cctype>

Book* parseBookLine(const std::string& line) {
    std::stringstream ss(line);
    std::string genreStr, title, author, type, extraData, statusStr, checkoutDateStr, dueDateStr, patronIdStr;

    std::getline(ss, genreStr, '|');
    std::getline(ss, title, '|');
    std::getline(ss, author, '|');
    std::getline(ss, type, '|');
    std::getline(ss, extraData, '|');
    std::getline(ss, statusStr, '|');
    std::getline(ss, checkoutDateStr, '|');
    std::getline(ss, dueDateStr, '|');
    std::getline(ss, patronIdStr, '|');

    const Book::Genre genre = Book::stringToGenre(genreStr);
    Book* book = nullptr;

    if (type == "EBook") {
        const double fileSize = std::stod(extraData);
        book = new EBook(title, author, genre, fileSize);
    } else if (type == "PrintedBook") {
        const int pages = std::stoi(extraData);
        book = new PrintedBook(title, author, genre, pages);
    } else {
        book = new Book(title, author, genre);
    }

    // Restore checkout state if available
    if (!statusStr.empty()) {
        if (statusStr == "CheckedOut") {
            book->setStatus(Book::BookStatus::CheckedOut);

            if (!checkoutDateStr.empty() && checkoutDateStr != "null") {
                int d, m, y;
                char delim;
                std::stringstream cdss(checkoutDateStr);
                cdss >> d >> delim >> m >> delim >> y;
                book->setCheckoutDate(Date(d, m, y));
            }

            if (!dueDateStr.empty() && dueDateStr != "null") {
                int d, m, y;
                char delim;
                std::stringstream ddss(dueDateStr);
                ddss >> d >> delim >> m >> delim >> y;
                book->setDueDate(Date(d, m, y));
            }

            if (!patronIdStr.empty() && patronIdStr != "null") book->setCurrentPatronId(std::stoi(patronIdStr));

        } else {
            book->setStatus(Book::BookStatus::Available);
        }
    }

    return book;
}

std::string bookToString(const Book* book) {
    std::string result = Book::genreToString(book->getGenre()) + "|" + book->getTitle() + "|" + book->getAuthor() + "|";

    if (auto* ebook = dynamic_cast<const EBook*>(book)) result += "EBook|" + std::to_string(ebook->getFileSize()) + "|";
    else if (auto* printed = dynamic_cast<const PrintedBook*>(book)) result += "PrintedBook|" + std::to_string(printed->getPageCount()) + "|";
    else result += "Unknown|0|";

    result += (book->getStatus() == Book::BookStatus::Available ? "Available|" : "CheckedOut|");

    // Checkout date
    if (book->getCheckoutDate().has_value()) result += book->getCheckoutDate()->toString() + "|";
    else result += "null|";

    // Due date
    if (book->getDueDate().has_value()) result += book->getDueDate()->toString() + "|";
    else result += "null|";

    // Current patron ID
    if (book->getCurrentPatronId().has_value()) result += std::to_string(book->getCurrentPatronId().value());
    else result += "null";

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
    // Delete loaded books if refreshing
    for (const auto* book : books) delete book;
    books.clear();

    // Load new books
    loadFromFile(books, filename, parseBookLine);

    // Rebuild patron-book associations
    rebuildPatronBorrowedBooks();
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
        loadPatrons();
        loadBooks();
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

void Library::rebuildPatronBorrowedBooks() {
    for (auto& patron : patrons) patron.clearBorrowedBooks();

    for (auto* book : books) {
        if (book->getStatus() == Book::BookStatus::CheckedOut) {
            if (auto patronId = book->getCurrentPatronId()) {
                if (Patron* patron = findPatron(*patronId)) patron->addBorrowedBook(book);
            }
        }
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

    saveData();
}

void Library::returnBook(int patronId, const std::string& title) {
    Patron* patron = findPatron(patronId);
    if (!patron) throw std::runtime_error("Patron with ID " + std::to_string(patronId) + " not found.");

    Book* book = findBook(title);
    if (!book) throw std::runtime_error("Book '" + title + "' not found.");

    patron->returnBook(book);  // This calls book->returnBook() and removes from patron's vector
    transactions.emplace_back(patronId, title, TransactionType::Return);

    saveData();
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