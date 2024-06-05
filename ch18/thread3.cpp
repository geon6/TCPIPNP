#include <cstdio>
#include <cstdlib>

#include <unistd.h>
#include <pthread.h>

#include <fmt/format.h>

void* thread_summation(void* arg);

int sum = 0;

int main() {
    pthread_t tid1, tid2;
    auto p1 = std::make_pair(1, 500);
    auto p2 = std::make_pair(501, 1000);

    pthread_create(&tid1, NULL, thread_summation, (void*)&p1);
    pthread_create(&tid2, NULL, thread_summation, (void*)&p2);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    fmt::println("result: {}", sum);
}

void* thread_summation(void* arg) {
    auto pairp = (std::pair<int, int>*)arg;
    for (int i = pairp->first; i <= pairp->second; i++) {
        sum += i;
    }
    return NULL;
}
