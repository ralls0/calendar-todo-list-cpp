/**
 *
 * @author  Marco Manco
 * @date    04/12/21.
 * @file    Category.h
 * @brief
 *
 */

#ifndef CALENDAR_TODO_LIST_CPP_CATEGORY_H
#define CALENDAR_TODO_LIST_CPP_CATEGORY_H

#include <iostream>

class Category {
private:
  unsigned int id;
  std::string name;
  std::string color;

public:
  Category(Category &category);

  Category(unsigned int id, const std::string &name, const std::string &color);

  ~Category();

  std::string getName();
  std::string getColor();
  unsigned int getId();

  bool equals(Category &c);
  long hashcode();
};

#endif // CALENDAR_TODO_LIST_CPP_CATEGORY_H
