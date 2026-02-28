#ifndef FINAL_PROJECT_DATE_HPP
#define FINAL_PROJECT_DATE_HPP

#include <string>
#include <iomanip>

class Date {
private:
    int day;
    int month;
    int year;

public:
    Date();
    Date(int day, int month, int year);

    [[nodiscard]] Date addDays(int days) const;
    [[nodiscard]] std::string toString() const;

    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;
    bool operator<(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>=(const Date& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Date& d);
};

#endif