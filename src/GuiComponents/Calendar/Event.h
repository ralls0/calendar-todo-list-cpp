/**
 *
 * @author  Marco Manco
 * @date    04/12/21.
 * @file    Event.h
 * @brief
 *
 */

#ifndef CALENDAR_TODO_LIST_CPP_EVENT_H
#define CALENDAR_TODO_LIST_CPP_EVENT_H

#include "Category.h"
#include <ctime>
#include <iostream>


/* A todo is defined as an event with special dates, they are the following:
 * start = end = 29/01/2105 09:40 , the UTC timestamp is: 4262665200
 * Note: this value is incompatible for 32bit systems due to the 2038 year bug
 * (Reference: https://en.wikipedia.org/wiki/Year_2038_problem)
 * The todo is only an high-level concept managed by the logic view, thus the
 * persistence part consider them as normal events. */
//#define BIT32
#ifndef BIT32
// As explained above, this is for 64 bit systems
#define TODO_DATE 4262665200
#else
// 2147437133 is the 18 Jan 2038, the maximum value representable with a 32 bit
// system
#define TODO_DATE 2147437133
#endif

class Event {
public:
  Event(Event &event);

  Event(unsigned int id, std::string name, const std::string &description,
        const std::string &place, Category *category, time_t start, time_t end,
        int ggS = 0, int mmS = 0, int yyS = 0, int ggE = 0, int mmE = 0,
        int yyE = 0);

  ~Event();

  void setInvalid();

  bool isInvalid();
  int getYearS();
  int getDayS();
  int getMonthS();
  int getYearE();
  int getDayE();
  int getMonthE();
  unsigned int getId();
  std::string getName();
  std::string getDescription();
  std::string getPlace();
  Category *getCategory();
  time_t getStart();
  time_t getEnd();

  bool equals(Event &e);

  long hashcode();

private:
  unsigned int id;
  std::string name;
  std::string description;
  std::string place;
  Category *category;
  /* Timestamp */
  time_t start;
  time_t end;
  int ggS;
  int mmS;
  int yyS;
  int ggE, mmE, yyE;
};

#endif // CALENDAR_TODO_LIST_CPP_EVENT_H
