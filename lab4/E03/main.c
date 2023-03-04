#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define STR_LENGTH 51
#define REGEXP_LENGTH 51

typedef enum {badRegex=-1,tutti=0,specifico=1,tranne=2,piccolo=3,grande=4,lettera=5}regexState;

/*
 * uso regexState per avere uno stato che mi dice cosa devo cercare (cosa mi aspetto) nella stringa.
 * questo stato cambia aumentando l'indice della regexp
 * */

char *cercaRegexp(char *src, char *regexp); //wrapper, suddivido la frase in piu' parole
char *cercaRegexpParola(char *str,char *regexp);
regexState getRegState (char *regexp,int *index);

int main() {
    char str[STR_LENGTH+1];
    char regexp[REGEXP_LENGTH+1];
    char *foundStr;
    printf("Lunghezza massima stringhe %d, regexp %d\n",STR_LENGTH,REGEXP_LENGTH);
    printf("Inserire la stringa: ");
    gets(str);
    printf("Inserire la regexp: ");
    scanf("%s",regexp);

    foundStr=cercaRegexp(str,regexp);

    if(foundStr!=NULL)
        printf("%s",foundStr);
    else
        printf("Nessun risultato trovato");

    return 0;
}

char *cercaRegexp(char *src, char *regexp){

    char *currStr,*foundStr;
    currStr=strtok(src," ");

    while(currStr!=NULL){ //itero cercaRegexpParola per ogni parola
        if(strlen(currStr)<=STR_LENGTH){
            if((foundStr=cercaRegexpParola(currStr,regexp))!=NULL) //ritorno la prima occorrenza
            {
                return foundStr;
            }
        }
        currStr=strtok(NULL," ");
    }
    return NULL;
}

char *cercaRegexpParola(char *str,char *regexp){
    int regI = 0;
    int currI = 0, oldI = 0;
    int regState;
    int strLength = strlen(str);
    int regLength = strlen(regexp);
    int fail = 0;
    char *ptmp;
    int i,tmp;

    while (regI < regLength && currI < strLength) {
        regState = getRegState(regexp, &regI);

        switch (regState) {
            case badRegex:
                return NULL;

            case lettera:
                if (regexp[regI-1] != str[currI])
                    fail = 1;
                break;
            case piccolo:
                if (!islower(str[currI]))
                    fail = 1;
                break;
            case grande:
                if (!isupper(str[currI]))
                    fail = 1;
                break;
            case tutti:
                break;
            case specifico: //[

                ptmp = strpbrk(regexp+regI, "]"); //indirizzo cella con ]
                tmp = ptmp-regexp; //tmp = indice regexp dopo ultima lettera alfabetica

                //se il carattere nella striga è diverso  da quelli interni a [...], fail

                fail = 1;
                for (i = regI; i < tmp && fail == 1; i++) {
                    if (regexp[i] == str[currI])
                        fail = 0;
                }
                regI = tmp + 1;
                break;

            case tranne://[^
                ptmp = strpbrk(regexp+regI, "]");
                tmp = ptmp-regexp; //tmp = indice regexp dopo ultima lettera alfabetica
                fail = 0;

                //se il caratteri nella striga sono uguali a quelli interni a [^...], fail

                for (i = regI; i < tmp && fail == 0; i++) {
                    if (regexp[i] == str[currI])
                        fail = 1;
                }
                regI = tmp + 1;
                break;
        }
        currI++;

        if (fail) { //resetto, facendo ripartire la stringa con il carattere successivo

            oldI += 1;
            currI = oldI;

            regI = 0;
            fail = 0;
        }
    }

    if (regI == regLength && !fail)
    {
        char resStr[STR_LENGTH];
        for(i=0;i<currI-oldI;i++)
            resStr[i]=str[i+oldI];
        resStr[i]='\0';
        char *p = resStr;
        return p;
    }

    return NULL;
}

regexState getRegState(char *regexp,int *index){

    switch(regexp[(*index)++]){
        case '[': //tmp =1 -> tranne

            if(regexp[(*index)]=='^')
            {
                (*index)++;
                return tranne;
            }

            return specifico;

        case '\\':
            switch(regexp[(*index)++])
            {
                case 'a':
                    return piccolo;
                case 'A':
                    return grande;
            }

            return badRegex;

        case '.':
            return tutti;

        default:
            return lettera;

    }
}