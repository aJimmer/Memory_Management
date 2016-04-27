#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <algorithm>
using namespace std;

struct Process {
    
    int pid;
    int time_start;
    int time_end;
    int time_life;
    int mem_block_count;
    int total_memory = 0;
    vector<int> mem_blocks;
    bool operator<(const Process& a) const {
        return time_start < a.time_start;
    }
    
    void printProcess() const {
        cout << "Pid: " << pid << endl;
        cout << "Process Time Start: " << time_start << endl;
        cout << "Process Time End: " << time_end << endl;
        cout << "Process Time Life: " << time_life << endl;
        cout << "Memory Block Count: " << mem_block_count << endl;
        cout << "Total Memory: " << total_memory << endl << endl;
    }
};

struct Memory_Block {
    int start;
    int end;
    int process_number;
    int page_number;
    bool available;
};

void readFile(priority_queue<Process> & process_queue, vector<int> & events);
void buildMemoryMap(vector<Memory_Block> & memory_block, int memory_size, int page_size);
bool checkAvailableMemory(vector<Memory_Block> & memory_block, Process process);
void addProcessToMem(vector<Memory_Block> & memory_block, Process process);
//void loadInMemory(priority_queue<Process> & process_queue, vector<Memory_Block> & memory_block, process);

int main()
{
    string file = "in1.txt";
    int memory_size = 0, page_size = 0, choice;
    priority_queue<Process> process_queue;
    vector<Memory_Block> memory_block;
    vector<int> events;
    vector<Process> ready_queue;
    Process process;
    
    cout << "Enter the memory size(Kbytes): ";
    cin >> memory_size;
    cout << "Enter the page size (1:100, 2:200, 3:400): ";
    cin >> choice;
    
    switch(toupper(choice)){
        case 1:
            page_size = 100;
            break;
        case 2:
            page_size = 200;
            break;
        case 3:
            page_size = 400;
            break;
    }
    
    readFile(process_queue, events);
    buildMemoryMap(memory_block, memory_size, page_size);
    
    int e_time = 0;
    
    for(int time = 0; time < events.back(); time++){
        while (events[e_time] == time){
            process = process_queue.top();
            // process.printProcess();
            
            if ((process.time_start == time) && checkAvailableMemory(memory_block, process)){
                addProcessToMem(memory_block, process);
            }
            else if(process.time_end == time){
                cout << "Try to remove from memory!" << endl;
            }
            else{
                ready_queue.push_back(process);
            }
            process_queue.pop();
            e_time++;
        }
    }
    
    cout << "Ready Queue: ";
    for(int i = 0; i < ready_queue.size(); i++){
        cout << ready_queue[i].pid << " ";
    }
    //loadInMemory(mem_block, process_queue);
    //findTimes();
    //printOutput();
    
    return 0;
}

void readFile(priority_queue<Process> & process_queue, vector<int> & events){
    
    Process process;
    vector<int> mem_blocks;
    int process_count, mem_block;
    
    ifstream input_file;
    input_file.open("in1.txt");
    
    input_file >> process_count;
    
    //Read in text file and buid a priority queue of processes
    for(int i = 0; i < process_count; i++){
        input_file >> process.pid;
        
        input_file >> process.time_start;
        input_file >> process.time_life;
        input_file >> process.mem_block_count;
        
        for(int j = 0; j < process.mem_block_count; j++){
            input_file >> mem_block;
            //cout << mem_block;
            process.total_memory += mem_block;
            mem_blocks.push_back(mem_block);
        }
        
        process.mem_blocks = mem_blocks;
        process.time_end = process.time_start + process.time_life;
        
        //initialize events of processes
        events.push_back(process.time_start);
        events.push_back(process.time_end);
        
        process_queue.push(process);
        process.total_memory = 0;
    }
    
    sort(events.begin(), events.end());
    
    
    /* Check process events*/
    for(int k = 0; k < events.size(); k++){
        cout << events[k] << " ";
    }
    
    
    for(int k = 0; k < process_count; k++){
        process_queue.top().printProcess();
        process_queue.pop();
    }
}

void buildMemoryMap(vector<Memory_Block> & memory_blocks, int memory_size, int page_size){
    
    Memory_Block mem_block;
    
   	for (int i = 0; i < (memory_size / page_size); i++)
    {
        mem_block.start = i * page_size;
        cout << "Block begins: " << mem_block.start << endl;
        mem_block.end = ((i + 1) * page_size) - 1;
        cout << "Block ends: " << mem_block.end << endl << endl;
        mem_block.process_number = -1;
        mem_block.page_number = -1;
        mem_block.available = true;
        memory_blocks.push_back(mem_block);
    }
}

bool checkAvailableMemory(vector<Memory_Block> & memory_block, Process process){
    bool available = false;
    
    
    return available;
}

void addProcessToMem(vector<Memory_Block> & memory_block, Process process){
    
}









