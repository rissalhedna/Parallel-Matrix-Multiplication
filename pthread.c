#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define MATRIX_SIZE 1000
#define NUM_THREADS 60

int matrixA[MATRIX_SIZE][MATRIX_SIZE];
int matrixB[MATRIX_SIZE][MATRIX_SIZE];
int matrixC[MATRIX_SIZE][MATRIX_SIZE];

// Define a struct to hold the data that will be passed to each thread
typedef struct {
    int thread_id;
    int num_threads;
} thread_data;

// Define the function that each thread will run
void* multiply_matrices(void* arg) {
    thread_data* data = (thread_data*) arg;
    int start = data->thread_id * (MATRIX_SIZE / data->num_threads);
    int end = start + (MATRIX_SIZE / data->num_threads);
    for (int i = start; i < end; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrixC[i][j] = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
    pthread_exit(NULL);
}

int main() {
    // Initialize the matrices with random values
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrixA[i][j] = rand() % 10;
            matrixB[i][j] = rand() % 10;
        }
    }

    // Start the timer
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    // Multiply the matrices using pthreads
    pthread_t threads[NUM_THREADS];
    thread_data thread_args[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i].thread_id = i;
        thread_args[i].num_threads = NUM_THREADS;
        pthread_create(&threads[i], NULL, multiply_matrices, &thread_args[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Stop the timer and print the elapsed time
    gettimeofday(&end_time, NULL);
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
    printf("Elapsed time using %d threads: %f seconds\n", NUM_THREADS, elapsed_time);

    // Multiply the matrices serially
//     gettimeofday(&start_time, NULL);
//     for (int i = 0; i < MATRIX_SIZE; i++) {
//         for (int j = 0; j < MATRIX_SIZE; j++) {
//             matrixC[i][j] = 0;
//             for (int k = 0; k < MATRIX_SIZE; k++) {
//                 matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
//             }
//         }
//     }
//     gettimeofday(&end_time, NULL);
//     elapsed_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
//    printf("Elapsed time using 1 thread: %f seconds\n", elapsed_time);

    return 0;
}