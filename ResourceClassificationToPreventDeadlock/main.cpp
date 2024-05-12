#include <iostream>
#include <vector>
#include <unordered_map>
#include <stdexcept>

using namespace std;

class Resource {
private:
    int level_;
    bool available_;
public:
    Resource(int level):level_(level),available_(true){}

    int getLevel() const {return  level_;}
    bool isAvailable() const {return available_;};

    void request() {
        if(!available_) {
            throw runtime_error("Resource is not avilable!");
        }
        available_ = false;
    }

    void release() {
        available_ = true;
    }
};

class Process {
private:
    vector<int> resourceLevels_;
    unordered_map<int, unique_ptr<Resource>> resources_;
public:
    Process(const vector<int>& resourceLevels):resourceLevels_(resourceLevels){}

    void execute() {
        for(int level:resourceLevels_) {
            resources_.emplace(level,make_unique<Resource>(level));
        }
        try {
            for(int level:resourceLevels_) {
                resources_.at(level)->request();
                cout<<"Please requested resource with level"<<level<<endl;
            }

            // more process action here

            for(int level:resourceLevels_) {
                resources_.at(level)->release();
                cout<<"Please released resource with level"<<level<<endl;
            }
        }catch(exception& e) {
            cerr<<"Error:"<<e.what()<<endl;
        }
    }
};

int main() {
    vector<int> resourceLevels = {1,2,3};
    Process process(resourceLevels);
    process.execute();
    return 0;
}
