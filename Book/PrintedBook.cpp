#include "PrintedBook.hpp"

PrintedBook::PrintedBook(const std::string &title, const std::string &author, const Genre genre, const int pages)
    : Book(title, author, genre), pageCount(pages) {}

void PrintedBook::displayInfo() const {
    std::cout << "[Printed] " << getTitle() << " - " << getAuthor() << ", "
              << genreToString(getGenre()) << " [" << bookStatusToString(getStatus())
              << "] (Pages: " << pageCount << ")" << std::endl;
}