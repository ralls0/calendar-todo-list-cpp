/**
 *
 * @author  Marco Manco
 * @date    28/11/21
 * @file    DateUtils.cpp
 * @brief
 *
 */

#include <QDebug>
#include <QtCore/qregularexpression.h>

#include "DateUtils.h"

int DateUtils::getWeekdayIndexFromString(QString weekdayString) {
  int iRet = 0;

  if (weekdayString.endsWith("MO", Qt::CaseInsensitive)) {
    iRet = 1;
  } else if (weekdayString.endsWith("TU", Qt::CaseInsensitive)) {
    iRet = 2;
  } else if (weekdayString.endsWith("WE", Qt::CaseInsensitive)) {
    iRet = 3;
  } else if (weekdayString.endsWith("TH", Qt::CaseInsensitive)) {
    iRet = 4;
  } else if (weekdayString.endsWith("FR", Qt::CaseInsensitive)) {
    iRet = 5;
  } else if (weekdayString.endsWith("SA", Qt::CaseInsensitive)) {
    iRet = 6;
  } else if (weekdayString.endsWith("SU", Qt::CaseInsensitive)) {
    iRet = 7;
  } else {
    iRet = 0;
  }

  return iRet;
}

int DateUtils::getWeekdaySelectorFromString(QString weekdayString) {
  int iRet = 0;
  bool bConversion = false;
  weekdayString.remove(QRegularExpression("[MOTUWEHFRSAUmotuwehfrsau]"));

  if (weekdayString.isEmpty()) {
    return 0;
  }

  iRet = weekdayString.toInt(&bConversion);

  if (false == bConversion) {
    qDebug() << "ERROR could not convert" << weekdayString << "to integer";
    iRet = DATEUTILS_INVALID_DAY;
  }

  return iRet;
}

int DateUtils::getWeekdayCountInMonth(int iWeekday, QDate monthDate) {
  if ((iWeekday < 1) || (iWeekday > 7)) {
    return -1;
  }

  int iWeekDayCount[7] = {4, 4, 4, 4, 4, 4, 4};
  QDate monthStart;
  QDate monthEnd;

  monthStart.setDate(monthDate.year(), monthDate.month(), 1);
  monthEnd.setDate(monthDate.year(), monthDate.month(),
                   monthDate.daysInMonth());

  int firstWeekdayInMonth = monthStart.dayOfWeek();
  int lastWeekdayInMonth = monthEnd.dayOfWeek();

  for (int index = 1; index <= 7; index++) {
    if (monthDate.daysInMonth() > 28) {
      if ((index >= firstWeekdayInMonth) && (index <= lastWeekdayInMonth)) {
        iWeekDayCount[(index - 1)] = 5;
      }
    }
  }

  return iWeekDayCount[(iWeekday - 1)];
}

QDate DateUtils::getWeekdayOfMonth(int iWeekday, int iWeekDaySelector,
                                   QDate monthDate) {
  QDate testDate = QDate(monthDate);
  QDate datRet = QDate(); // constructs a null date

  if ((iWeekday < 1) || (iWeekday > 7)) {
    return datRet;
  }

  // begin with first day in month
  testDate.setDate(testDate.year(), testDate.month(), 1);

  // if the testDate doesn't start on the questioned weekday, jump forward to
  // the next weekday occurrence
  if (testDate.dayOfWeek() != iWeekday) {
    int weekdayDiff = iWeekday - testDate.dayOfWeek();
    if (weekdayDiff < 0) {
      weekdayDiff += 7;
    }
    testDate = testDate.addDays(weekdayDiff);
  }

  // we are now at the first occurrence of the questioned weekday of the month

  int weekdayCount = DateUtils::getWeekdayCountInMonth(iWeekday, testDate);

  if (0 > iWeekDaySelector) {
    /*
    a  b  c  d  e
    1  2  3  4
   -4 -3 -2 -1
    1  2  3  4  5
   -5 -4 -3 -2 -1
    */
    iWeekDaySelector += (weekdayCount + 1);
  }

  if (iWeekDaySelector < 1) {
    // questioned weekday not contained in month
    return datRet;
  }

  for (int iWeekdayOccurrence = 1; iWeekdayOccurrence <= weekdayCount;
       iWeekdayOccurrence++) {
    if (iWeekDaySelector == iWeekdayOccurrence) {
      // found the questioned nth weekday
      datRet = testDate;
      break;
    } else {
      // try with next week
      testDate = testDate.addDays(7);
    }
  }

  return datRet;
}

int DateUtils::lastDayOfMonth(int year, int month) {
  QDate date;
  date.setDate(year, month, 1);
  return date.daysInMonth();
}
