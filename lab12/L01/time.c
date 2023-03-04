
#include "time.h"

int TIMEget(Time time){
	return ((((time.d.year*12)+time.d.month)*30)+time.d.day)*24;
}

int TIMEcmp(Time time1,Time time2){
    return TIMEget(time1)-TIMEget(time2);
}

void TIMEprint(FILE *fp, Time time){
	fprintf(fp,"%d/%d/%d",time.d.year,time.d.month,time.d.day);
}

void TIMEread(FILE *fp,Time *time){
	if(fscanf(fp,"%d/%d/%d %d:%d",&(time->d.year),&(time->d.month),&(time->d.day),&(time->t.hour),&(time->t.min))!=5)
	{
		printf("Errore nella letture delle date");
		exit(2);
	}
}

void TIMEreadsimple(FILE *fp,Time *time){
    if(fscanf(fp,"%d/%d/%d",&(time->d.year),&(time->d.month),&(time->d.day))!=3)
    {
        printf("Errore nella letture dlele date");
        exit(2);
    }
}

Time TIMEinvalid(){
    Time time;
    time.d.year=-1;
    time.d.month=-1;
    time.d.day=-1;
    time.t.hour=-1;
    time.t.min=-1;
    return time;
}
