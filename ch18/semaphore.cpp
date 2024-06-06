#include <cstdio>
#include <iostream>

#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include <fmt/format.h>

void* read(void* arg);
void* accu(void* arg);
static sem_t sem_one;
static sem_t sem_two;
static int num;

int main() {
    pthread_t tid1, tid2;
    sem_init(&sem_one, 0, 0);
    sem_init(&sem_two, 0, 1);

    pthread_create(&tid1, NULL, read, NULL);
    pthread_create(&tid2, NULL, accu, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    sem_destroy(&sem_one);
    sem_destroy(&sem_two);
}

void* read(void* arg) {
    for (int i = 0; i < 5; i++) {
        fmt::println("Input num: ");
        sem_wait(&sem_two);
        std::cin >> num;
        sem_post(&sem_one);
    }
    return NULL;
}

void* accu(void* arg) {
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        sem_wait(&sem_one);
        sum += num;
        sem_post(&sem_two);
    }
    fmt::println("result: {}", sum);
    return NULL;
}
