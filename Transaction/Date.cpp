#include "Date.hpp"
#include <iostream>
#include <ctime>
#include <sstream>
#include <stdexcept>
#include <iomanip>

Date::Date() {
    const time_t now = time(nullptr);
    const tm* ltm = localtime(&now);

    day = ltm->tm_mday;
    month = 1 + ltm->tm_mon;
    year = 1900 + ltm->tm_year;
}

Date::Date(const int day, const int month, const int year)
    : day(day), month(month), year(year) {
    if (month < 1 || month > 12 || day < 1 || day > 31) throw std::invalid_argument("Invalid date.");
}

Date Date::addDays(const int days) const {
    // Convert to time_t, add days, convert back
    tm timeInfo = {};
    timeInfo.tm_mday = day;
    timeInfo.tm_mon = month - 1;
    timeInfo.tm_year = year - 1900;

    time_t rawTime = mktime(&timeInfo);
    rawTime += days * 24 * 60 * 60;

    tm* newTime = localtime(&rawTime);

    return {newTime->tm_mday, newTime->tm_mon + 1, newTime->tm_year + 1900};
}

std::string Date::toString() const {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << day << "/"
        << std::setw(2) << std::setfill('0') << month << "/"
        << year;
    return oss.str();
}

// Can't believe I ACTUALLY had to overload this just for overdue dates
bool Date::operator==(const Date& other) const { return day == other.day && month == other.month && year == other.year; }

bool Date::operator!=(const Date& other) const { return !(*this == other); }

bool Date::operator<(const Date& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}

bool Date::operator>(const Date& other) const { return other < *this; }

bool Date::operator<=(const Date& other) const { return !(*this > other); }

bool Date::operator>=(const Date& other) const { return !(*this < other); }

std::ostream& operator<<(std::ostream& os, const Date& d) {
    os << d.toString();
    return os;
}