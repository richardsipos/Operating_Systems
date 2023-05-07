#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <signal.h>
#include <sys/types.h>
#include <errno.h> 


//a nevet meg modositani is lehet.
//#define MAX 500;

int week[]={10,10,10,10,10,10,10};
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

void handler(int signumber){
  //printf("Signal with number %i has arrived\n",signumber);
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
    ssize_t readFile;
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
        printf("\nMenu:\n");
        printf("1. Jelentkezo hozzaadasa:\n");
        printf("2. Jelentkezo adatainak a modositasa:\n");
        printf("3. Jelentkezo torlese:\n");
        printf("4. Teljes lista betoltese:\n");
        printf("5. Napi utaztatas inditasa!\n");
        printf("6. Kilepes es mentes:\n");

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
                        readFile = getline(&line, &len, stdin);

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
                            //Nev valtoztatas.
                            char answerNameChange[200];
                            printf("A neved szeretned megvaltoztatni?");
                            scanf("%s%*c",answerNameChange);

                            if(strncmp(answerNameChange,"igen",4)==0){
                                printf("Mi legyen az uj neved?");
                                
                                char newName[200];
                                scanf("%s%*c",newName);
                                
                                for(int i=0;i<7;i++){
                                    if(strstr(weekWorkers[i],nameToDelete)!=NULL){
                                        delete(nameToDelete,i);
                                        applyIsSuccessfull(newName,i);
                                        
                                    }
                                }
                                strcpy(nameToDelete,newName);
                            }else{
                                printf("Nevvaltoztatas nem tortent.\n");
                            }

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
                    printf("Kerlek olvasd be hogy melyik nap utaztatasat szeretned megoldani!\n");
                    char dayReadIn[200];
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
                    int workersToTakeOff = 0;
                    char weekWorkersCopy[300];
                    strcpy(weekWorkersCopy,weekWorkers[dayIndex]);
                    char * token = strtok(weekWorkersCopy, " ");
                    while(token != NULL){
                        workersToTakeOff=workersToTakeOff+1;
                        // printf("%s\n",token);
                        token = strtok(NULL, " ");
                    }

                    printf("\n\nElszalitando munkasok szama: %d\n",workersToTakeOff);
                    fflush(stdout);

                    char workersOnDay[200] = "";
                    strcpy(workersOnDay,weekWorkers[dayIndex]);

                    if(workersToTakeOff==0){
                        printf("A mai nap nem indul busz,mivel nincs munkas!\n");
                    }else if(workersToTakeOff <=5){
                        //pipe
                        int pipefd[2];
                        

                        if (pipe(pipefd) == -1) 
                        {
                            perror("Hiba a pipe nyitaskor!");
                            exit(EXIT_FAILURE);
                        }

                        signal(SIGTERM,handler);
                        pid_t  child1=fork();
                        if (child1<0){perror("The fork calling was not succesful\n"); exit(1);} 

                        if(child1>0){
                            //parent process
                            

                            char sz[100]="";
                                //parent process
                            pause(); //var egy signalra
                           // printf("Signal arrived to the parent!\n",SIGTERM);

                            write(pipefd[1], workersOnDay,strlen(workersOnDay));
                            close(pipefd[1]);
                            fflush(NULL);
                            //printf("\nSzulo atkuldte az adatokat csovon keresztul!\n");	

                            wait(NULL);
                            
                            //printf("\nSzulo visszakapja az adatokat a csovon keresztul!\n");

                            read(pipefd[0],sz,(strlen(workersOnDay) *sizeof(char)));
                            //printf("Az uzenet: %s",sz);
                            close(pipefd[0]);

                            //delete from
                            char toDeleteFromAllWorkers[200] = "";
                            strcpy(toDeleteFromAllWorkers,sz);
                            char * token = strtok(sz, " ");
                            int workersBroughtIn = 0;
                            printf("\nAz elso busz a kovetkezo szemlyeket szalitotta be a munkaba:\n");
                            while(token != NULL){
                                workersBroughtIn=workersBroughtIn+1;
                                printf("%s\n",token);
                                token = strtok(NULL, " ");
                            }

                            printf("Ez osszesen %d szemelyt jelent!",workersBroughtIn);

                            // strcpy(workersOnDay,workersOnDay+strlen(toDeleteFromAllWorkers));
                            if(strcmp(workersOnDay,toDeleteFromAllWorkers)==0){
                                strcpy(workersOnDay,"");
                            }else{
                                size_t len = strlen(toDeleteFromAllWorkers)-1;
                                char *p;
                                p=strstr( workersOnDay,toDeleteFromAllWorkers);
                                if(p){
                                    strcpy( p,p+len+1);
                                }
                            }

                            
                            // printf("%s",workersOnDay);
                        }else{
                            //child process
                            char sz[100]="";
                            printf("Elindult az 1. busz!\n");
                            kill(getppid(),SIGTERM);
                            
                            read(pipefd[0],sz,(strlen(workersOnDay) *sizeof(char)));

                            printf("A busz a kovetkezo szemelyeket kell munkaba bevigye: %s\n" , sz);

                            char workersManagedToBring[100] ="";
                            int workersToBring=0;
                            char * token = strtok(sz, " ");
                            while(token != NULL){
                                workersToBring=workersToBring+1;
                                // printf("%s\n",token);
                                strcat(workersManagedToBring,token);
                                strcat(workersManagedToBring," ");
                                token = strtok(NULL, " ");
                                if(workersToBring==5){
                                    break;
                                    //a busz nem kepes tobbet vinni
                                }
                            }
                            // printf("\nAz uzenetet ami visszalesz kuldve a szulobe: %s \n",workersManagedToBring);
                            close(pipefd[0]);
                            write(pipefd[1], workersManagedToBring ,strlen(workersManagedToBring));
                            close(pipefd[1]);
                            fflush(NULL);


                            return 0;
                            
                        }

                    }else if(workersToTakeOff >5 && workersToTakeOff<=10){
                        //pipe
                        printf("Ket buszra van szukseg!\n");
                        int pipefd[2];

                        if (pipe(pipefd) == -1) 
                        {
                            perror("Hiba a pipe nyitaskor!");
                            exit(EXIT_FAILURE);
                        }

                        signal(SIGTERM,handler);
                        pid_t  child1=fork();

                        if (child1<0){perror("The fork calling was not succesful\n"); exit(1);} 

                        if(child1>0){
                            //parent process

                            char sz[100]="";
                                
                            pause(); //var egy signalra
                            //printf("Signal arrived to the parent!\n",SIGTERM);

                            write(pipefd[1], workersOnDay,strlen(workersOnDay));
                            // close(pipefd[1]);
                            fflush(NULL);
                            //printf("\nSzulo atkuldte az adatokat csovon keresztul!\n");	

                            wait(NULL);
                            
                            //printf("\nSzulo visszakapja az adatokat a csovon keresztul!\n");

                            read(pipefd[0],sz,(strlen(workersOnDay) *sizeof(char)));
                            //printf("Az uzenet: %s",sz);
                            // sclose(pipefd[0]);

                            //delete from
                            char toDeleteFromAllWorkers[200] = "";
                            strcpy(toDeleteFromAllWorkers,sz);
                            char * token = strtok(sz, " ");
                            int workersBroughtIn = 0;
                            printf("\nAz 1. busz a kovetkezo szemlyeket szalitotta be a munkaba:\n");
                            while(token != NULL){
                                workersBroughtIn=workersBroughtIn+1;
                                printf("%s\n",token);
                                token = strtok(NULL, " ");
                            }

                            printf("Ez osszesen %d szemelyt jelent!",workersBroughtIn);

                            // strcpy(workersOnDay,workersOnDay+strlen(toDeleteFromAllWorkers));
                            if(strcmp(workersOnDay,toDeleteFromAllWorkers)==0){
                                strcpy(workersOnDay,"");
                            }else{
                                size_t len = strlen(toDeleteFromAllWorkers)-1;
                                char *p;
                                p=strstr( workersOnDay,toDeleteFromAllWorkers);
                                if(p){
                                    strcpy( p,p+len+1);
                                }
                            }     

                            close(pipefd[0]);
                            close(pipefd[1]);


                            
                            

                            int pipefd2[2];

                                if (pipe(pipefd2) == -1) 
                                {
                                    perror("Hiba a pipe nyitaskor!");
                                    exit(EXIT_FAILURE);
                                }

                            pid_t  child2=fork();
                            if (child2<0){perror("The fork calling was not succesful\n"); exit(1);}

                            if(child2>0){
                                //parent process
                                char sz2[100]="";
                                pause();
                                
                                write(pipefd2[1], workersOnDay,strlen(workersOnDay));

                                wait(NULL);

                                read(pipefd2[0],sz2,(strlen(workersOnDay) *sizeof(char)));

                                char * token = strtok(sz2, " ");
                                int workersBroughtIn = 0;
                                printf("\nA 2. busz a kovetkezo szemlyeket szalitotta be a munkaba:\n");
                                while(token != NULL){
                                    workersBroughtIn=workersBroughtIn+1;
                                    printf("%s\n",token);
                                    token = strtok(NULL, " ");
                                }

                                printf("Ez osszesen %d szemelyt jelent!",workersBroughtIn);

                                close(pipefd2[0]);
                                close(pipefd2[1]);



                            }else{
                                //child process
                                char sz2[100]="";
                                printf("\n\nElindult az 2. busz!\n");
                                // sleep(1);
                                kill(getppid(),SIGTERM);
                                
                                read(pipefd2[0],sz2,(strlen(workersOnDay) *sizeof(char)));

                                printf("A busz a kovetkezo szemelyeket kell munkaba bevigye: %s\n" , sz2);
                                char workersManagedToBring[100] ="";
                                int workersToBring=0;
                                char * token = strtok(sz2, " ");
                                while(token != NULL){
                                    workersToBring=workersToBring+1;
                                    strcat(workersManagedToBring,token);
                                    strcat(workersManagedToBring," ");
                                    token = strtok(NULL, " ");
                                    if(workersToBring==5){
                                        break;
                                    }
                                }


                                write(pipefd2[1], workersManagedToBring ,strlen(workersManagedToBring));

                                return 0;

                            }

                        }else{

                            //child process
                            char sz[100]="";
                            printf("Elindult az 1. busz!\n");
                            kill(getppid(),SIGTERM);
                            
                            read(pipefd[0],sz,(strlen(workersOnDay) *sizeof(char)));

                            printf("A busz a kovetkezo szemelyeket kell munkaba bevigye: %s\n" , sz);

                            char workersManagedToBring[100] ="";
                            int workersToBring=0;
                            char * token = strtok(sz, " ");
                            while(token != NULL){
                                workersToBring=workersToBring+1;
                                // printf("%s\n",token);
                                strcat(workersManagedToBring,token);
                                strcat(workersManagedToBring," ");
                                token = strtok(NULL, " ");
                                if(workersToBring==5){
                                    break;
                                    //a busz nem kepes tobbet vinni
                                }
                            }
                            write(pipefd[1], workersManagedToBring ,strlen(workersManagedToBring));
                            fflush(NULL);


                            return 0;
                        }
                    }
            
                    
                    

                    printf("\n");


                    break;

                case 6:
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