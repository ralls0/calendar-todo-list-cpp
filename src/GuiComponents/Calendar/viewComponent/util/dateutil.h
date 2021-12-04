#ifndef DATEUTIL_H
#define DATEUTIL_H

#include <time.h>
#include <string>
#include <list>

#include "../model/date.h"

#define SECONDS_IN_1DAY 86400

using namespace std;

class DateUtil
{
public:
    DateUtil();
    static Date get_current_date();
    static string get_literal_month(int m); /* 1 => January, ... */
    static string numeric2literal_day_of_week(int d); /* Monday => 1, Tuesday => 2, ... */
    static int literal2numeric_day_of_week(const string &d); /* 1 => Monday, 2 => Tuesday, ... */
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

#endif
