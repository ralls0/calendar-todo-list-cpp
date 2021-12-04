#ifndef EVENT_H
#define EVENT_H

#include <ctime>
#include<string>
#include "category.h"

/* A todo is defined as an event with special dates, they are the following:
 * start = end = 29/01/2105 09:40 , the UTC timestamp is: 4262665200
 * Note: this value is incompatible for 32bit systems due to the 2038 year bug (Reference: https://en.wikipedia.org/wiki/Year_2038_problem)
 * The todo is only an high-level concept managed by the logic view, thus the persistence part consider them as normal events. */
//#define BIT32
#ifndef BIT32
// As explained above, this is for 64 bit systems
#define TODO_DATE 4262665200
#else
// 2147437133 is the 18 Jan 2038, the maximum value representable with a 32 bit system
#define TODO_DATE 2147437133
#endif

class Event
{
private:
    unsigned int id;
    string name;
    string description;
    string place;
    Category *category;
    /* Timestamp */
    time_t start;
    time_t end;
    int ggS;
    int mmS;
    int yyS;
    int ggE,mmE,yyE;

public:
    Event(Event &event) {
        this->name = event.getName();
        this->description = event.getDescription();
        this->place = event.getPlace();
        this->category = new Category(*event.getCategory());
        this->id = event.getId();
        this->start = event.getStart();
        this->end = event.getEnd();
        this->yyS=event.getYearS();
        this->mmS=event.getMonthS();
        this->ggS=event.getDayS();
        this->yyE=event.getYearE();
        this->mmE=event.getMonthE();
        this->ggE=event.getDayE();
    }

    Event(unsigned int id, string name, const string &description, const string &place, Category *category, time_t start, time_t end, int ggS = 0, int mmS = 0, int yyS = 0, int ggE=0,int mmE=0, int yyE=0) {
        this->name = name;
        this->description = description;
        this->yyE= yyE;
        this -> mmE = mmE;
        this -> ggE = ggE;
        this->yyS = yyS;
        this -> mmS = mmS;
        this -> ggS = ggS;
        this->place = place;
        if (category == NULL) {
            /* An event with a NULL category is inconsistent, it shouldn't exist */
            this->category = NULL;
            fprintf(stderr, "Event %d (%s) received NULL category.\n", id, name.c_str());
        } else
            this->category = new Category(*category);
        this->start = start;
        this->end = end;
        if (id == 0)
            /* The returned value from the hash function could be bigger than an integer, so be careful with normal integers.
             * I use an unsigned integer to have always a positive number (also with the overflow). */
            this->id = static_cast<unsigned int> (hash<string>()(this->name + this->description + this->place)) + (this->category ? this->category->getId() : 0) + static_cast<unsigned int> ((this->start / 1000) + (this->end - this->start));
        else
            this->id = id;
    }

    ~Event() {
        if (this->category != NULL) delete this->category;
    }

    void setInvalid() {
        this->id = 65535;
    }

    bool isInvalid() {
        return this->id == 65535;
    }
    int getYearS() {return yyS;}
    int getDayS() {return ggS;}
    int getMonthS() {return mmS;}
    int getYearE() {return yyE;}
    int getDayE() {return ggE;}
    int getMonthE() {return mmE;}
    unsigned int getId() { return id; }
    string getName() { return name; }
    string getDescription() { return description; }
    string getPlace() { return place; }
    Category *getCategory() { return category; }
    time_t getStart() { return start; }
    time_t getEnd() { return end; }

    bool equals(Event &e) {
        return (this->id == e.getId());
    }

    long hashcode() {
        return this->id;
    }
};

#endif // EVENT_H
