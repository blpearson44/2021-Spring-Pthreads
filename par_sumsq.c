/*
 * Ben Pearson
 * CS 446 Project 5
 * Implementing Pthreads in C99
 *
 * Chose to recreate file from scratch
*/
/* TODO
 * [ ] Implement task list as singly-linked list
 * [ ] Use mutex to protect task queue and protect global aggregate variables
 * [ ] Remove extra printf statements
 * [ ] Don't pass parameters to workers, use global variables to coordinate
*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <limits.h>
#include <unistd.h>

// Queue implemented with a singly linked list
typedef struct node
{
  long data;
  struct node * next;
}node;

typedef struct queue
{
  int count;
  node * head;
  node * tail;
}queue;

// shared variables
long sum = 0;
long odd = 0;
long min = INT_MAX;
long max = INT_MIN;
bool done = false;
// this variable will track how many threads are actively being used
int active_threads = 0;
queue task_queue;
// thread locker
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t count_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t que_empty = PTHREAD_COND_INITIALIZER;


void init(queue* q){
  q->count = 0;
  q->head = NULL;
  q->tail = NULL;
}
// basic queue operations
void enqueue(queue *q, long data){
  node * temp = (node*)malloc(sizeof(node));
  temp->data = data;
  if(q->head==NULL){
    q->head = temp;
  }
  else{
    q->tail->next = temp;
  }
  q->tail = temp;

  pthread_mutex_unlock(&count_lock);
  q->count++;
  pthread_mutex_unlock(&count_lock);

}
void dequeue(queue *q){
  if(q->count < 1){
    return; // check for null
  }
  node * temp = q->head;
  q->head = q->head->next;
  temp->next = NULL;
  free(temp);
  pthread_mutex_lock(&count_lock);
  q->count--;
  pthread_mutex_unlock(&count_lock);
}
bool is_empty(queue *q){
  return q->head == NULL;
}

// to print the queue for testing purposes
void print_links(node* n){
  if(n != NULL){
    printf("P %ld\t", n->data);
    print_links(n->next);
  }
  return;
}
void print_queue(queue* q){
  printf("%d Nodes\n", q->count);
  print_links(q->head);
  printf("\n");
}






void calculate_square(long number){
  long square = number * number;
  // pause to simulate comp time
  sleep(number);
  sum += square;
  // check for odd number
  if (number % 2 == 1) {
    odd++;
  }
  if (number < min){
    min = number;
  }
  if (number > max){
    max = number;
  }
}

// Thread function
void * thr_fn(){
  pthread_mutex_lock(&lock);
  long temp = task_queue.head->data;
  dequeue(&task_queue);
  pthread_mutex_unlock(&lock);
  calculate_square(temp);
  active_threads--;

  pthread_mutex_lock(&lock);
  while(is_empty(&task_queue) && !done){
    pthread_cond_wait(&que_empty, &lock);
  }
  pthread_mutex_unlock(&lock);
}

int main(int argc, char* argv[])
{
  // check command line usage
  if (argc != 3){
    printf("Usage: sumsq <infile> <numthreads>\n");
    exit(EXIT_FAILURE);
  }
    
  // get data from command line
  char *filename = argv[1];
  int num_threads = atoi(argv[2]);
  // check that threads input is valid
  if (num_threads < 1){
    printf("Second argument must be a positive integer greater than one.\n");
    exit(EXIT_FAILURE);
  }
  // initialize threads
  pthread_t thr_arr[num_threads];
  long num_arr[num_threads];

  // read from file
  FILE* fin = fopen(filename, "r");
  char action;
  long num;
  int thrs = 0; // for debugging
  while(fscanf(fin, "%c %ld\n", &action, &num) == 2) {
    if (action == 'p'){
      pthread_mutex_lock(&lock);
      enqueue(&task_queue, num);
      pthread_cond_signal(&que_empty);
      pthread_mutex_unlock(&lock);
    }
    else if (action == 'w'){
      sleep(num);
    }
    else{
      printf("File error\n");
      exit(EXIT_FAILURE);
    }
    if (active_threads < num_threads && !(is_empty(&task_queue)))
    {
      pthread_create(&thr_arr[active_threads++], NULL, thr_fn, NULL);
    }
  }
  done = true;
  while(!(is_empty(&task_queue))){
    pthread_cond_signal(&que_empty);
    if(active_threads<num_threads){
      pthread_create(&thr_arr[active_threads++], NULL, thr_fn, NULL);
    }
  }
  for(int i = 0; i < num_threads; i++){
    pthread_join(thr_arr[i], NULL);
  }

  pthread_mutex_destroy(&lock);
  printf("Sum:\t%ld\nOdd:\t%ld\nMin:\t%ld\nMax:\t%ld\n", sum, odd, min, max);
}
