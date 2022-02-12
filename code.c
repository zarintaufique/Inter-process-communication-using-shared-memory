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
	
	int pid = fork();
	if (pid == 0){
		
		sem_wait(s1);
		//filename reading as command line argument
		char* filename = (char*)malloc (sizeof(char)*40);
		filename = strdup(argv[1]);
		int fd = open (filename, O_RDONLY);
		
		char* temp = ( char* ) malloc (sizeof (char) * 100);
		int bytesread = 0, d = 0;
		 size = read (fd, temp, 100);
		int* buffer1 = ( int* ) malloc (sizeof (int) * size);	
		int i = 0;
			//conversion from char array to int
		for(;sscanf(temp, "%d%n", &d, &bytesread) > 0; i++)
		{
			buffer1[i] = d; //converting to int array
			temp = temp + bytesread;
		}
			close(fd);
		
		sem_post(s2);  
		
		sem_wait(s3);
		shmdt(buffer1);
		
		sem_wait(s3);
		int fd1 = open(filename, O_WRONLY);
		write(fd1, buffer2, size);
		close(fd1);
		int j = 0;
		for( j = 0;j < sizeof(buffer2);j++ ){
			printf("%d\n", buffer2[j] );
		}
		sem_post(s4);
	}
	else {
			sem_wait(s2);
			int size1 = sizeof(buffer1);
			int a = 0, k = 0, j = 0, i = 0;
			int* buffer2 = (int*) malloc (sizeof(int) * size1);
			for ( i = 0;i < size1 - 1;i++) {
				for( j = i + 1;j < size1;j++) {
					if(buffer1[i] == buffer1[j])
						a++;
				}
				//no duplicate
				if(a == 0){
					buffer2[k] = buffer1[i];
					k++;
				}
				a = 0;
			}
			sem_post(s3);
			sem_wait(s4);

			//detachment of the pointers
			shmdt(s1);
			shmdt(s2);
			shmdt(buffer1);
			//deletion of all shared memories
			shmctl(skey, IPC_RMID, NULL);
			shmctl(skey1, IPC_RMID, NULL);
			shmctl(skey2, IPC_RMID, NULL);
			shmctl(skey3, IPC_RMID, NULL);
			shmctl(id_cs1, IPC_RMID, NULL);
			shmctl(id_cs2, IPC_RMID, NULL);

			sem_destroy(s1);
			sem_destroy(s2);
			sem_destroy(s3);
			sem_destroy(s4);

	}
	return 0;
}
