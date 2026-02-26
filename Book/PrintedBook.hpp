#ifndef FINAL_PROJECT_PRINTEDBOOK_H
#define FINAL_PROJECT_PRINTEDBOOK_H

#include "Book.hpp"

class PrintedBook : public Book {
private:
    int pageCount;

public:
    PrintedBook(const std::string &title, const std::string &author, Genre genre, int pages);

    void displayInfo() const override;

    [[nodiscard]] int getPageCount() const { return pageCount; };
};

#endif