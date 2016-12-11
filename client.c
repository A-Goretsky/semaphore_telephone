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
  int semkey = ftok("makefile", 23);
  int semid = semget(semkey, 1, 0644);
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = 0;
  semop(semid, &sb, 1);
  sb.sem_op = -1;
  semop(semid, &sb, 1);

  int shmkey = ftok("control.c", 12);
  int shmid = shmget(shmkey, sizeof(long), 0644);
  long *lul = shmat(shmid, 0, 0);
  char *lu = (char *)malloc(*lul + 1);
  
  int fd = open("story.txt", O_RDWR | O_APPEND, 0644);
  lseek(fd, -(*lul), SEEK_END);
  read(fd, lu, *lul);
  printf("Last Update: %s\n", lu);
  printf("Type Here: ");

  char nu[512];
  fgets(nu, 256, stdin);

  *lul = strlen(nu);

  write(fd, nu, *lul);
  close(fd);
  
  shmdt(lul);

  sb.sem_op = 1;
  semop(semid, &sb, 1);
  
  return 0;
}
