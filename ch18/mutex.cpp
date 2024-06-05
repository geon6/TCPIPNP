#include <cstdio>
#include <cstdlib>

#include <unistd.h>
#include <pthread.h>

#include <fmt/format.h>

constexpr int num_thread = 100;

void* thread_incr(void* arg);
void* thread_desc(void* arg);
long long num = 0;
pthread_mutex_t mutex;

int main() {
    pthread_t tid[num_thread];
    pthread_mutex_init(&mutex, NULL);
    fmt::println("sizeof long long: {}", sizeof(long long));
    for (int i = 0; i < num_thread; i++) {
        if (i % 2) {
            pthread_create(&(tid[i]), NULL, thread_incr, NULL);
        } else {
            pthread_create(&(tid[i]), NULL, thread_desc, NULL);
        }
    }
    for (int i = 0; i < num_thread; i++) {
        pthread_join(tid[i], NULL);
    }
    fmt::println("result: {}", num);
    pthread_mutex_destroy(&mutex);
}

void* thread_incr(void* arg) {
    for (int i = 0; i < 50000; i++) {
        pthread_mutex_lock(&mutex);
        num += 1;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* thread_desc(void* arg) {
    for (int i = 0; i < 50000; i++) {
        pthread_mutex_lock(&mutex);
        num -= 1;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}