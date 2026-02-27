#ifndef FINAL_PROJECT_PATRON_H
#define FINAL_PROJECT_PATRON_H

#include <string>
#include <vector>
#include <iostream>
#include "Book/Book.hpp"

class Patron {
private:
    std::string name;
    int id;
    std::vector<Book*> borrowedBooks;
    static int nextId;

public:
    explicit Patron(std::string name);
    Patron(std::string name, int id);

    void borrowBook(Book* book);
    void returnBook(Book* book);
    void displayPatron() const;

    [[nodiscard]] std::string getName() const { return name; }
    [[nodiscard]] int getId() const { return id; }
    [[nodiscard]] const std::vector<Book*>& getBorrowedBooks() const { return borrowedBooks; }

    bool operator==(const Patron& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Patron& p);

    static void resetIdCounter(int startFrom = 1);
};

#endif