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

int main(int argnum, char **args) {
    //random num gen for key.
    sranddev();
    int randnum = rand();

    //creation of file.
    umask(0000);
    f = open("./story.txt", O_CREAT, 0644);
    umask(0022);

    int semid;
    int key = ftok("story.txt", randnum);

    if (strcmp(args[1] == "-c")) {
        semid = semget(key, 1, IPC_CREAT | 0644);
        semctl(semid, 0)
    }

    return 0;
}
