/**
 *
 * @author  Marco Manco
 * @date    04/12/21.
 * @file    APImain.h
 * @brief
 *
 */

#ifndef CALENDAR_TODO_LIST_CPP_APIMAIN_H
#define CALENDAR_TODO_LIST_CPP_APIMAIN_H

#include "../QLabelEvent.h"
#include "../Utils/DateUtils.h"

class APImain {

public:
  explicit APImain();
  ~APImain();
  std::list<Event *> getEventByMonth(int month, int year);
};

#endif // CALENDAR_TODO_LIST_CPP_APIMAIN_H
