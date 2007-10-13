// 
// File:   main.h
// Author: willow
//
// Created on 9 Октябрь 2007 г., 20:32
//

#ifndef _MAIN_H
#define	_MAIN_H

#include "fltk/Browser.h"
#include <string>
#include <vector>
using namespace std;

struct City{
	string city, state, datapath;
};

typedef vector<City> LocRec;

void refill_all();
void do_sort();
void get_city_list(LocRec &v);
void refresh_lbsize();
void move_record(LocRec &src, LocRec &dest, int index);
void set_year(const char *str);
void refill_list(fltk::Browser *lst, LocRec &v);
void refill_all();
void get_city_list(LocRec &v);
bool comparator(const City &lhs, const City &rhs);



int run_exe(const char *cmd);


#endif	/* _MAIN_H */

