// Powered by nChain's Nakasendo libraries.
// See LICENSE.txt in project root for licensing information.

/*
 * A thread pool for firing off tasks in parallel.
 */

#ifndef _NCHAIN_SDK_THREAD_POOL_H_
#define _NCHAIN_SDK_THREAD_POOL_H_    

#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <queue>
#include <vector>

class ThreadPool final
{
  public:

    // Constructor
    ThreadPool(size_t threads = std::thread::hardware_concurrency());

    // Destructor
    ~ThreadPool();

    // Wait for everything to finish
    void waitAll();

    // Get number of threads in the pool
    size_t getNoThreads() const { return mNoThreads; }

    // Add a new task to the queue
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
    {   
        using return_type = typename std::result_of<F(Args...)>::type;
        
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
        std::future<return_type> res { task->get_future() };
        {
            std::unique_lock<std::mutex> lock { mQueueMtx };

            // Don't allow enqueueing after stopping the pool
            if(mStopping)
            {
                throw std::runtime_error("enqueue on stopped ThreadPool");
            }

            mTasks.emplace([task](){ (*task)(); });
        }
        mQueueCondVar.notify_one();
        return res;
    }

  private:

    // Need to keep track of threads so we can join them
    std::vector<std::thread> mWorkers {};
    size_t mNoThreads {};

    // The task queue
    std::queue<std::function<void()>> mTasks {};

    // Synchronization
    std::mutex mQueueMtx {};
    std::condition_variable mQueueCondVar {};
    bool mStopping {false};

};


#endif
