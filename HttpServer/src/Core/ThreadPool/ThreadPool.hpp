#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include "../Socket/Socket.hpp"
#include "../TCP/Parser.hpp"
#include "../TCP/MyRequestHandler.hpp"

#include <optional>
#include <thread>
#include <vector>
#include <mutex>
#include <queue>
#include <condition_variable>

class ThreadPool {
public:
    
    ThreadPool(size_t numThreads) {
         for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::optional<MyRequestHandler> clientHandler;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        condition.wait(lock, [this] { return !tasks.empty() || stop; });
                        if (stop && tasks.empty()) return;
                        clientHandler.emplace(std::move(tasks.front()));
                        tasks.pop();
                    }
                    if (clientHandler) {
                        clientHandler->handleRequest();
                    }
                }
            });
         }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread& worker : workers) {
            worker.join();
        }
    }

    void Enqueue(MyRequestHandler clientHandler) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.push(std::move(clientHandler));
        }
        condition.notify_one();
    }

private:

    std::vector<std::thread> workers;
    std::queue<MyRequestHandler> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop = false;
};

#endif // THREADPOOL_HPP
