#include <stdio.h>
#include "libRisika.h"

int main() {
    FILE *f=fopen("facolta.txt","r");
    int i=-1;
    int j=0;
    char c;
    Territorio t[26];
    char ce[24];

    if(f==NULL) {
        printf("caaaazo");
        exit(1);
    }

    while(fscanf(f, "%c", &c)!=EOF){
        if(c=='-')
            *ce='\0';

        if(c!='-') {
            if(c=='0' || c=='1' || c=='2' || c=='3' || c=='4' || c=='5') {
                i = atoi(&c);

            }else
                *ce=c;
            if(i!=-1) {
                printf(" - %d\n",i);
                strcpy(t[j].nome,ce);
                t[j].id=j;
                t[j].f=i;
                j++;
                i=-1;
            }else
                printf("%c", *ce);
        }








    }
fclose(f);

    for(i=0;i<26;i++){
        printf("%d %s-%d\n",t[i].id,t[i].nome,t[i].f);
    }

    //gioco();
    return 0;
}