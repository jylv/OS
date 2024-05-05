/*
* The following is a simple C++ multi-threaded program example,
* It uses the std::thread library to create and manage threads,
* std::mutex is used to ensure mutually exclusive access between threads.
* std::condition_variable to synchronize threads.
*
* In this example we have two tasks:
* One thread is responsible for producing data,
* Another thread is responsible for consuming data.
* Producers and consumers share a fixed-size buffer and need to ensure mutually exclusive access and synchronization.
 */
#include <iostream>
#include <thread>
#include <queue>

using namespace std;
//Size of shared buffer
const int BUFFER_SIZE = 10;
//Data structures shared by producers and consumers
class SharedBuffer {
private:
    //A queue to store the data items.
    queue<int> buffer_;
    mutex mutex_;
    //A condition variable to signal when there is data available for consumption
    condition_variable not_empty_;
    //A condition variable to signal when there is space available for new data
    condition_variable not_full_;
public:
    SharedBuffer():buffer_(),mutex_(),not_empty_(),not_full_(){}
    /*
     * Adds data to the buffer
     * First locks the mutex to ensure exclusive access to the buffer.
     * waits until the buffer is not full
     * add data to the buffer
     * notifies one waiting consumer thread that the buffer is not empty
     * releases the lock
     */
    void produce(int data) {
        unique_lock<mutex> lock(mutex_);
        not_full_.wait(lock,[this]{return buffer_.size()<BUFFER_SIZE;});
        buffer_.push(data);
        lock.unlock();
        not_empty_.notify_one();
    }
    /* Retrieves data from the buffer.
     * First locks the mutex to ensure exclusive access to the buffer.
     * waits until the buffer is not empty
     * consume data from the front of the buffer
     * notifies one waiting producer thread that the buffer is not full
     * releases the lock and returns the consumed data.
     */
    int comsume() {
        unique_lock<mutex> lock(mutex_);
        not_empty_.wait(lock,[this]{return !buffer_.empty();});
        int data = buffer_.front();
        buffer_.pop();
        lock.unlock();
        not_full_.notify_one();
        return data;
    }
};
/*
 * Simulates a producer's behavior.
 * It generates data (unique to each producer) and adds it to the shared buffer
 */
void producer(SharedBuffer& buffer,int id) {
    for(int i=0;i<20;++i) {
        //Simulate the time it takes to produce data
        this_thread::sleep_for(chrono::milliseconds(100));
        int data = id*100+i;
        buffer.produce(data);
        cout<<"Producer "<<id<<" produced "<<data<<endl;
    }
}
/*
 * Simulates a consumer's behavior.
 * It retrieves data from the shared buffer
 */
void consumer(SharedBuffer& buffer,int id) {
    for(int i=0;i<20;++i) {
        int data = buffer.comsume();
        //Simulate the time it takes to consume data
        this_thread::sleep_for(chrono::milliseconds(200));
        cout<<"Consumer "<<id<<" consumed "<<data<<endl;
    }
}

int main() {
    SharedBuffer buffer;

    //Create two producer threads and consumer threads
    thread producer1(producer,ref(buffer),1);
    thread producer2(producer,ref(buffer),2);
    thread consumer1(consumer,ref(buffer),1);
    thread consumer2(consumer,ref(buffer),2);

    //This ensures that the main thread waits for all other threads to finish their execution before exiting.
    producer1.join();
    producer2.join();
    consumer1.join();
    consumer2.join();

    return 0;
}
