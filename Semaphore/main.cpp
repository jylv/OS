/*
 * Semaphore is a synchronization mechanism used to control access to shared resources by multiple threads or processes
 * Implement a simple C++ semaphore class, including P and V operations, and use a mutex lock to protect the semaphore value
 */

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>

using namespace std;

class Semaphore {
private:
    int value;
    mutex mtx;
    condition_variable cv;
public:
    Semaphore(int initialValue):value(initialValue){}
    /*
     * P operation
     * Decrements the semaphore's value (if it's positive).
     * If the value is zero, the calling thread will wait (block) until another thread calls signal().
     */
    void wait() {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock,[this]{return value>0;});
        --value;
    }
    /*
     * V operation
     * Increments the semaphore's value and notifies one waiting thread that a resource is available.
     */
    void signal() {
        unique_lock<mutex> lock(mtx);
        ++value;
        cv.notify_one();
    }
};

int main() {
    Semaphore semaphore(1);
    //Suppose we have two threads that want to access a shared resource
    //Thread 1
    {
        cout<<"Thread 1: Waiting for resource...\n";
        semaphore.wait();
        cout<<"Thread 1: Got resource, doing some work...\n";
        //Suppose we have some work here
        this_thread::sleep_for(chrono::seconds(1));
        cout<<"Thread 1: Work done, releasing resource...\n";
        semaphore.signal();
    }
    //Thread 2
    {
        cout<<"Thread 2: Waiting for resource...\n";
        semaphore.wait();
        cout<<"Thread 2: Got resource, doing some work...\n";
        //Suppose we have some work here
        this_thread::sleep_for(chrono::seconds(1));
        cout<<"Thread 2: Work done, releasing resource...\n";
        semaphore.signal();
    }
    return 0;
}
