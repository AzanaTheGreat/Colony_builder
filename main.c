typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;
void Exit(char Exit_Type);
#include <pthread.h>
#include <time.h>
void ThreadType();
pthread_mutex_t lock;
uint8 ThreadUsage; //How many threads the game is will use
#include "sys/sysinfo.h"
uint8 delay = 0;
uint32 TE = 0;
#include "IO/Events.c"

void Cycles();
uint8 run = 1;
uint8 ExitType = 0;

long NS_PER_SECOND = 1000000000;
double TimeElapsed = 0;
uint32 TICKS = 0;

void sub_timespec(struct timespec t1, struct timespec t2, struct timespec *td)
{
    td->tv_nsec = t2.tv_nsec - t1.tv_nsec;
    td->tv_sec  = t2.tv_sec - t1.tv_sec;
    if (td->tv_sec > 0 && td->tv_nsec < 0)
    {
        td->tv_nsec += NS_PER_SECOND;
        td->tv_sec--;
    }
    else if (td->tv_sec < 0 && td->tv_nsec > 0)
    {
        td->tv_nsec -= NS_PER_SECOND;
        td->tv_sec++;
    }
}

int main()
{
    struct timespec start, finish, delta;
    pthread_mutex_init(&lock, NULL);
    InitiateSDL();
    Render();
    ThreadType();
    MAINMENUPAGE(1);
    usleep(2000);
    InitiateOres();
    //InitMatPresets(); 
    
    
    while (run)
    {
        TICKS++;
        clock_gettime(CLOCK_MONOTONIC, &start);
        
        Cycles();
        if(DoRender) {
            Render();
            DoRender = 0;
        }
        usleep(100);
        Events();

        clock_gettime(CLOCK_MONOTONIC, &finish);
        sub_timespec(start, finish, &delta);
        TimeElapsed += delta.tv_sec + delta.tv_nsec*0.000000001;
        if(TimeElapsed >= 1) {
            TimeElapsed = 0;
            TE = TICKS;
            TICKS = 0;
        }
    }
    Freeeeeetile();
    pthread_mutex_destroy(&lock);
    return ExitType;
}

void Exit(char Exit_Type) {
    run = 0;
    ExitType = Exit_Type;
}

void Cycles() {
    delay++;
    /**/
    
    if(Ingame && AO) {
        switch (delay)
        {
        case 1:
            ShiftCam();
            break;

        case 8:
            DoRender = 1;
            break;
        
        case 9:
            delay = 0;
            break;
        };
    }
    else {
        switch (delay)
        {
        case 7:
            DoRender = 1;
            break;
        
        case 8:
            delay = 0;
            break;
        }
    }
    
}

//DEBUG

void ThreadType() {
    uint16 CPUCores;
    CPUCores = get_nprocs_conf();
    //printf("CPU Cores: %d\n", CPUCores);
    switch (CPUCores)
    {
    case 0:
        //printf("What?\n");
        ThreadUsage = 0;
        break;
    
    case 1:
    case 2:
    case 3:
        ThreadUsage = 1;
        break;

    case 4:
    case 5:
        ThreadUsage = 2;
        break;

    case 6:
    case 7:
        ThreadUsage = 3;
        break;

    case 8:
    case 9:
        ThreadUsage = 4;
        break;

    case 10:
    case 11:
        ThreadUsage = 5;
        break;

    case 12:
    case 13:
        ThreadUsage = 6;
        break;
    }
}