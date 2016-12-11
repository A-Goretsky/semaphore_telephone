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

#define DEBUG 1
/*union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf
};*/

void showstory() {
    int file;
    file = open("story.txt", O_RDWR);
    char storage[1000000];
    read(file, storage, sizeof(storage));
    printf("--------------\nPrinting Story\n--------------\n");
    printf("%s", storage);
    close(file);
}

int main(int argnum, char **args) {

    if (argnum != 2) {
        printf("Incorrect arguments. Please type program name followed by one flag. -c, -v, or -r");
        return 1;
    }
    //random num gen for key.
    //sranddev();
    //int randnum = rand();

    //things needed
    int f;
    int memkey;
    int semkey;
    int semid;
    int shmid;
    int semscval;
    int shmcval;

    //creation of keys
    memkey = ftok("control.c", 12);
    if (DEBUG) printf("Mem key created. Key: %d\n", memkey);
    semkey = ftok("makefile", 23);
    if (DEBUG) printf("Sem key created. Key: %d\n", semkey);

//Creation
    if (strncmp(args[1], "-c", strlen(args[1])) == 0) {
        //creation of file
        umask(0000);
        f = open("story.txt", O_CREAT | O_TRUNC, 0644);
        umask(0022);

        //creation of shared memory for character num
        shmid = shmget(memkey, 8, IPC_CREAT | IPC_EXCL | 0664);
        if (DEBUG) printf("Shared memory created. Shmid: %d\n", shmid);

        //creation of semaphore
        semid = semget(semkey, 1, IPC_CREAT | IPC_EXCL | 0664);
        if (semid == -1) {
            printf("error: %s\n", strerror(errno));
            return 1;
        }
        if (DEBUG) printf("Semaphore created. Semid: %d\n", semid);

        //setting semaphore value
        union semun su;
        su.val = 1;
        semscval = semctl(semid, 0, SETVAL, su);
        if (DEBUG) printf("Smctl value set. Value is: %d | Success is 0: %d\n", su.val, semscval);

        //closing file
        close(f);
    }
    //creation of shared memory

//Viewing
    if (strncmp(args[1], "-v", strlen(args[1])) == 0) {
        //return 1;
        showstory();
    }

//Removing
    if (strncmp(args[1], "-r", strlen(args[1])) == 0) {
        //removing semaphore
        semid = semget(semkey, 1, 0);
        /*if (semid == -1) {
            printf("error: %s\n", strerror(errno));
            return 1;
        }*/
        semscval = semctl(semid, 0, IPC_RMID);
        if (DEBUG) printf("Sem is removed, val should be 0. Value: %d\n", semscval);

        //removing shared memory
        struct shmid_ds buf;
        shmid = shmget(memkey, sizeof(int), 0);
        shmcval = shmctl(shmid, IPC_RMID, &buf);
        if (DEBUG) printf("Shared mem is removed, val should be 0. Value: %d\n", shmcval);

        //printing story
        showstory();
    }
    return 0;
}
