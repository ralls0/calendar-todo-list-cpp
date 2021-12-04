#ifndef DATE_H
#define DATE_H

#include <time.h>
#include <string>
#include <list>

using namespace std;

class Date {
private:
    int mday; //month day
    int wday; //week day
    int month;
    int year;
public:
    Date() {}

    Date(int mday, int wday, int month, int year) {
        this->mday = mday;
        this->wday = wday;
        this->month = month;
        this->year = year;
    }
    int getMonthDay() { return mday; }
    int getWeekDay() { return wday; }
    int getMonth() { return month; }
    int getYear() { return year; }
    void setMonthDay(int mday) { this->mday = mday; }
    void setWeekDay(int wday) { this->wday = wday; }
    void setMonth(int month) { this->month = month; }
    void setYear(int year) { this->year = year; }
    //TODO: maybe it can be useful to add the method: isHoliday()

    int compareTo (Date &d) {
        int ret;
        ret = this->year - d.getYear();
        if (ret != 0) return ret;
        else { //Same year, compare months
            ret = this->month - d.getMonth();
            if (ret != 0) return ret;
            else { //Same year and month, compare days
                ret = this->mday - d.getMonthDay();
                return ret;
            }
        }
    }

    Date& operator=(Date d) {
        this->mday = d.getMonthDay();
        this->wday = d.getWeekDay();
        this->month = d.getMonth();
        this->year = d.getYear();
        return *this;
    }

    bool operator==(Date &d) {
        return (this->mday == d.getMonthDay()) &&
               (this->wday == d.getWeekDay()) &&
               (this->month == d.getMonth()) &&
               (this->year == d.getYear());
    }

    string toString(bool weekday) {
        /* In this project the standard format for dates is: d/m/y */
        char sdate[11];
        snprintf(sdate, 11, "%02d/%02d/%d", this->mday, this->month, this->year);
        string ret (sdate);
        if (weekday)
            ret += " Week-day: " + to_string(this->wday);
        return ret;
    }
};

#endif
