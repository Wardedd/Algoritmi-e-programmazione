#include "quot.h"

//2018/01/01 00:00 100 3
//2018/01/01 00:00

void QUOTinit(Quot *quot,Time time){
	quot->time=time;
	quot->val=0;
	quot->num=0;	
}

void QUOTprint(FILE *fp, Quot quot){
    if(QUOTisNull(quot))
    {
        fprintf(fp,"Quotazione giornaliera non esistente");
    }else{
        TIMEprint(fp, quot.time);
        fprintf(fp, " value: %f", QUOTgetVal(quot));
    }
}


void QUOTupdate(FILE *fp,Quot *quot){
	int num;
	float val;
	fscanf(fp,"%f %d ",&val,&num);
	quot->val+=val*num;
	quot->num+=num;
}

Time Q_KEYget(Quot quot){
	return quot.time;
}

int Q_KEYcmp(Time time1,Time time2){
	return TIMEcmp(time1,time2);
}

float QUOTvalCmp(float key1,float key2){
    return key1-key2;
}

float QUOTgetVal(Quot quot){
    return quot.val/(float)quot.num;
}

Quot QUOTgetNull(){
	Quot quot={val:-1,num:-1,time:TIMEinvalid()};
	return quot;
}

int QUOTisNull(Quot quot)
{
    if (Q_KEYcmp((Q_KEYget(quot)),Q_KEYget(QUOTgetNull()))==0)
        return 1;
    return 0;
}
