/**
 *
 * @author  Marco Manco
 * @date    04/12/21.
 * @file    Category.cpp
 * @brief
 *
 */

#include "Category.h"

Category::Category(Category &category) {
  this->name = category.getName();
  this->color = category.getColor();
  this->id = category.getId();
}

Category::Category(unsigned int id, const std::string &name,
                   const std::string &color) {
  this->name = name;
  this->color = color;
  if (id == 0)
    this->id = static_cast<unsigned int>(
        std::hash<std::string>()(this->name + this->color));
  else
    this->id = id;
}

Category::~Category() {}

std::string Category::getName() { return name; }
std::string Category::getColor() { return color; }
unsigned int Category::getId() { return id; }

bool Category::equals(Category &c) { return (this->id == c.getId()); }

long Category::hashcode() { return static_cast<long>(this->id); }