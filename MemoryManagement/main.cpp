#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>

using namespace std;

//Number of page frames in physical memory
const int NUM_FRAMES = 4;
//Size of each page
const int PAGE_SIZE = 1024;
//Frame list used for LRU page replacement
list<int> frameList;
//Used to quickly find the position of a page in the frame list
unordered_map<int,list<int>::iterator> pageMap;
//Physical memory
vector<char> physicalMemory(NUM_FRAMES*PAGE_SIZE);

int allocateMemory(int processId, int size) {
    if(size>PAGE_SIZE) {
        cerr<<"Requested size exceeds page size!"<<endl;
        return -1;
    }
    //Find free frames or perform page replacement
    list<int>::iterator it;
    if(frameList.empty()) {
        it = frameList.begin();

    }
}


int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
