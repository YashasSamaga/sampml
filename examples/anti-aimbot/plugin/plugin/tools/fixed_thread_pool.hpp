#include <queue>
#include <vector>
#include <thread>
#include <mutex>

template <class InputTaskItem, class OutputTaskItem, class TaskFunction>
class fixed_thread_pool {
public:
    fixed_thread_pool() { }
    ~fixed_thread_pool() { stop(); }

    bool empty() {
        std::lock_guard<std::mutex> lock(queue_lock);
        return output_queue.empty();
    }

    void enqueue(InputTaskItem& task) {
        std::unique_lock<std::mutex> lock(queue_lock);
        input_queue.push(task);
        lock.unlock();
        queue_not_empty.notify_one();
    }

    OutputTaskItem dequeue() {
        std::unique_lock<std::mutex> lock(queue_lock);
        assert(output_queue.size() > 0);
        OutputTaskItem item = output_queue.back();
        output_queue.pop();
        lock.unlock();
        return item;
    }

    void deqeue_all(std::vector<OutputTaskItem>& results) {
         std::unique_lock<std::mutex> lock(queue_lock);
         while (!output_queue.empty()) {
             results.push_back(output_queue.back());
             output_queue.pop();
         }
    }

    void start(int num = 2) {
        stop();

        stop_threads = false;
        for (int id = 0; id < num; id++) {
            threads.emplace_back([=] {
                TaskFunction task;
                while (true) {
                    std::unique_lock<std::mutex> lock(queue_lock);
                    queue_not_empty.wait(lock, [=] { return stop_threads || !input_queue.empty(); });

                    if (stop_threads && input_queue.empty())
                        break;

                    InputTaskItem input = input_queue.back();
                    input_queue.pop();
                    lock.unlock();

                    OutputTaskItem output;
                    task(input, output);

                    lock.lock();
                    output_queue.push(output);
                }
            });            
        } 
    }

    void stop() {
        std::unique_lock<std::mutex> lock(queue_lock);
        stop_threads = true;

        queue_not_empty.notify_all();

        for(auto& thread : threads)
            thread.join();
    }

private:
    std::vector<std::thread> threads;
    std::queue<InputTaskItem> input_queue;
    std::queue<OutputTaskItem> output_queue;
    std::condition_variable queue_not_empty;
    std::mutex queue_lock;

    bool stop_threads;
};