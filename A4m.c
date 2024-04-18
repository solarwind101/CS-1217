/*
Suraj Sharam
CS 1217 - Operating System
Assignment 4

Floyd Warshall Algorithm using Multithreading

  */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define V 100 // Maximum number of nodes
#define INF 99999

int dist[V][V];
pthread_mutex_t lock;

void* floydWarshall(void* arg) {
    int* indices = (int*)arg;
    int k = indices[0];
    int i = indices[1];
    free(arg); // Free the dynamically allocated memory

    for (int j = 0; j < V; j++) {
        if (dist[i][k] + dist[k][j] < dist[i][j]) {
            pthread_mutex_lock(&lock); // Acquire lock only when update is needed
            dist[i][j] = dist[i][k] + dist[k][j];
            pthread_mutex_unlock(&lock);
        }
    }

    pthread_exit(NULL);
}

void print_matrix(int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (dist[i][j] == INF)
                printf("%7s", "INF");
            else
                printf("%7d", dist[i][j]);
        }
        printf("\n");
    }
}


int main() {
    int N, M;
    if (scanf("%d %d", &N, &M) != 2) {
        fprintf(stderr, "Invalid input format.\n");
        exit(1);
    }

    pthread_t threads[V][V];
    pthread_mutex_init(&lock, NULL);

    // Initialize dist matrix
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (i == j)
                dist[i][j] = 0;
            else
                dist[i][j] = INF;
        }
    }

    // Read graph edges and weights
    int ui, vi, w;
    for (int i = 0; i < M; i++) {
        if (scanf("%d %d %d", &ui, &vi, &w) != 3) {
            fprintf(stderr, "Invalid input format.\n");
            exit(1);
        }
        dist[ui - 1][vi - 1] = dist[vi - 1][ui - 1] = w;
    }

    // Loop through each vertex k
    for (int k = 0; k < N; k++) {
        // Create N threads for each iteration of the k loop
        for (int i = 0; i < N; i++) {
            int* indices = malloc(2 * sizeof(int));
            if (indices == NULL) {
                perror("malloc failed");
                exit(1);
            }
            indices[0] = k; // Copy the value of k
            indices[1] = i; // Copy the value of i
            if (pthread_create(&threads[k][i], NULL, floydWarshall, indices) != 0) {
                perror("pthread_create failed");
                exit(1);
            }
        }

        // Wait for all threads to finish before proceeding to the next iteration of k
        for (int i = 0; i < N; i++) {
            if (pthread_join(threads[k][i], NULL) != 0) {
                perror("pthread_join failed");
                exit(1);
            }
        }
    }

    // Print the shortest distance matrix
    print_matrix(N);

    pthread_mutex_destroy(&lock);
    return 0;
}
