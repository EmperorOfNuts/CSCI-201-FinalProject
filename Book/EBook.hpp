#ifndef FINAL_PROJECT_EBOOK_H
#define FINAL_PROJECT_EBOOK_H

#include <format>
#include "Book.hpp"

class EBook : public Book {
private:
    double fileSizeMB;

public:
    EBook(const std::string &title, const std::string &author, Genre genre, double size);

    void displayInfo() const override;

    std::string getType() override { return "E-Book: " + std::format("{:.3f}", fileSizeMB) + "MB"; }

    [[nodiscard]] double getFileSize() const { return fileSizeMB; };
};

#endif