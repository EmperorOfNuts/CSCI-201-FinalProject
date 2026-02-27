#ifndef FINAL_PROJECT_BOOK_H
#define FINAL_PROJECT_BOOK_H

#include <string>
#include <iostream>

class Book {
public:
    enum class Genre { Fiction, NonFiction, Mystery, Science, Biography };
    enum class BookStatus { Available, CheckedOut };

protected:
    std::string title;
    std::string author;
    Genre genre;
    BookStatus status;

public:
    Book(std::string title, std::string author, Genre genre);
    virtual ~Book() = default;

    virtual void displayInfo() const;
    virtual std::string getType() { return "Unknown"; }

    void setStatus(const BookStatus s) { status = s; };
    [[nodiscard]] BookStatus getStatus() const { return status; };
    [[nodiscard]] std::string getTitle() const { return title; }
    [[nodiscard]] std::string getAuthor() const { return author; }
    [[nodiscard]] Genre getGenre() const { return genre; }

    bool operator==(const Book& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Book& b);

    static std::string genreToString(Genre g);
    static std::string bookStatusToString(BookStatus s);
    static Genre stringToGenre(const std::string& s);
};

#endif