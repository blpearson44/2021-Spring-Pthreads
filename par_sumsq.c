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
long min = INT_MIN;
long max = INT_MAX;
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
  printf("Made it to the function\n");
  long temp = (long) num;
  printf("%ld", temp);
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
  printf("There are %d threads working on %s\n", num_threads, filename);

  // initialize threads
  pthread_t thr_arr[num_threads];

  // read from file
  printf("Reading files\n");
  FILE* fin = fopen(filename, "r");
  char action;
  long num;
  while(fscanf(fin, "%c %ld\n", &action, &num) == 2) {
    if (action == 'p'){
      // wait for a thread to be available
      
      // create thread and have it calculate square
      pthread_create(&thr_arr[active_threads++], NULL, thr_fn, (void *) &num);

    }
    else if (action == 'w'){
      sleep(num);
    }
    else{
      printf("File error\n");
      exit(EXIT_FAILURE);
    }
  }

  printf("Sum:\t%ld\nOdd:\t%ld\nMin:\t%ld\nMax:\t%ld\n", sum, odd, min, max);
}
