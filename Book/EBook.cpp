#include "EBook.hpp"

EBook::EBook(const std::string &title, const std::string &author, const Genre genre, const double size)
    : Book(title, author, genre), fileSizeMB(size) {}

void EBook::displayInfo() const {
    std::cout << "[E-Book] " << getTitle() << " - " << getAuthor() << ", "
              << genreToString(getGenre()) << " [" << bookStatusToString(getStatus())
              << "] (File Size: " << fileSizeMB << " MB)" << std::endl;
}