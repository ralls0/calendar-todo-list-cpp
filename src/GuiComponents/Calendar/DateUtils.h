/**
 *
 * @author  Marco Manco
 * @date    04/12/21.
 * @file    DateUtils.h
 * @brief
 *
 */

#ifndef CALENDAR_TODO_LIST_CPP_DATEUTILS_H
#define CALENDAR_TODO_LIST_CPP_DATEUTILS_H

#include <QDebug>
#include <list>
#include <time.h>

#include "Date.h"

#define SECONDS_IN_1DAY 86400


class DateUtil {
public:
  DateUtil();
  static Date get_current_date();
  static std::string get_literal_month(int m); /* 1 => January, ... */
  static std::string
  numeric2literal_day_of_week(int d); /* Monday => 1, Tuesday => 2, ... */
  static int literal2numeric_day_of_week(
      const std::string &d); /* 1 => Monday, 2 => Tuesday, ... */
  static int get_days_in_month(int month, int year);
  static Date date_from_timestamp(time_t timestamp);
  static Date get_first_day_of_month(Date &date);
  static Date get_last_day_of_month(Date &date);
  static Date increase_month(Date date);
  static Date decrease_month(Date date);
  static Date increase_day(Date date);
  static Date decrease_day(Date date);
  static Date increase_year(Date date);
  static Date decrease_year(Date date);
  static bool is_leap(int year);
};

#endif // CALENDAR_TODO_LIST_CPP_DATEUTILS_H
