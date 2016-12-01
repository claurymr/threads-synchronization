#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <vector>
#include "proj06.student.h"

#define NLOOPS 100
#define BUFFERSIZE 20

struct Record
{
  int threadID;
  int value;
};

int count = 0;
int producerThreads = 1;
int in = 0, out = 0;

sem_t e;
sem_t s;
sem_t n;

vector<Record> buffer(BUFFERSIZE);

void *producer(void*);

void *consumer(void*);

Record produce(long int, int);

int main(int argc, char *argv[])
{
  
  int consumerThreads = 1;

  sem_init(&e, 0, BUFFERSIZE);
  sem_init(&s, 0, 1);
  sem_init(&n, 0, 0);

  if((argc == 2) && (atoi(argv[argc-1]) <= 10))
    {
      producerThreads = atoi(argv[argc-1]);
    }

  pthread_t producers[producerThreads];
  pthread_t consumers;

  cout << "pthreads: " << producerThreads << endl;
  cout << "cthreads: " << consumerThreads << endl;
  cout << "size of buffer: " << buffer.size() << endl << endl;

  /*for(int i = 0; i < BUFFERSIZE; i++)
    {
      buffer[i] = Record{0,0};      
      }*/
  

  if(pthread_create(&consumers, NULL, &consumer, NULL))
    {
      cout << "Error: Unable to create consumer thread" << endl;
      exit(-1);
    }

  for(long int i = 0; i < producerThreads; i++)
    {
      if(pthread_create(&producers[i], NULL, &producer, (void*)i))
	{
	  cout << "Error: Unable to create producer thread" << endl;
	  exit(-1);
	}
    }

  for(long int j = 0; j < producerThreads; j++)
      {
	pthread_join(producers[j], NULL);
      }

  pthread_exit(NULL);
  
  return 0;
}

void *producer(void* thread_id)
{
  long int th_id;
  th_id = (long)thread_id;
  for(int i = 0; i < NLOOPS; i++)
    {      
      Record itemToBeInserted = produce(th_id, i);
      
      /*if((in + 1) % BUFFERSIZE != out)
	{*/
      sem_wait(&e);
      sem_wait(&s);
      
      buffer[in] = itemToBeInserted;
      in = (in + 1) % BUFFERSIZE;
      //count++;
      
      sem_post(&s);
      sem_post(&n);
	  //}
    }
  return NULL;
  
}

void *consumer(void* arg)
{
  
  Record itemToBeRemoved;
  
  for(int i = 0; i < producerThreads*NLOOPS; i++)
    {      
      sem_wait(&n);
      sem_wait(&s);
      
      itemToBeRemoved = buffer[out];
      out = (out + 1) % BUFFERSIZE;
      //count--;
      sem_post(&s);
      sem_post(&e);
      cout << "Thread Id: " << itemToBeRemoved.threadID << " Value: "
	   << itemToBeRemoved.value << endl;
      
    }
  return NULL;
}


Record produce(long int th_id, int count)
{
  Record itemToBeInserted;
  itemToBeInserted.threadID = th_id;
  itemToBeInserted.value = count;
  
  return itemToBeInserted;
}

/*void consume(long index)
{
  Record itemToBeInserted;
  itemToBeInserted.threadID = th_id;
  itemToBeInserted.value = count;
  
  return itemToBeInserted;
  }*/


