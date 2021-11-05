/**
 * @author      Created by Marco Manco on 16/10/21.
 *
 *  @brief      Common date and time utils.
 */

#ifndef CALENDAR_TODO_LIST_CPP_DATEUTILS_H
#define CALENDAR_TODO_LIST_CPP_DATEUTILS_H

#include <QDate>
#include <QDateTime>

#define DATEUTILS_INVALID_DAY 32

/**
 * Class
 *
 * @brief DateUtils provides (static) methods for date and time handling.
 *
 */
class DateUtils {
public:
  /**
   * @brief Returns the weekday index (1..7) from a string.
   * @param weekdayString Should be like "2FR" (2nd Friday), "-1SA" (last
   * Saturday), "MO" (Monday).
   * @note Parameter weekdayString is not case sensitive.
   * @retval Returns 0 if weekdayString is invalid.
   */
  static int getWeekdayIndexFromString(QString weekdayString);

  /**
   * @brief Returns the integer part from strings like "2FR" (2nd Friday) or
   * "-1SA" (last Saturday).
   * @note Parameter weekdayString is not case sensitive.
   * @retval Returns DATEUTILS_INVALID_DAY if weekdayString is invalid.
   * @retval Returns 0 if weekdayString contains no integer part.
   */
  static int getWeekdaySelectorFromString(QString weekdayString);

  /**
   * @brief Returns the number of occurrences of a weekday (1..7 = MO..SU) in a
   * given month.
   * @retval Returns -1 if iWeekday is invalid.
   */
  static int getWeekdayCountInMonth(int iWeekday, QDate monthDate);

  /**
   * @brief Returns a QDate of a given weekday (1-7) in a given month and with
   * respect to a given weekday selector.
   * @param iWeekDaySelector Example: -2 = second-last occurrence of iWeekday in
   * month
   * @retval Returns an invalid QDate if the questioned weekday occurrence is
   * not contained in the month (i.e. if asked for the 10th Monday)
   */
  static QDate getWeekdayOfMonth(int iWeekday, int iWeekDaySelector,
                                 QDate monthDate);

  /**
   * @brief Helper function to get the last day of a year+month.
   */
  static int lastDayOfMonth(int year, int month);
};

#endif // CALENDAR_TODO_LIST_CPP_DATEUTILS_H
