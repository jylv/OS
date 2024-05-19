/*
 * Simulates basic file system operations such as creating, reading, updating, and deleting files, and listing the files in the file system.
 * It provides error handling for cases where files do not exist or already exist.
 */
#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

class FileSystem {
private:
    //store file names as keys and file contents as values.
    unordered_map<string,string> files;
public:
    bool createFile(const string& filename, const string& content) {
        if(files.find(filename)!=files.end()) {
            cerr<<"File already exists"<<filename<<endl;
            return false;
        }
        files[filename] = content;
        return true;
    }

    bool readFile(const string& filename, string& content) {
        //searches for the specified filename in the files unordered map. The find function returns an iterator to the element if it is found, or end() if the element is not found.
        auto it = files.find(filename);
        if(it == files.end()) {
            cerr<<"File does not exit: "<<filename<<endl;
            return false;
        }
        //assigns the content associated with the filename (stored in the second part of the key-value pair pointed to by the iterator it) to the content string passed by reference.
        content = it->second;
        return true;
    }

    bool writeFile(const string& filename, const string& content) {
        auto it = files.find(filename);
        if(it == files.end()) {
            cerr<<"File does not exit: "<<filename<<endl;
            return false;
        }
        it->second = content;
        return true;
    }

    bool deleteFile(const string& filename) {
        //erases the element with the specified filename from the map files
        //The function returns the number of elements erased (which is either 0 or 1 in the case of an unordered_map).
        if(files.erase(filename)==0) {
            cerr<<"File does not exit: "<<filename<<endl;
            return false;
        }
        return true;
    }

    void listFile() {
        for(auto& pair:files) {
            cout<<"File: "<<pair.first<<endl;
        }
    }
};
int main() {
    FileSystem fs;

    fs.createFile("example.txt","Hello world!");

    string content;
    if(fs.readFile("example.txt",content)) {
        cout<<"File content: "<<content<<endl;
    }

    fs.writeFile("example.txt","Hello, I changed theworld!");

    if(fs.readFile("example.txt",content)) {
        cout<<"Updated file content: "<<content<<endl;
    }

    fs.listFile();

    fs.deleteFile("example.txt");

    if(!fs.readFile("example.txt",content)) {
        cout<<"Fail to read deleted file "<<endl;
    }

    fs.listFile();
    return 0;
}
