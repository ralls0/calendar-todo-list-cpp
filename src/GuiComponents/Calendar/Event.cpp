/**
 *
 * @author  Marco Manco
 * @date    04/12/21.
 * @file    Event.cpp
 * @brief
 *
 */

#include "Event.h"

Event::Event(Event &event) {
  this->name = event.getName();
  this->description = event.getDescription();
  this->place = event.getPlace();
  this->category = new Category(*event.getCategory());
  this->id = event.getId();
  this->start = event.getStart();
  this->end = event.getEnd();
  this->yyS = event.getYearS();
  this->mmS = event.getMonthS();
  this->ggS = event.getDayS();
  this->yyE = event.getYearE();
  this->mmE = event.getMonthE();
  this->ggE = event.getDayE();
}

Event::Event(unsigned int id, std::string name, const std::string &description,
             const std::string &place, Category *category, time_t start,
             time_t end, int ggS, int mmS, int yyS, int ggE, int mmE, int yyE) {
  this->name = name;
  this->description = description;
  this->yyE = yyE;
  this->mmE = mmE;
  this->ggE = ggE;
  this->yyS = yyS;
  this->mmS = mmS;
  this->ggS = ggS;
  this->place = place;
  if (category == NULL) {
    /* An event with a NULL category is inconsistent, it shouldn't exist */
    this->category = NULL;
    fprintf(stderr, "Event %d (%s) received NULL category.\n", id,
            name.c_str());
  } else
    this->category = new Category(*category);
  this->start = start;
  this->end = end;
  if (id == 0)
    /* The returned value from the hash function could be bigger than an
     * integer, so be careful with normal integers.
     * I use an unsigned integer to have always a positive number (also with the
     * overflow). */
    this->id = static_cast<unsigned int>(std::hash<std::string>()(
                   this->name + this->description + this->place)) +
               (this->category ? this->category->getId() : 0) +
               static_cast<unsigned int>((this->start / 1000) +
                                         (this->end - this->start));
  else
    this->id = id;
}

Event::~Event() {
  if (this->category != NULL)
    delete this->category;
}

void Event::setInvalid() { this->id = 65535; }

bool Event::isInvalid() { return this->id == 65535; }
int Event::getYearS() { return yyS; }
int Event::getDayS() { return ggS; }
int Event::getMonthS() { return mmS; }
int Event::getYearE() { return yyE; }
int Event::getDayE() { return ggE; }
int Event::getMonthE() { return mmE; }
unsigned int Event::getId() { return id; }
std::string Event::getName() { return name; }
std::string Event::getDescription() { return description; }
std::string Event::getPlace() { return place; }
Category *Event::getCategory() { return category; }
time_t Event::getStart() { return start; }
time_t Event::getEnd() { return end; }

bool Event::equals(Event &e) { return (this->id == e.getId()); }

long Event::hashcode() { return this->id; }