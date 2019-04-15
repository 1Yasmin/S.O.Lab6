#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <time.h>

#define THREAD_NUM 5
#define iter 4

FILE *pf;
sem_t mutex;
pthread_t thread_pool[THREAD_NUM];

void *exit_status; 

void delay(int number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
  
    // Stroing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not acheived 
    while (clock() < start_time + milli_seconds); 
}; 

void *funct_T(void *param){
	int i, tmp;
	for(i = 0; i < iter; i++){
		FILE *pft = fopen("BitacoraSemaforo.txt", "a+");
		fprintf(pft, "iteracion numero %d\n", i);
		fclose(pft);
		sem_wait(&mutex);
		FILE *pft2 = fopen("BitacoraSemaforo.txt", "a+");
		fprintf(pft2, "Semaforo activado por el thread no.%u\n", thread_pool[i]);
		fprintf(pft2, "Recurso tomado por el thread no.%u\n", thread_pool[i]);
		fclose(pft2);
		delay(200);
		FILE *pft3 = fopen("BitacoraSemaforo.txt", "a+");  
		fprintf(pft3, "Recurso devuelto por el thread no.%u\n", thread_pool[i]);
		fclose(pft3);
		sem_post(&mutex);
	};
}


int main(){
	//FILE *pf;
	int i, n;
	sem_init(&mutex, 0, 1);


	//creacion del fichero 
	pf = fopen("BitacoraSemaforo.txt", "w"); //abre archivo, si no existe lo crea y si existe su contenido se destruye
   if (pf == NULL){
      printf("El fichero no se puede abrir");
      exit(1); }
fclose(pf);

	//creacion de los threads
	for(i = 0; i < THREAD_NUM; i++){
		pthread_create(&thread_pool[i],NULL, funct_T, NULL);
		pf = fopen("BitacoraSemaforo.txt", "a+");
		fprintf(pf, "se creo el thread no.%u\n", thread_pool[i]);
		fclose(pf);
	};
	
	// finalizan los threads
	for(n = 0; n < THREAD_NUM; n++){
		pthread_join(thread_pool[n], &exit_status);
		pf = fopen("BitacoraSemaforo.txt", "a+");
		fprintf(pf, "se ha cerrado el thread no.%u\n", thread_pool[n]);
		fclose(pf);	
	};


	return 0;
}


