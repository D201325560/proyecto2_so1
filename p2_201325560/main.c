#include <stdio.h>
#include<ncurses.h>
#include<unistd.h>
#include<string.h>
#include<sys/sem.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<stdlib.h>
#include <time.h>
#include <sys/shm.h>
#define MAXSIZE     3

int minutes=0, seconds=0, millisec=0;
//#if defined(__GNU_LIBRARY__) && !deined()
bool p1_puede_entrar,p2_puede_entrar;
#define DELAY 30000
//#if defined(__GNU_LIBRARY__) && !deined(__)
//#else
union semun{
    int val;
    struct semid_ds *buf;
    unsigned short int * array;
    struct seminfo *__buf;
};
void die(char *s)
{
    perror(s);
    exit(1);
}
//#endif
void down(int semid){
    struct sembuf Proceso;
    int valor=10;
    valor=semctl(semid,0,GETVAL);
        Proceso.sem_num = 0;
        Proceso.sem_op = -1;
        Proceso.sem_flg = 0;
        semop(semid,&Proceso,1);
}
void up(int semid){
    struct sembuf Proceso;
    Proceso.sem_num = 0;
    Proceso.sem_op = 1;
    Proceso.sem_flg = 0;
    semop(semid,&Proceso,1);
}
void ventana_invasor(int max_y,int max_x){
    char msg[]="seleccionaste: 2";
    char msg1[]="invasor";
    char msg2[]="espera mientras se conecta el defensor";
    erase();
    usleep(3);
    //*******************semaforo para la pantalla
    key_t Llave;
    int semaforo;
    int i=0;
    Llave = ftok ("/bin/ls", 33);
    semaforo = semget (Llave,1, 0600 | IPC_CREAT);
    semctl (semaforo, 0, SETVAL, 0);
    //*******************fin semaforo para pantalla
    while (true){
        if(i==101){
            printf("PROGRAMA TERMINADO \n");
            sleep(10);
            break;
        }else{
            initscr();
            mvprintw((max_y/2)-2,(max_x-strlen(msg))/2,"%s",msg);
            refresh();
            mvprintw((max_y/2),(max_x-strlen(msg1))/2,"%s",msg1);
            refresh();
            mvprintw((max_y/2)+2,(max_x-strlen(msg2))/2,"%s",msg2);
            refresh();
            up(semaforo);
            // AREA PROTEGIDA PARA CRONOMETRO
            erase();
            int shmid;
            key_t key;
            char *shm, *s;
            key = 5679;
            //AREA PROTEGIDA PARA CRONOMETRO
            int shmid1;
            key_t key1;
            char *shm1, *s1;
            key1 = 6679;
            int x=0,y=0;
            //AREA PARA NAVES
            while(true){
                cont:
                sleep(1);
                millisec++;
                if(millisec == 100)
                {
                    millisec = 0;
                    seconds++;
                    if(seconds == 60)
                    {
                        seconds = 0;
                        minutes++;
                    }
                }
                //inicia memoria compartida cliente*********************************
                if ((shmid = shmget(key, MAXSIZE, 0666)) < 0)
                    die("shmget");
                if ((shm = shmat(shmid, NULL, 0)) == (char *) -1)
                    die("shmat");
                s =shm;
                char total[10]="";
                int cont=0;
                char val[1];
                val[0]=s[cont];
                while (val[0] != 's'){
                    int result=((char*)val)[0];
                    if(result!=58){
                        char integerString[3];
                        sprintf(integerString,"%d",result);
                        strcat(total,integerString);
                    }else{
                        strcat(total,val);
                    }
                    cont++;
                    val[0]=s[cont];
                }
                clear();
                mvprintw((max_y/2)-11,(max_x-strlen(msg))/2+34," %s ",total);
                refresh();
                if ((shmid1 = shmget(key1, MAXSIZE, IPC_CREAT | 0666)) < 0)
                    die("shmget");
                if ((shm1 = shmat(shmid1, NULL, 0)) == (char *) -1)
                    die("shmat");
                s1 = shm1;
                char total1[40]="";
                int cont1=0;
                char val1[1];
                val1[0]=
                s1[cont1];
                int inicio_fila=0;
                while (val1[0] != 's'){
                    inicio_fila++;
                    int result1=((char*)val1)[0];
                    if(inicio_fila==1){//aqui viene el valor de y
                        y=result1;
                    }else if(inicio_fila==2){//viene el valor de x
                        x=result1;
                    }else if(result1==7){//nave normal
                        strcat(total1,"\\-.-/");
                        strcat(total1,"  ");
                    }else if(result1==8){//nave especial
                        strcat(total1,"(/--\\)");
                        strcat(total1,"  ");
                    }else if(result1==9){
                        inicio_fila=0;
                        mvprintw(y,x," %s ",total1);
                        //refresh();
                        strcpy(total1,"");
                    }
                    cont1++;
                    val1[0]=s1[cont1];
                }
                mvprintw((max_y/2)-11,(max_x-strlen(msg))/2+34," %s ",total);
                refresh();
                /*mvprintw(y,x," %s ",total1);
                refresh();*/
            }
            //sleep(10);
            down(semaforo);
            i++;
        }
    }
    sleep(10);
    //endwin();
}

void ventana_defensor(int max_y,int max_x){
    erase();
    char msg[]="seleccionaste: 1";
    char msg1[]="defensor";
    char msg2[]="espera mientras se conecta el invasor";
    key_t Llave;
    int semaforo;
    //struct sembuf Proceso;
    int i = 0;
    Llave = ftok ("/bin/ls", 33);
    semaforo = semget (Llave, 1, 0600 | IPC_CREAT);
    semctl (semaforo, 0, SETVAL, 0);
    usleep(3);
    while (true){
        if(i==10){
            printf("PROGRAMA TERMINADO \n");
            sleep(10);
            break;
        }else{
            //printf("%d Esperando semaforo en verde.... \n",i);
            initscr();
            mvprintw((max_y/2)-2,(max_x-strlen(msg))/2,"%s",msg);
            refresh();
            mvprintw((max_y/2),(max_x-strlen(msg1))/2,"%s",msg1);
            refresh();
            mvprintw((max_y/2)+2,(max_x-strlen(msg2))/2,"%s",msg2);
            refresh();
            //semop (semaforo, &Proceso, 1);
            down(semaforo);
            // AREA PROTEGIDA PARA CRONOMETRO
            erase();
            int shmid;
            key_t key;
            char *shm, *s;
            key = 5679;
            //AREA PROTEGIDA PARA CRONOMETRO
            int shmid1;
            key_t key1;
            char *shm1, *s1;
            key1 = 6679;
            //AREA PARA NAVES
            int x=0,y=10;
            int max_x1=0,max_y1=0;
            int next_x=0;
            int direction=6;
            int total_fila=40;
            while(true)
            {
                cont:
                sleep(9);
                millisec++;
                if(millisec == 100)
                {
                    millisec = 0;
                    seconds++;
                    if(seconds == 60)
                    {
                        seconds = 0;
                        minutes++;
                    }
                }
                //inicio memoria compartida***********************
                if ((shmid = shmget(key, MAXSIZE, IPC_CREAT | 0666)) < 0)
                    die("shmget");
                if ((shm = shmat(shmid, NULL, 0)) == (char *) -1)
                    die("shmat");
                s = shm;
                *s = minutes;
                s++;
                *s = ':';
                s++;
                *s=seconds;
                s++;
                *s = ':';
                s++;
                *s = millisec;
                s++;
                *s = 's';
                clear();
                mvprintw((max_y/2)-11,(max_x-strlen(msg))/2+34,"  %d :  %d :  %d ", minutes, seconds, millisec);
                refresh();
                getmaxyx(stdscr,max_y1,max_x1);
                if ((shmid1 = shmget(key1, MAXSIZE, IPC_CREAT | 0666)) < 0)
                    die("shmget");
                if ((shm1 = shmat(shmid1, NULL, 0)) == (char *) -1)
                    die("shmat");
                s1 = shm1;
                mvprintw(y,x,"\\-.-/  \\-.-/  (/--\\)  \\-.-/  \\-.-/");
                mvprintw(y+2,x+6,"\\-.-/  \\-.-/  (/--\\)  \\-.-/  \\-.-/");
                mvprintw(y+4,x,"\\-.-/  \\-.-/  (/--\\)  \\-.-/  \\-.-/");
                mvprintw(y+6,x+6,"\\-.-/  \\-.-/  (/--\\)  \\-.-/  \\-.-/");
                refresh();
                int k,l;
                int aux_x,aux_y;
                aux_x=x;
                aux_y=y;
                for(l=0;l<4;l++){
                    *s1=y;
                    s1++;
                    *s1=x;
                    s1++;
                    for(k=0;k<5;k++){
                        if(k==2){
                            *s1=8;//para nave especial
                            s1++;
                        }else{
                            *s1=7;//para nave especial
                            s1++;
                        }
                    }
                    *s1=9;//indica q finalizo la linea
                    s1++;
                    if(l==0||l==2){
                     x=aux_x+6;
                     //   x=0;
                    }else{
                        x=0;
                    }
                    y=y+2;
                }
                x=aux_x;
                y=aux_y;
                *s1='s';
                next_x = x + direction;
                if(next_x+total_fila >= max_x1 || next_x<0){
                    direction*=-1;
                }else{
                    x+=direction;
                }
            }
            up(semaforo);
            i++;
        }
    }
    //endwin();
}

void ventana2(int max_y,int max_x){
    char msg[]="selecciona tu lado";
    char msg1[]="jugador1 - defensor";
    char msg2[]="jugador2 - invasor";
    char msg3[]="debes esperar mientras se conecta tu oponente";
    initscr();
    //getmaxyx(stdscr,max_y,max_x);
    mvprintw((max_y/2)-5,(max_x-strlen(msg))/2,"%s",msg);
    refresh();
    start_color();
    init_pair(1,COLOR_WHITE,COLOR_BLUE);
    attron(COLOR_PAIR(1));
    mvprintw((max_y/2)-2,(max_x-strlen(msg1))/2,"%s",msg1);
    refresh();
    attroff(COLOR_PAIR(1));

    start_color();
    init_pair(2,COLOR_WHITE,COLOR_RED);
    attron(COLOR_PAIR(2));
    mvprintw((max_y/2)+2,(max_x-strlen(msg2))/2,"%s",msg2);
    refresh();
    attroff(COLOR_PAIR(2));
    mvprintw((max_y/2)+5,(max_x-strlen(msg3))/2,"%s",msg3);
    refresh();
    while(true){
        sleep(3);
        keypad(stdscr,TRUE);
        int ch=0;
        ch=getch();
        switch (ch) {
            case 49:
                erase();
                ventana_defensor(max_y,max_x);
                break;
            case 50:
                erase();
                ventana_invasor(max_y,max_x);
                break;
        }
    }
}
int main(void)
{
    int max_x=0,max_y=0;
    char msg[]="SPACE INVADERS";
    char msg1[]="Bienvenido";
    char msg2[]="Presione Enter Para Continuar";
    char msg3[]="David Daniel Alvarez Hernandez - 201325560";
    initscr();
    getmaxyx(stdscr,max_y,max_x);
    start_color();
    init_pair(1,COLOR_WHITE,COLOR_BLUE);
    attron(COLOR_PAIR(1));
    mvprintw((max_y/2)-5,(max_x-strlen(msg))/2,"%s",msg);
    refresh();
    attroff(COLOR_PAIR(1));
    mvprintw((max_y/2),(max_x-strlen(msg1))/2,"%s",msg1);
    refresh();
    start_color();
    init_pair(1,COLOR_WHITE,COLOR_RED);
    attron(COLOR_PAIR(1));
    mvprintw((max_y/2)+2,(max_x-strlen(msg2))/2,"%s",msg2);
    refresh();
    attroff(COLOR_PAIR(1));
    mvprintw((max_y/2)+5,(max_x-strlen(msg3))/2,"%s",msg3);
    refresh();
    sleep(3);
    keypad(stdscr,TRUE);
    while(10!=getch()){
    }
    erase();
    ventana2(max_y,max_x);
    return 0;    
}
