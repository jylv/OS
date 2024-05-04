/*
*  A scheduler that schedules processes according to priority
 */

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

struct Process {
    int id;
    int priority;
    int burst_time;
    Process(int id,int priority,int burst_time):id(id),priority(priority),burst_time(burst_time){}
    //Custom comparison function for priority queue
    bool operator<(const Process& other) const {
        return priority > other.priority;//min heep
    }
};

struct ProcessScheduler {
private:
    priority_queue<Process> ready_queue;
public:
    void add_process(int id,int priority,int burst_time) {
        Process process(id,priority,burst_time);
        ready_queue.push(process);
    }
    void schedule() {
        while(!ready_queue.empty()) {
            //Get thread with highest priority
            Process current_process = ready_queue.top();
            ready_queue.pop();
            cout<<"Executing Process "<<current_process.id<<" with priority "<<current_process.priority<<" and burst time "<<current_process.burst_time<<endl;
            //Simulate process execution
            for(int i=0;i<current_process.burst_time;++i) {
                //some action here
                cout<<".";
            }
            cout<<endl;
        }
    }
};
int main() {
    ProcessScheduler scheduler;

    scheduler.add_process(1,3,5);
    scheduler.add_process(2,1,2);
    scheduler.add_process(3,4,1);

    scheduler.schedule();
    return 0;
}
