#include <cstdio>

#include <unistd.h>
#include <pthread.h>

#include <fmt/format.h>

void* thread_main(void* arg);

int main() {
    pthread_t tid;
    int thread_param = 5;
    if (pthread_create(&tid, NULL, thread_main, (void*)&thread_param) != 0) {
        fmt::println("pthread_create() error");
        return -1;
    }
    sleep(10);
    fmt::println("end of main");
}

void* thread_main(void* arg) {
    int param = *((int*)arg);
    fmt::println("thread param: {}", param);
    for (int i = 0; i < param; i++) {
        sleep(1);
        fmt::println("running thread");
    }
    return NULL;
}