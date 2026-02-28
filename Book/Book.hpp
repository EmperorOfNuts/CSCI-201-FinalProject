#ifndef FINAL_PROJECT_BOOK_H
#define FINAL_PROJECT_BOOK_H

#include <string>
#include <iostream>
#include <optional>
#include "Transaction/Date.hpp"

class Book {
public:
    enum class Genre { Fiction, NonFiction, Mystery, Science, Biography };
    enum class BookStatus { Available, CheckedOut };

protected:
    std::string title;
    std::string author;
    Genre genre;
    BookStatus status;

    std::optional<Date> checkoutDate;
    std::optional<Date> dueDate;
    std::optional<int> currentPatronId;

public:
    Book(std::string title, std::string author, Genre genre);
    virtual ~Book() = default;

    virtual void displayInfo() const;
    virtual std::string getType() { return "Unknown"; }

    void setStatus(const BookStatus s) { status = s; };
    void setCheckoutDate(const Date& date) { checkoutDate = date; }
    void setDueDate(const Date& date) { dueDate = date; }
    void setCurrentPatronId(int id) { currentPatronId = id; }

    void checkout(int patronId);
    void returnBook();

    [[nodiscard]] BookStatus getStatus() const { return status; };
    [[nodiscard]] std::string getTitle() const { return title; }
    [[nodiscard]] std::string getAuthor() const { return author; }
    [[nodiscard]] Genre getGenre() const { return genre; }
    [[nodiscard]] std::optional<Date> getCheckoutDate() const { return checkoutDate; }
    [[nodiscard]] std::optional<Date> getDueDate() const { return dueDate; }
    [[nodiscard]] std::optional<int> getCurrentPatronId() const { return currentPatronId; }
    [[nodiscard]] bool isOverdue() const;
    [[nodiscard]] int getDaysOverdue() const;

    bool operator==(const Book& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Book& b);

    static std::string genreToString(Genre g);
    static std::string bookStatusToString(BookStatus s);
    static Genre stringToGenre(const std::string& s);
};

#endif