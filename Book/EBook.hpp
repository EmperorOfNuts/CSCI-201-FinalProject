#ifndef FINAL_PROJECT_EBOOK_H
#define FINAL_PROJECT_EBOOK_H

#include "Book.hpp"

class EBook : public Book {
private:
    double fileSizeMB;

public:
    EBook(const std::string &title, const std::string &author, Genre genre, double size);

    void displayInfo() const override;

    [[nodiscard]] double getFileSize() const { return fileSizeMB; };
};

#endif