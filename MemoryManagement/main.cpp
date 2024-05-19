#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>

using namespace std;

// Number of page frames in physical memory
const int NUM_FRAMES = 4;
// Size of each page
const int PAGE_SIZE = 1024;
// Frame list in physical memory used for LRU page replacement
list<int> frameList;
// Used to map a process ID to the iterator pointing to its position in the frameList.
unordered_map<int, list<int>::iterator> pageMap;
// Physical memory
vector<char> physicalMemory(NUM_FRAMES * PAGE_SIZE);

/*
 * processId, which identifies the process requesting memory, and
 * size, which specifies the size of memory requested by the process.
 */
int allocateMemory(int processId, int size) {
    if (size > PAGE_SIZE) {
        cerr << "Requested size exceeds page size!" << endl;
        return -1;
    }
    // Find free frames or perform page replacement
    list<int>::iterator it;
    int victimFrame;
    if (frameList.empty()) {
        // No free frames, page replacement
        it = frameList.begin();
        victimFrame = *it;
        frameList.erase(it);
        // TODO: Write the replaced page contents to disk
    } else {
        // Has free frames, use it
        it = frameList.end();
        --it;
        victimFrame = *it;
        frameList.erase(it);
    }
    // Allocate memory and update data structures
    int frameIndex = victimFrame / PAGE_SIZE;
    int offset = victimFrame % PAGE_SIZE;
    physicalMemory[frameIndex * PAGE_SIZE + offset] = static_cast<char>(processId); // Assuming dynamic allocation for memory
    pageMap[processId] = frameList.insert(frameList.end(), victimFrame); // Insert the page into the frame list and update mapping
    return victimFrame;
}

void deallocateMemory(int processId) {
    if (pageMap.find(processId) == pageMap.end()) {
        std::cerr << "Process " << processId << " not found in memory!" << std::endl;
        return;
    }

    // Find and free pages
    list<int>::iterator it = pageMap[processId];
    int frame = *it;
    frameList.erase(it); // Remove a page from the frame list
    pageMap.erase(processId); // Remove a page from the mapping

    // TODO: Write the replaced page contents to disk
    // set the frame to free
    frameList.push_back(frame);
}

int main() {
    // Allocate memory for process 1
    int frame1 = allocateMemory(1, 512);
    if (frame1 != -1) {
        cout << "Allocated frame for process 1: " << frame1 << endl;
    }

    // Allocate memory for process 2
    int frame2 = allocateMemory(2, 768);
    if (frame2 != -1) {
        cout << "Allocated frame for process 2: " << frame2 << endl;
    }

    // Deallocate memory for process 1
    deallocateMemory(1);
    cout << "Deallocated memory for process 1" << endl;

    return 0;
}
