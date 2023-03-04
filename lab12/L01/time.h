#ifndef STIME_H
#define STIME_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int year,month,day;
}Date;

typedef struct {
    int hour,min;
}DayTime;

typedef struct {
	Date d;
	DayTime t;
}Time;

int TIMEcmp(Time time1,Time time2);
int TIMEget(Time time);
void TIMEprint(FILE *fp, Time time);
void TIMEread(FILE *fp,Time *time); //chiede data e ora, per leggere il file
void TIMEreadsimple(FILE *fp,Time *time); //chiede solo data, uso nel main

Time TIMEinvalid();

#endif
