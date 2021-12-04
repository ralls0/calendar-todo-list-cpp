/**
 *
 * @author  Marco Manco
 * @date    04/12/21.
 * @file    Date.cpp
 * @brief
 *
 */

#include "Date.h"

Date::Date(int mday, int wday, int month, int year) {
  this->mday = mday;
  this->wday = wday;
  this->month = month;
  this->year = year;
}
int Date::getMonthDay() { return mday; }
int Date::getWeekDay() { return wday; }
int Date::getMonth() { return month; }
int Date::getYear() { return year; }
void Date::setMonthDay(int mday) { this->mday = mday; }
void Date::setWeekDay(int wday) { this->wday = wday; }
void Date::setMonth(int month) { this->month = month; }
void Date::setYear(int year) { this->year = year; }
// TODO: maybe it can be useful to add the method: isHoliday()

int Date::compareTo(Date &d) {
  int ret;
  ret = this->year - d.getYear();
  if (ret != 0)
    return ret;
  else { // Same year, compare months
    ret = this->month - d.getMonth();
    if (ret != 0)
      return ret;
    else { // Same year and month, compare days
      ret = this->mday - d.getMonthDay();
      return ret;
    }
  }
}

Date &Date::operator=(Date d) {
  this->mday = d.getMonthDay();
  this->wday = d.getWeekDay();
  this->month = d.getMonth();
  this->year = d.getYear();
  return *this;
}

bool Date::operator==(Date &d) {
  return (this->mday == d.getMonthDay()) && (this->wday == d.getWeekDay()) &&
         (this->month == d.getMonth()) && (this->year == d.getYear());
}

std::string Date::toString(bool weekday) {
  /* In this project the standard format for dates is: d/m/y */
  char sdate[11];
  snprintf(sdate, 11, "%02d/%02d/%d", this->mday, this->month, this->year);
  std::string ret(sdate);
  if (weekday)
    ret += " Week-day: " + std::to_string(this->wday);
  return ret;
}