#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include <unistd.h>
#include <pthread.h>

#include <fmt/format.h>

void* thread_main(void* arg);

int main() {
    pthread_t tid;
    int thread_param = 5;
    void* thread_return;

    if (pthread_create(&tid, NULL, thread_main, (void*)&thread_param) != 0) {
        fmt::println("pthread_create() error");
        return -1;
    }

    if (pthread_join(tid, &thread_return) != 0) {
        fmt::println("pthread_join() error");
        return -1;
    }

    std::string* msg = (std::string*)thread_return;
    fmt::println("Thread return message: {}", *msg);
    delete msg;
}

void* thread_main(void* arg) {
    int param = *((int*)arg);
    std::string* msg = new std::string{"hello, i am a message in thread"};
    for (int i = 0; i < param; i++) {
        sleep(1);
        fmt::println("running thread");
    }
    return (void*)msg;
}
