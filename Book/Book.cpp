// Book.cpp
#include "Book.hpp"
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <utility>

Book::Book(std::string title, std::string author, const Genre genre)
    : title(std::move(title)), author(std::move(author)), genre(genre), status(BookStatus::Available) {}

void Book::displayInfo() const {
    std::cout <<"Unknown Type " << getTitle() << " - " << getAuthor() << ", "
              << genreToString(getGenre()) << " [" << bookStatusToString(getStatus())
              << "]" << std::endl;
}

bool Book::operator==(const Book& other) const {
    return title == other.title && author == other.author;
}

std::ostream& operator<<(std::ostream& os, const Book& b) {
    os << b.title << "," << b.author << "," << Book::genreToString(b.genre)
       << "," << Book::bookStatusToString(b.status);
    return os;
}

std::string Book::genreToString(const Genre g) {
    switch (g) {
        case Genre::Fiction: return "Fiction";
        case Genre::NonFiction: return "Non-Fiction";
        case Genre::Mystery: return "Mystery";
        case Genre::Science: return "Science";
        case Genre::Biography: return "Biography";
        default: return "Unknown";
    }
}

std::string Book::bookStatusToString(const BookStatus s) {
    switch (s) {
        case BookStatus::Available: return "Available";
        case BookStatus::CheckedOut: return "Checked Out";
        default: return "Unknown";
    }
}

Book::Genre Book::stringToGenre(const std::string& s) {
    if (s == "Fiction") return Genre::Fiction;
    if (s == "Non-Fiction") return Genre::NonFiction;
    if (s == "Mystery") return Genre::Mystery;
    if (s == "Science") return Genre::Science;
    if (s == "Biography") return Genre::Biography;
    throw std::invalid_argument("Invalid genre string: " + s);
}