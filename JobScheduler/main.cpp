#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Job {
    int jobId;
    int priority;
    int burstTime;
    int waitTime;
    int turnaroundTime;

    Job(int id,int prio,int burst):jobId(id),priority(prio),burstTime(burst),waitTime(0),turnaroundTime(0){}

    bool operator<(const Job& other) const {
        return priority<other.priority;
    }
};

struct JobScheduler {
private:
    priority_queue<Job> readyQueue;
    vector<Job> jobHistory;
    int currentTime;
public:
    JobScheduler():currentTime(0){}

    void addJob(const Job& job) {
        readyQueue.push(job);
    }

    Job* schedule() {
        if(readyQueue.empty()) {
            return nullptr;
        }
        Job currentJob = readyQueue.top();
        readyQueue.pop();

        currentJob.waitTime = currentTime;
        currentJob.turnaroundTime = currentJob.waitTime + currentJob.burstTime;

        //Simulate job execution
        cout<<"Executing job "<< currentJob.jobId<<" with priority "<<currentJob.priority
        <<", burst time "<<currentJob.burstTime<<", wait time "<<currentJob.waitTime<<endl;

        currentTime += currentJob.burstTime;
        jobHistory.push_back(currentJob);

        return &jobHistory.back();
    }

    int getCurrentTime() const {
        return currentTime;
    }

    const vector<Job>& getJobHistory() const {
        return jobHistory;
    }
};

int main() {
    JobScheduler scheduler;

    scheduler.addJob(Job(1,3,5));
    scheduler.addJob(Job(2,1,2));
    scheduler.addJob(Job(3,4,3));

    Job* job;
    while((job=scheduler.schedule())!=nullptr) {
        // do something here
    }

    for(const auto& job: scheduler.getJobHistory()) {
        cout<<"Job "<<job.jobId<<": Turnaround Time = "<<job.turnaroundTime<<endl;
    }

    return 0;
}
