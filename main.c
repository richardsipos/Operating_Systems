#include<stdio.h>
#include <string.h>
#include <stdlib.h>


//a nevet meg modositani is lehet.
//#define MAX 500;

int week[]={5,4,3,2,1,4,0};
char weekWorkers[7][500];


int applyIsSuccessfull(char* name,int dayIndex){ //0 at add vissza ha helyes maskep -1;

    if(week[dayIndex]<=0){
        printf("Nem sikerult helyett talalni a het %d. napjara!\n",dayIndex+1);
        return -1;
    }else{
        if(strstr(weekWorkers[dayIndex],name)){
            printf("Hiba! Mar fel vagy irva erre a napra!");
        }else{
            week[dayIndex]--;
            if(strlen(weekWorkers[dayIndex])>1){
                strcat(weekWorkers[dayIndex]," ");
            }
            strcat(weekWorkers[dayIndex],name);
            
            return 0;
        }
        
    }
    return -1;
}



int delete(char *name,int dayIndex){
    
    size_t len = strlen(name);
    char *p;
    p=strstr( weekWorkers[dayIndex],name);
    if(p){
        strcpy( p,p+len+1);
        week[dayIndex]++;
    }
    return 0;
}


int main(){
    //elokeszuletek
    printf("Az Igyal komam szoleszetben a segitsegedre van szukseg!\n\n");
    int gameIsOn=1;

    FILE *fp;
    fp = fopen("database.txt", "r+");

    //read from file
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int counter=0;
    while (getline(&line, &len, fp)!=EOF) {
        line[strcspn(line, "\r\n")] = 0;
        printf("%d %s\n",counter+1, line);
        
        strcat(weekWorkers[counter],line);
        counter++;
    }
    free(line);
    printf("\n");
    
    fclose(fp);

    //program
    while(gameIsOn){
        printf("Menu:\n");
        printf("1. Jelentkezo hozzaadasa:\n");
        printf("2. Jelentkezo adatainak a modositasa:\n");
        printf("3. Jelentkezo torlese:\n");
        printf("4. Teljes lista betoltese:\n");
        printf("5. Kilepes es mentes:\n");

        printf("\n\nKerlek valassz:\n");
        int choice=0;
        int result;
        result = scanf("%d%*c",&choice);
        
        
        
        if(result == EOF){
            printf("%s","Valami rosszul ment a beolvasasnal!");
            gameIsOn=0;
        }
        
        if(result!=0){
            
            switch (choice)
            {
                case 1:
                    {
                        
                        printf("\nKerlek add meg az uj jelentkezo teljes nevet es a napokat, mikor tudna dolgozni! pl: Almasi_Peter hetfo csutortok\n");

                        line = NULL;
                        len = 0;
                        

                        printf("Ide ird a jelentkezest: ");
                        read = getline(&line, &len, stdin);

                        char *token;
                        char name[50];

                        token = strtok(line, " ");
                        strcpy(name,token);
                        //name[strlen(name)]="\0";

                        printf("A nev a kovi: %s\n",name);

                        token = strtok(NULL, " ");
                        int dayIndex=0;
                        
                        while( token != NULL ) {
                            
                            if(strncmp(token,"hetfo",5)==0){
                                dayIndex=0;
                            }else if(strncmp(token,"kedd",4)==0){
                                dayIndex=1;
                            }else if(strncmp(token,"szerda",5)==0){
                                dayIndex=2;
                            }else if(strncmp(token,"csutortok",9)==0){
                                dayIndex=3;
                            }else if(strncmp(token,"pentek",6)==0){
                                dayIndex=4;
                            }else if(strncmp(token,"szombat",7)==0){
                                dayIndex=5;
                            }else if(strncmp(token,"vasarnap",8)==0){
                                dayIndex=6;
                            }else{
                                printf("Helytelen programhasznalat!");
                                return 1;
                                break;
                            }

                            int success = applyIsSuccessfull(name,dayIndex);

                            if(success==0){
                                printf("Sikeres folyamat.\n\n");
                            }else{
                                printf("\nSikertelen folyamat.\n\n");
                            }
                            token = strtok(NULL, " ");
                        }

                        free(line);

                        break;   
                    }
                    
                case 2:
                    {
                        printf("Modositas! Kerlek add meg a neved:");
                        char nameToDelete[50];
                        int size = 0;
                        int resultNameRead;

                        scanf("%s%*c",&nameToDelete);
                        int nameExists=0;
                        for(int i=0;i<7;i++){
                            if(strstr(weekWorkers[i],nameToDelete)!=NULL){
                                nameExists=1;
                            }
                        }
                        if(nameExists==1){
                            int stillDelete = 1;
                            char dayReadIn[200];
                            while(stillDelete){
                                printf("Kerlek olvasd be hogy melyik naprol szeretned kitorolni magadat!\n");
                                
                                scanf("%s%*c",dayReadIn);

                                int dayIndex=-1;
                                if(strncmp(dayReadIn,"hetfo",5)==0){
                                    dayIndex=0;
                                }else if(strncmp(dayReadIn,"kedd",4)==0){
                                    dayIndex=1;
                                }else if(strncmp(dayReadIn,"szerda",5)==0){
                                    dayIndex=2;
                                }else if(strncmp(dayReadIn,"csutortok",9)==0){
                                    dayIndex=3;
                                }else if(strncmp(dayReadIn,"pentek",6)==0){
                                    dayIndex=4;
                                }else if(strncmp(dayReadIn,"szombat",7)==0){
                                    dayIndex=5;
                                }else if(strncmp(dayReadIn,"vasarnap",8)==0){
                                    dayIndex=6;
                                }else{
                                    printf("Helytelen programhasznalat!\n");
                                    return 1;
                                    break;
                                }
                                delete(nameToDelete,dayIndex); 

                                printf("Szeretned meg kitorolni magad valamikorrol? (1 -> igen / 2-> nem)");
                                int toContinue;
                                scanf("%d",&toContinue);
                                if(toContinue==1){
                                    stillDelete=1;
                                }else{
                                    stillDelete=0;
                                }
                                
                            }
                            int stillJoin = 1;
                            printf("\nKerlek olvasd be hogy melyik naphoz szeretned hozzaaddni magadat!\n");
                            while(stillJoin){
                                
                                
                                scanf("%s%*c",dayReadIn);

                                int dayIndex=-1;
                                if(strncmp(dayReadIn,"hetfo",5)==0){
                                    dayIndex=0;
                                }else if(strncmp(dayReadIn,"kedd",4)==0){
                                    dayIndex=1;
                                }else if(strncmp(dayReadIn,"szerda",5)==0){
                                    dayIndex=2;
                                }else if(strncmp(dayReadIn,"csutortok",9)==0){
                                    dayIndex=3;
                                }else if(strncmp(dayReadIn,"pentek",6)==0){
                                    dayIndex=4;
                                }else if(strncmp(dayReadIn,"szombat",7)==0){
                                    dayIndex=5;
                                }else if(strncmp(dayReadIn,"vasarnap",8)==0){
                                    dayIndex=6;
                                }else{
                                    printf("Helytelen programhasznalat!\n");
                                    return 1;
                                    break;
                                }   
                                applyIsSuccessfull(nameToDelete,dayIndex);

                                printf("Szeretned meg hozzadni magad valamikorra? (1 -> igen / 2-> nem)");
                                int toContinue;
                                scanf("%d",&toContinue);
                                if(toContinue==1){
                                    stillJoin=1;
                                }else{
                                    stillJoin=0;
                                }
                                
                            }
                        }else{
                            printf("\nA nev nem szerepel az adatbazisban! Eloszor jelentkezned kell, ha ez meg nem tortent meg!");
                        }
                        

                        break; 
                    }
                case 3:
                    {
                        printf("Torles! Kerlek add meg a neved:");
                        char nameToDelete[50];
                        int size = 0;
                        int resultNameRead;
                        // getline(&nameToDelete,&size,stdin);
                        scanf("%s%*c",&nameToDelete);


                        //nameToDelete[strlen(nameToDelete) - 1] = '\0';

                        int stillDelete = 1;
                        char dayReadIn[200];
                        while(stillDelete){
                            printf("Kerlek olvasd be hogy melyik naprol szeretned kitorolni magadat!\n");
                            
                            scanf("%s%*c",dayReadIn);
                            int dayIndex=-1;
                            if(strncmp(dayReadIn,"hetfo",5)==0){
                                dayIndex=0;
                            }else if(strncmp(dayReadIn,"kedd",4)==0){
                                dayIndex=1;
                            }else if(strncmp(dayReadIn,"szerda",5)==0){
                                dayIndex=2;
                            }else if(strncmp(dayReadIn,"csutortok",9)==0){
                                dayIndex=3;
                            }else if(strncmp(dayReadIn,"pentek",6)==0){
                                dayIndex=4;
                            }else if(strncmp(dayReadIn,"szombat",7)==0){
                                dayIndex=5;
                            }else if(strncmp(dayReadIn,"vasarnap",8)==0){
                                dayIndex=6;
                            }else{
                                printf("Helytelen programhasznalat!\n");
                                return 1;
                                break;
                            }   
                            delete(nameToDelete,dayIndex); 

                            printf("Szeretned meg kitorolni magad valamikorrol? (1 -> igen / 2-> nem)");
                            int toContinue;
                            scanf("%d",&toContinue);
                            if(toContinue==1){
                                stillDelete=1;
                            }else{
                                stillDelete=0;
                            }
                            
                        }
                    }
                    

                    break;
                case 4:
                    printf("\nListazas:\n");
                    for(int i=0; i<7; i++) {
                        printf("%d. ",i+1);
                        for(int j=0; j<strlen(weekWorkers[i]); j++) {
                            printf("%c",weekWorkers[i][j]);
                        }
                        printf("\n");
                    }
                    printf("\n");
                    break;
                case 5:
                    fp = fopen("database.txt", "w");
                    printf("\nSikeresen kileptel a programbol!\n");
                    for(int i=0;i<7;i++){
                        if(i<6){
                            strcat(weekWorkers[i],"\n");    
                        }
                        int results = fputs(weekWorkers[i], fp);
                        
                        // printf("%s",weekWorkers[i]);
                        if (results == EOF) {
                            printf("The transcription to the file has failed!");
                        }
                    }
                    fclose(fp);
                    gameIsOn=0;
                    
                    break;
                default:
                    printf("Ez a parancs nem letezik, kerlek probald ujra!\n");
                    break;
            }
        }else{
            printf("Hiba! Nem szamot adtal meg!\n");
            break;
            fclose(fp);

        }
    }

    return 0;
}
//meg csinalhatsz egy masolatot a filerol es ennyi kb