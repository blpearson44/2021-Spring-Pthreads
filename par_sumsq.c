/*
 * Ben Pearson
 * CS 446 Project 5
 * Implementing Pthreads in C99
 *
 * Chose to recreate file from scratch
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <limits.h>
#include <unistd.h>


// shared variables
long sum = 0;
long odd = 0;
long min = INT_MAX;
long max = INT_MIN;
bool done = false;
// this variable will track how many threads are actively being used
int active_threads = 0;

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
void * thr_fn(void* num){
  long temp = *((long *) num);
  printf("Made it to the: value:\t");
  printf("%ld\n", temp);
  calculate_square(temp);
  active_threads--;
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
    printf("Please enter a positive integer greater than one.\n");
    exit(EXIT_FAILURE);
  }
  // verify that input is working
  printf("There are %d thread(s) working on %s\n", num_threads, filename);

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
      // wait for a thread to be available
      while(active_threads >= num_threads);
      // create thread and have it calculate square
      printf("Execution %d \n", thrs++);
      num_arr[active_threads] = num;
      pthread_create(&thr_arr[active_threads++], NULL, thr_fn, (void *) &num_arr[active_threads]);
    }
    else if (action == 'w'){
      printf("Sleeping\n");
      sleep(num);
    }
    else{
      printf("File error\n");
      exit(EXIT_FAILURE);
    }
  }
  for(int i = 0; i < num_threads; i++){
    pthread_join(thr_arr[i], NULL);
  }
  printf("Sum:\t%ld\nOdd:\t%ld\nMin:\t%ld\nMax:\t%ld\n", sum, odd, min, max);
}
