#include <stdio.h>
#include<ncurses.h>
#include<unistd.h>
#include<sys/sem.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<stdlib.h>

//#if defined(__GNU_LIBRARY__) && !deined()
#define DELAY 30000
//#if defined(__GNU_LIBRARY__) && !deined(__)
//#else
union semun{
    int val;
    struct semid_ds *buf;
    unsigned short int * array;
    struct seminfo *__buf;
};
//#endif
/*int main(void){
    key_t Llave;
    int semaforo;
    struct sembuf Proceso;
    int i = 0 ;
    Llave = ftok ("/bin/ls", 313);
    semaforo = semget (Llave, 10, 0600 | IPC_CREAT);
    semctl (semaforo, 0, SETVAL, 0);
    Proceso.sem_num = 0;
    Proceso.sem_op = -1;
    Proceso.sem_flg = 0;
    while (1){
        if(i==11){
            printf("PROGRAMA TERMINADO \n");
            break;
        }else{
            printf("%d Esperando semaforo en verde.... \n",i);
            semop (semaforo, &Proceso, 1);
            // AREA PROTEGIDA
            printf("%d Saliendo del area critica - Semaforo en rojo \n",i);
            i++;
        }
    }
}*/
/*bool p1_puede_entrar=false;
bool p2_puede_entrar=false;
int turno=1;

void proceso1(){
    while(true){
        p1_puede_entrar=true;
        printf("hace algo 1");
        while(p2_puede_entrar){
            if(turno==2){
                p1_puede_entrar=false;
                while(turno==2){
                }
                p1_puede_entrar=true;
            }
        }
        //region critica
        printf("region critica1");
        turno=2;
        p1_puede_entrar=false;
        printf("hace algo mas 1");
    }
}

void proceso2(){
    while(true){
        printf("hace cosas2");
        p2_puede_entrar=true;
        while(p1_puede_entrar){
            if(turno==1){
                p2_puede_entrar=false;
                while(turno==1){
                }
                p2_puede_entrar=true;
            }
        }
        //region critica
        printf("region critica2");
        turno=1;
        p2_puede_entrar=false;
        printf("hace mas cosas2");
    }
}

int main(void){
    p1_puede_entrar=false;
    p2_puede_entrar=false,
    turno=1;
    proceso1();
    proceso2();
}
*/
void down(int semid){
    struct sembuf Proceso;
    int valor=0;
    valor=semctl(semid,0,GETVAL);
    if(valor>=0){
        Proceso.sem_num = 0;
        Proceso.sem_op = -1;
        Proceso.sem_flg = 0;
        semop(semid,&Proceso,1);
    }else{
        Proceso.sem_num = 0;
        Proceso.sem_op = 1;
        Proceso.sem_flg = 0;
        semop(semid,&Proceso,1);
    }
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
    key_t Llave;
    int semaforo;
    //struct sembuf Proceso;
    int i = 0 ;
    Llave = ftok ("/bin/ls", 33);
    semaforo = semget (Llave,1, 0600 | IPC_CREAT);
    //semctl (semaforo, 0, SETVAL, 0);

    //Proceso.sem_num = 0;
    //Proceso.sem_op = 1;
    //Proceso.sem_flg = 0;
    /*for(i = 0 ; i < 11; i++) {
        printf("%d Semaforo en verde \n",i);
        //semop(semaforo,&Proceso, 1);
        down(semaforo);
        sleep(1);
    }*/
    while (true){
        if(i==11){
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
            // AREA PROTEGIDA
            printf("%d Saliendo del area critica invasor - Semaforo en rojo \n",i);
            sleep(10);
            up(semaforo);
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
    int i = 0 ;
    Llave = ftok ("/bin/ls", 33);
    semaforo = semget (Llave, 1, 0600 | IPC_CREAT);
    //semctl (semaforo, 0, SETVAL, 0);
    //Proceso.sem_num = 0;
    //Proceso.sem_op = -1;
    //Proceso.sem_flg = 0;
    usleep(3);
    while (true){
        if(i==11){
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
            // AREA PROTEGIDA
            printf("%d Saliendo del area critica defensor - Semaforo en rojo \n",i);
            sleep(10);
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
    /*int x =0,y=0;
    int max_x=0,max_y=0;
    int next_x=0;
    int direction=1;
    char msg[]="Hola";
    initscr();
    noecho();
    curs_set(FALSE);
    while (1) {
        getmaxyx(stdscr,max_y,max_x);
        clear();
        mvprintw(y,x,"-o-");
        mvprintw(max_y-1,x,"-o-");
        //mvprintw(max_y/2,(max_x-strlen(msg))/2,"%s",msg);
        refresh();
        usleep(DELAY);
        next_x = x + direction;
        if(next_x >= max_x || next_x<0){
            direction*=-1;
        }else{
            x+=direction;
        }
    }
    endwin();*/
    //return 0;
}
