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
            initscr();
            up(semaforo);
            // AREA PROTEGIDA PARA CRONOMETRO
            erase();
            int shmid;
            key_t key;
            char *shm, *s;
            key = 5679;
            //************+AREA PARA P1_PUEDE_ENTRAR
            int shmid1;
            key_t key1;
            char *shm1, *s1;
            key1 = 6678;
            //************+AREA PARA P2_PUEDE_ENTRAR
            int shmid2;
            key_t key2;
            char *shm2, *s2;
            key2 = 9876;
            //************+AREA PARA turno
            int shmid3;
            key_t key3;
            char *shm3, *s3;
            key3 = 8679;
            if ((shmid1 = shmget(key1, MAXSIZE, 0666)) < 0)
                die("shmget");
            if ((shm1 = shmat(shmid1, NULL, 0)) == (char *) -1)
                die("shmat");
            s1 =shm1;
            if ((shmid2 = shmget(key2, MAXSIZE, 0666)) < 0)
                die("shmget");
            if ((shm2 = shmat(shmid2, NULL, 0)) == (char *) -1)
                die("shmat");
            s2 =shm2;
            if ((shmid3 = shmget(key3, MAXSIZE, 0666)) < 0)
                die("shmget");
            if ((shm3 = shmat(shmid3, NULL, 0)) == (char *) -1)
                die("shmat");
            s3 =shm3;
            if ((shmid = shmget(key, MAXSIZE, 0666)) < 0)
                die("shmget");
            if ((shm = shmat(shmid, NULL, 0)) == (char *) -1)
                die("shmat");
            s =shm;
            int pos=0;
            while(true)
            {
                *s2=1;          //p2_puede_entrar=true;
                char val[1];
                val[0]=s1[0];
                int p1_puede_entrar=((char*)val)[0];
                while(p1_puede_entrar==1){      //while(p1_puede_entrar)
                    char val3[1];
                    val3[0]=s3[0];
                    int turno=((char*)val3)[0];
                    if(turno==1){
                        *s2=0;          //p2_puede_entrar=false;
                        while(turno==1){                            
                            val3[0]=s3[0];
                            turno=((char*)val3)[0];
                        }
                        *s2=1;          //p2_puede_entrar=true;
                    }
                    pos+=2;
                    val[0]=s1[0];
                    p1_puede_entrar=((char*)val)[0];
                    //sleep(1);
                }                
                char total[10]="";
                int cont=0;
                char val5[1];
                val5[0]=s[cont];
                while (val5[0] != 's'){
                    int result=((char*)val5)[0];
                    if(result!=58){
                        char integerString[3];
                        sprintf(integerString,"%d",result);
                        strcat(total,integerString);
                    }else{
                        strcat(total,val5);
                    }
                    cont++;
                    val5[0]=s[cont];
                }
                //inicio memoria compartida***********************
                mvprintw((max_y/2),(max_x-10)/2,"%s",total);
                refresh();
                *s3=1;
                *s2=0;
            }
            down(semaforo);
}

void ventana_defensor(int max_y,int max_x){
    erase();
    key_t Llave;
    int semaforo;
    //struct sembuf Proceso;
    int i = 0;
    Llave = ftok ("/bin/ls", 33);
    semaforo = semget (Llave, 1, 0600 | IPC_CREAT);
    semctl (semaforo, 0, SETVAL, 0);
    usleep(3);
            initscr();
            //semop (semaforo, &Proceso, 1);
            down(semaforo);
            // AREA PROTEGIDA PARA CRONOMETRO
            erase();
            int shmid;
            key_t key;
            char *shm, *s;
            key = 5679;
            //************+AREA PARA P1_PUEDE_ENTRAR
            int shmid1;
            key_t key1;
            char *shm1, *s1;
            key1 = 6678;
            //************+AREA PARA P2_PUEDE_ENTRAR
            int shmid2;
            key_t key2;
            char *shm2, *s2;
            key2 = 9876;
            //************+AREA PARA turno
            int shmid3;
            key_t key3;
            char *shm3, *s3;
            key3 = 8679;
            //inicializando y creando variables DEKKER
            if ((shmid1 = shmget(key1, MAXSIZE, 0666)) < 0)
                die("shmget");
            if ((shm1 = shmat(shmid1, NULL, 0)) == (char *) -1)
                die("shmat");
            s1 =shm1;
            *s1=0;
            if ((shmid2 = shmget(key2, MAXSIZE, 0666)) < 0)
                die("shmget");
            if ((shm2 = shmat(shmid2, NULL, 0)) == (char *) -1)
                die("shmat");
            s2 =shm2;
            *s2=0;
            if ((shmid3 = shmget(key3, MAXSIZE, 0666)) < 0)
                die("shmget");
            if ((shm3 = shmat(shmid3, NULL, 0)) == (char *) -1)
                die("shmat");
            s3 =shm3;
            *s3=1;
            if ((shmid = shmget(key, MAXSIZE, 0666)) < 0)
                die("shmget");
            if ((shm = shmat(shmid, NULL, 0)) == (char *) -1)
                die("shmat");
            s =shm;
            *s=0;
            //FIN de la creacion e inicializacion DEKKER
            int pos=0;
            while(true)
            {
                *s1=1;//p1QE=TRUE;
                char val[1];
                val[0]=s2[0];
                int p2_puede_entrar=((char*)val)[0];
                while(p2_puede_entrar==1){      //while(p2_puede_entrar)
                    s3 =shm3;
                    char val3[1];
                    val3[0]=s3[0];
                    int turno=((char*)val3)[0];
                    if(turno==2){
                        *s1=0;          //p1_puede_entrar=false;
                        while(turno==2){                            
                            val3[0]=s3[0];
                            turno=((char*)val3)[0];
                        }
                        *s1=1;          //p1_puede_entrar=true;
                    }
                    pos+=2;
                    val[0]=s2[0];
                    p2_puede_entrar=((char*)val)[0];
                    sleep(1);
                }                
                millisec++;
                if(millisec == 100)
                {
                    millisec = 0;
                    seconds++;
                    if(seconds == 60)
                    {
                        seconds = 0;
                    }
                }
                //mvprintw((max_y/2)+pos,(max_x-10)/2,"entrando a la region critica en el proceso1");
                //refresh();
                //inicio memoria compartida***********************
                if ((shmid = shmget(key, MAXSIZE, IPC_CREAT | 0666)) < 0)
                    die("shmget");
                if ((shm = shmat(shmid, NULL, 0)) == (char *) -1)
                    die("shmat");
                s = shm;
                *s=seconds;
                s++;
                *s = ':';
                s++;
                *s = millisec;
                s++;
                *s = 's';
                mvprintw((max_y/2)-11,(max_x-10)/2+34,"  %d :  %d ", seconds, millisec);
                refresh();
                *s3=2;
                *s1=0;
                sleep(1);
            }
            up(semaforo);
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