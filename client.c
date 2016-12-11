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
/*
union semun {
  int              val;
  struct semid_ds *buf;
  unsigned short  *array;
  struct seminfo  *__buf;

};
*/
int main() {
  //same key as control; semaphore magic
  int semkey = ftok("makefile", 23);
  int semid = semget(semkey, 1, 0);
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  //wait for the semaphore to become available
  sb.sem_op = 0;
  semop(semid, &sb, 1);
  //down it as soon as it is
  sb.sem_op = -1;
  semop(semid, &sb, 1);

  //shared memory magic
  int shmkey = ftok("control.c", 12);
  int shmid = shmget(shmkey, sizeof(long), 0);
  //Last Update Length
  long *lul = shmat(shmid, 0, 0);
  //Last Update
  char *lu = (char *)malloc(*lul + 1);
  
  //locating point in story and displaying last addition
  int fd = open("story.txt", O_RDWR | O_APPEND);
  lseek(fd, -(*lul), SEEK_END);
  read(fd, lu, *lul);
  printf("Last Update: %s\n", lu);
  printf("Type Here: ");

  //Next Update
  char nu[256];
  fgets(nu, 256, stdin);

  //since you need it for detach anyway
  *lul = strlen(nu);

  //cursor is at end of file, write length of input
  write(fd, nu, *lul);
  close(fd);
  
  shmdt(lul);
  
  //send the semaphore back skywards
  sb.sem_op = 1;
  semop(semid, &sb, 1);
  
  return 0;
}
