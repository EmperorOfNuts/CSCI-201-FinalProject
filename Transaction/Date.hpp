#ifndef FINAL_PROJECT_DATE_HPP
#define FINAL_PROJECT_DATE_HPP

#include <string>
#include <iomanip>
#include <ctime>

class Date {
private:
    int day;
    int month;
    int year;

public:
    Date();
    Date(int day, int month, int year);

    [[nodiscard]] std::string toString() const;
    [[nodiscard]] Date addDays(int days) const;

    friend std::ostream& operator<<(std::ostream& os, const Date& d);
};

#endif