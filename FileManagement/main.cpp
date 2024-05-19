/*
 * This code demonstrates a simple file management system with concurrent access using multithreading
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <string>

using namespace std;

class FileManager {
private:
    // ensure that file operations are thread-safe
    mutex mtx;
public:
    FileManager(){}
    // Creates a new file with the given filename and content.
    bool createFile(const string& filename, const string& content) {
        // Locks the mutex mtx using a lock_guard, ensuring that only one thread can execute the critical section of the code at a time.
        lock_guard<mutex> lock(mtx);
        // Creates an output file stream (ofstream) object named file with the specified filename.
        ofstream file(filename);
        if(file.is_open()) {
            //Writes the content (passed as a parameter) to the file
            file<<content;
            file.close();
            return true;
        }
        return false;
    }
    // Deletes the file with the given filename.
    bool deleteFile(const string& filename) {
        // Locks the mutex mtx using a lock_guard,
        lock_guard<mutex> lock(mtx);
        // Deletes the file specified by its filename. return 0 if successfully delete
        return remove(filename.c_str())==0;
    }
    // Reads and returns the content of the file with the given filename.
    string queryFile(const string& filename) {
        lock_guard<mutex> lock(mtx);
        // Creates an input file stream (ifstream) object named file with the specified filename. The file is opened for reading.
        ifstream file(filename);
        /*
         * Reads the content of the file into a string named content.
         * Takes two iterators, indicating the beginning and end of the content to be copied
         * istreambuf_iterator<char>(file) creates an iterator that reads characters from the file, and istreambuf_iterator<char>() creates an end-of-stream iterator.
         */
        string content((istreambuf_iterator<char>(file)),istreambuf_iterator<char>());
        return content;
    }
};


int main() {
    FileManager fileManeger;
    fileManeger.createFile("test.txt","Helo world!");
    vector<thread> threads;
    // Iterates five times to create five threads.
    for(int i=0;i<5;i++) {
        // Creates a new thread and adds it to the threads vector.
        threads.emplace_back([&fileManeger]() {
            // the thread accesses the content of the file "test.txt" using the queryFile method of the FileManager instance and prints it to the console.
            string content = fileManeger.queryFile(("test.txt"));
            cout << "Thread accessed file with content " << content << endl;
        });
    }
    // Waits for all threads in the threads vector to finish executing by calling the join method on each thread.
    for(auto& thread:threads) {
        thread.join();
    }
    fileManeger.deleteFile("test.txt");
    return 0;
}
