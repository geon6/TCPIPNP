add_executable(thread1 thread1.cpp)

add_executable(thread2 thread2.cpp)

add_executable(thread3 thread3.cpp)
target_compile_definitions(thread3 PUBLIC _REENTRANT)

add_executable(thread4 thread4.cpp)
target_compile_definitions(thread4 PUBLIC _REENTRANT)

add_executable(mutex mutex.cpp)
target_compile_definitions(mutex PUBLIC _REENTRANT)

add_executable(semaphore semaphore.cpp)
target_compile_definitions(semaphore PUBLIC _REENTRANT)

add_executable(chat_server chat_server.cpp)
target_compile_definitions(chat_server PUBLIC _REENTRANT)

add_executable(chat_client chat_client.cpp)
target_compile_definitions(chat_client PUBLIC _REENTRANT)