#include "APImain.h"


APImain::APImain(){

}

APImain::~APImain(){};

list<Event*> APImain::getEventByMonth(int month, int year){
    list<Event*> event_list;
    list<Event*> filter_list;
    time_t today,end;
    struct tm *birthday;

    for(int i=0;i<12;i++){

        time(&today);
        birthday = localtime(&today);
        birthday->tm_mon = i;
        birthday->tm_mday = i+1;
        birthday->tm_year = 2021-1900;

        end = mktime(birthday);
        if(i==11){
            Event *a = new Event(i+1, "Festune","D11" ,"casaMia", new Category(3,"enigma","green"), end, end,i+1,i+1,2021,i+1,i+1,2021);
             Event *b = new Event(i+1, "Festune","D331" ,"casaMia", new Category(3,"enigma","green"), end, end,i+1,i+1,2021,i+1,i+1,2021);
              Event *c = new Event(i+1, "Festune","D13" ,"casaMia", new Category(3,"enigma","green"), end, end,i+1,i+1,2021,i+1,i+1,2021);
             event_list.push_front(a);
             event_list.push_front(b);
             event_list.push_front(c);
        }else{
            Event *a = new Event(i+1, "Festune","" ,"casaMia", new Category(3,"enigma","green"), end, end,i+1,i+1,2021,i+1,i+1,2021);
             event_list.push_front(a);
        }




    }
    for(Event *t: event_list){
        //getstart torna un timet
        if(t->getMonthS()==month && t->getYearS() == year)
            filter_list.push_front(t);
    }

    return filter_list;
}
