#include "Patron.hpp"
#include <algorithm>
#include <stdexcept>
#include <utility>

int Patron::nextId = 1;

Patron::Patron(std::string name)
    : name(std::move(name)), id(nextId++) {}

Patron::Patron(std::string name, const int id)
    : name(std::move(name)), id(id) {
    if (id >= nextId) nextId = id + 1;
}

void Patron::borrowBook(Book* book) {
    if (!book) throw std::invalid_argument("Cannot borrow null book.");

    if (book->getStatus() != Book::BookStatus::Available) throw std::runtime_error("Book '" + book->getTitle() + "' is not available.");

    const auto it = std::ranges::find_if(borrowedBooks, [book](const Book* b) { return b->getTitle() == book->getTitle(); } );
    if (it != borrowedBooks.end()) throw std::runtime_error("Patron already has this book borrowed.");

    borrowedBooks.push_back(book);
    book->checkout(id);  // Use the new checkout method

    std::cout << "Book '" << book->getTitle() << "' borrowed successfully by " << name << "." << std::endl;
}

void Patron::returnBook(Book* book) {
    if (!book) throw std::invalid_argument("Cannot return null book.");

    const auto it = std::ranges::find(borrowedBooks, book);
    if (it == borrowedBooks.end()) throw std::runtime_error("Patron did not borrow this book.");

    borrowedBooks.erase(it);
    book->returnBook();  // Use the new return method

    std::cout << "Book '" << book->getTitle() << "' returned successfully by " << name << "." << std::endl;
}

void Patron::displayPatron() const {
    std::cout << "\nPatron Information:" << std::endl;
    std::cout << "ID: " << id << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Books Borrowed: " << borrowedBooks.size() << std::endl;

    if (!borrowedBooks.empty()) {
        std::cout << "Borrowed Books List:" << std::endl;
        for (size_t i = 0; i < borrowedBooks.size(); ++i) {
            std::cout << "  " << (i + 1) << ". " << borrowedBooks[i]->getTitle() << " by " << borrowedBooks[i]->getAuthor();
            if (borrowedBooks[i]->isOverdue()) {
                std::cout << " - OVERDUE!";
            }
            std::cout << std::endl;
        }
    }
}

bool Patron::operator==(const Patron& other) const { return id == other.id; }

std::ostream& operator<<(std::ostream& os, const Patron& p) {
    os << "Patron{ID: " << p.id << ", Name: " << p.name 
       << ", Books Borrowed: " << p.borrowedBooks.size() << "}";
    return os;
}

void Patron::resetIdCounter(const int startFrom) { nextId = startFrom; }