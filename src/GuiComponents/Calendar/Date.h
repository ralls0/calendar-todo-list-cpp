/**
 *
 * @author  Marco Manco Davide Manco
 * @date    04/12/21.
 * @file    Date.h
 * @brief
 *
 */

#ifndef CALENDAR_TODO_LIST_CPP_DATE_H
#define CALENDAR_TODO_LIST_CPP_DATE_H

#include <iostream>
#include <list>
#include <string>
#include <time.h>

class Date {

public:
  Date() = default;

  Date(int mday, int wday, int month, int year);
  int getMonthDay();
  int getWeekDay();
  int getMonth();
  int getYear();
  void setMonthDay(int mday);
  void setWeekDay(int wday);
  void setMonth(int month);
  void setYear(int year);
  // TODO: maybe it can be useful to add the method: isHoliday()

  int compareTo(Date &d);

  Date &operator=(Date d);

  bool operator==(Date &d);

  std::string toString(bool weekday);

private:
  int mday; // month day
  int wday; // week day
  int month;
  int year;
};

#endif // CALENDAR_TODO_LIST_CPP_DATE_H
