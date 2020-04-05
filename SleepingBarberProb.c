#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
	sem_t barber,customer,mutex;
	int count=0;
	void *GetHairCut(){
		while(1){	
			if(count==0){
		//barber will be sleeping as the count is zero means 
		//there is noo customer currently in the shop for hair cut
		sem_wait(&barber); 
		sem_wait(&mutex);

		//now when the customer will come he will wake up the barber
		//to ger a hair cut
		sem_post(&mutex);
		sem_post(&barber);
		}
			if(count==3){
		//as we can have max of 3 customer in waiting so this check will
		//make sure that not more then 3 customer are waiting 
		printf("Note:No more space for more customer/waiting room is full.\n");
		break;
		}
			else{
		//this check will increment the customer who are commin into the shop while
		//the barber is cutting the hair of the customer 
			count++;
			printf("customer count in waiting:%d\n",count-1);
		}
	}
		
}

	void *CutHair(){
		while(1){
		if(count>0){
		//cutting the hair of the customer
		sem_wait(&mutex);
		//will take one customer into the critical sention and put lock 
		//only one persons hair cut at one time
		sem_wait(&customer);
		//decrement means that barber is done with one customer
		count--;
			printf("customer count in waiting:%d\n",count);

		//will unlock the critical section after providing the hair to a single 
		//customer 
		sem_post(&customer);
		sem_post(&mutex);
		}
		else{
			printf("Note:No more customer available.\n");
			break;
		}
	}
}
int main(){
	sem_init(&mutex,0,1);
	sem_init(&barber,0,1);
	sem_init(&customer,0,1);
		pthread_t thread1,thread2;
		pthread_create(&thread1,NULL,GetHairCut,NULL);
		pthread_create(&thread2,NULL,CutHair,NULL);
		
		pthread_join(thread1,NULL);
		pthread_join(thread2,NULL);
		
		sem_destroy(&mutex);
		sem_destroy(&barber);
		sem_destroy(&customer);
}