#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <rand.h>

#define DEBUG 0
/*union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf
};*/

int main(int argnum, char **args) {
    //random num gen for key.
    //sranddev();
    //int randnum = rand();
    int key;
    int shmid;
    int semid;
//Creation
    if (strcmp(args[1], "-c")) {
        //creation of file
        umask(0000);
        f = open("./story.txt", O_CREAT, 0644);
        umask(0022);
        //creation of key
        key = ftok("./story.txt", 12);
        if (DEBUG) printf("Key created. Key: %d", key);
        //creation of shared memory for character num
        shmid = shmget(key, 8, IPC_CREAT | 0664);
        if (DEBUG) printf("Shared memory created. Shmid: %d", shmid);
        //creation of the semaphore
        semid = semget(key, 1, IPC_CREAT | 0664);
        if (DEBUG) printf("Semaphore created. Semid: %d", semid);
    }
    //creation of shared memory

//Removing
    if (strcmp(args[1], "-r")) {
        
    }
    return 0;
}
