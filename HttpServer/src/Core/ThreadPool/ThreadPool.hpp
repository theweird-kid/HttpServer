#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include "../Socket/Socket.hpp"
#include "../TCP/RequestHandler.hpp"

#include <optional>
#include <thread>
#include <vector>
#include <mutex>
#include <queue>
#include <condition_variable>

class ThreadPool {
public:

    ThreadPool(size_t numThreads, RequestHandler& reqHandler)
		: requestHandler{ reqHandler }, stop{ false }
    {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::optional<Socket> clientSocket;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        condition.wait(lock, [this] { return !tasks.empty() || stop; });
                        if (stop && tasks.empty()) return;
                        clientSocket.emplace(std::move(tasks.front()));
                        tasks.pop();
                    }
                    if (clientSocket->Get() != INVALID_SOCKET) {
						requestHandler.handleRequest(std::move(*clientSocket));
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

    void Enqueue(Socket clientSocket) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.push(std::move(clientSocket));
        }
        condition.notify_one();
    }

private:

    std::vector<std::thread> workers;
    std::queue<Socket> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
	RequestHandler& requestHandler;
    bool stop = false;
};

#endif // THREADPOOL_HPP