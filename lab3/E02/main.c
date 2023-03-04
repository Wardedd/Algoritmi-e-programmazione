#include <stdio.h>

/*IEEE736
 * Long double 80 bit con 48 bit di padding
 * https://en.wikipedia.org/wiki/Extended_precision#x86_extended_precision_format
 * GNU C Compiler
 * e 128bit no padding IEEE754
 */

//rendo little endian in big endian con la funzione getNumInBytes*/

void getNumInBytes(void *p, int size, int bigEndian,char data[size*8]);
int isBigEndian();
void stampaCodifica (void *p, int size, int bigEndian);
void byteToBinary(unsigned char p,char bin[9]); //ritorna 8 bit in un vettore char, msb
void printSubStr(char *str,int start,int end);
int calcPaddingLongDouble(int bigEndian); //ritorna il padding del long double

int main(){
    float af;
    double ad;
    long double ald;
    long double tmp;
    int bigEndian=isBigEndian();

    printf("Byte - bit\n");
    printf("Af: %ld - %ld \nAd: %ld - %ld\nAld: %ld - %ld\n",sizeof(af),sizeof(af)*8,sizeof(ad),sizeof(ad)*8,sizeof(ald),sizeof(ald)*8);

    scanf("%Lf",&tmp);
    af=tmp;
    ad=tmp;
    ald=tmp;

    printf("Float:\n");
    stampaCodifica(&af,sizeof(af),bigEndian);
    printf("Double:\n");
    stampaCodifica(&ad,sizeof(ad),bigEndian);
    printf("Long double:\n");
    stampaCodifica(&ald,sizeof(ald),bigEndian);

    return 0;
}

int isBigEndian(){
    unsigned short num=1;
    unsigned char *p=&num;
    if(p[0]==1)
        return 0;
    return 1;
}

void byteToBinary(unsigned char p,char bin[8]){ //ritorna un byte in binario (MSb) dentro il vettore bin
    int i,num=p;

    for( i=7;i>=0 && num>=1;i--)
    {
        bin[i]=num%2+48;
        num/=2;
    }
    for(;i>=0;i--)
        bin[i]='0';
}

void getNumInBytes(void *p, int size, int bigEndian,char data[size*8]){ //ritorna il numero nel suo formato binario dentro data, formattato in Big Endian (indicizzando i byte al contrario quando ci troviamo in Little Endian)
    unsigned char *cp = p;
    int index,toAdd;

    if(bigEndian) {
        index=0;toAdd=1;
    }else
    {
        index=size-1;
        toAdd=-1;
    }

    for(int i=0;i<size;i++){
        byteToBinary(cp[index],&(data[i*8]));
        index+=toAdd;
    }
}

void stampaCodifica(void *p, int size, int bigEndian){
    unsigned char *cp = p;
    char data[size*8]; //vettore che conterrà il vettore con tutti i bit del numero immesso da tastiera
    unsigned short padding,indexSign,startm,endm,starte,ende;

    getNumInBytes(cp,size,bigEndian,data);

    switch(size){

        case 4://caso float 32 bit
            indexSign=0;

            starte=1;
            ende=8;
            startm=9;
            endm=31;
            break;
        case 8://caso double 64 bit
            indexSign=0;

            starte=1;
            ende=11;
            startm=12;
            endm=63;
            break;
        case 16:
            padding = calcPaddingLongDouble(bigEndian);

            if(padding==47){ //caso long double 128 bit (48 padding, 80bit)
                indexSign=48;

                starte=49;
                ende=63;
                startm=64;
                endm=127;
            }
            else if (padding==-1){ //caso 128bit no padding IEEE754
                indexSign=0;

                starte=1;
                ende=15;
                startm=16;
                endm=127;
            }
            else{
                printf("Non sono riuscito a gestire il tuo formato long double");
                starte=0;startm=0;ende=-1;endm=-1;
            }

            break;
        default:
            starte=0;startm=0;ende=-1;endm=-1;
    }

    printf("Segno: %c\n",data[indexSign]);
    printf("Esponente: ");
    printSubStr(data,starte,ende);
    printf("\nMantissa: ");
    printSubStr(data,startm,endm);
    printf("\n");

}

int calcPaddingLongDouble(int bigEndian){ //ritorna l'indice finale di padding per long double
    long double a=1;
    long double b=-1;
    int i,found=0;
    char dataA[sizeof(a)*8];
    char dataB[sizeof(b)*8];

    getNumInBytes(&a,sizeof(a),bigEndian,dataA);
    getNumInBytes(&b,sizeof(b),bigEndian,dataB);
    //i dati dentro dataA e dataB sono in bigEndian
    for(i=sizeof(a)*8-1;i>=0 && found==0;i--)
    {
        if(dataA[i]!=dataB[i]) //abbiamo trovato il segno
            found=1;
    }//i=indice bit prima del segno, ovvero dove finisce il padding

    return i;
}

void printSubStr(char *str,int start,int end){
    for(int i=start;i<=end;i++){
        printf("%c",str[i]);
    }
}
