// Powered by nChain's Nakasendo libraries.
// See LICENSE.txt in project root for licensing information.

/*
 * A thread pool for firing off mTasks in parallel.
 */

#include "ThreadPool.h"

// Constructor
ThreadPool::ThreadPool(size_t threads)
: mNoThreads{threads}
{   
    for(size_t i = 0; i < threads; ++i)
    {   
        mWorkers.emplace_back(
            [this]
            {   
                for(;;)
                {   
                    std::function<void()> task {};

                    {   
                        std::unique_lock<std::mutex> lock { this->mQueueMtx };
                        this->mQueueCondVar.wait(lock,
                            [this]{ return this->mStopping || !this->mTasks.empty(); }
                        );

                        if(this->mStopping && this->mTasks.empty())
                        {
                            return;
                        }

                        task = std::move(this->mTasks.front());
                        this->mTasks.pop();
                    }

                    task();
                }
            }
        );
    }
}

// Destructor
ThreadPool::~ThreadPool()
{
    waitAll();
}

// Wait for everything to finish
void ThreadPool::waitAll()
{
    {
        // Wake everyone up
        std::unique_lock<std::mutex> lock { mQueueMtx };
        mStopping = true;
        mQueueCondVar.notify_all();
    }

    // Reap all the workers
    for(std::thread& worker: mWorkers)
    {
        worker.join();
    }
    mWorkers.clear();
}


