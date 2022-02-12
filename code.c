#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>
#include <sys/shm.h>

int main(int argc, char* argv[]) { 
	
	int size = 0;
	
	//filename reading as command line argument
	char* filename = (char*)malloc (sizeof(char)* 40);
	filename = strdup(argv[1]);
	int skey = shmget(IPC_PRIVATE, 200, 0666 | IPC_CREAT | IPC_EXCL);
	int skey1 = shmget(IPC_PRIVATE, 200, 0666 | IPC_CREAT | IPC_EXCL);
	int skey2 = shmget(IPC_PRIVATE, 200, 0666 | IPC_CREAT | IPC_EXCL); 
	int skey3 = shmget(IPC_PRIVATE, 200, 0666 | IPC_CREAT | IPC_EXCL); 
	int id_cs1= shmget(IPC_PRIVATE, 200, 0666| IPC_CREAT | IPC_EXCL);//buffer1
	int id_cs2= shmget(IPC_PRIVATE, 200, 0666| IPC_CREAT | IPC_EXCL);//buffer2
	
	if(skey == -1 || skey1 == -1 || skey2 == -1) {
		printf("error occured in creating shared memory\n");
		return 1;
	}
	sem_t *s1;
	sem_t *s2;
	sem_t *s3;
	sem_t *s4;
	
	int* buffer1 = (int*)shmat(id_cs1, NULL, 0);
	int* buffer2 = (int*)shmat(id_cs2, NULL, 0);
	s1= (sem_t*) shmat(skey, NULL, 0);
	s2= (sem_t*) shmat(skey1, NULL, 0);
	s3= (sem_t*) shmat(skey2, NULL, 0);
	s4= (sem_t*) shmat(skey3, NULL, 0);
	sem_init(s1, 1, 1);    //unlocked
	sem_init(s2, 1, 0);   //locked
	sem_init(s3, 1, 0);    //locked
	sem_init(s4, 1, 0);    //locked

	shmdt(s1);
	shmdt(s2);
	shmdt(s3);
	shmdt(s4);
	
	
	return 0;
}
