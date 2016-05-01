#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <algorithm>
using namespace std;

class Process
{
public:
    int pid;
    int time_start;
    int time_end;
    int time_life;
    int mem_block_count;
    int total_memory = 0;
    vector<int> mem_blocks;
    
    bool operator < (const Process & a) const
    {
        return time_start > a.time_start;
    }
    
    void printProcess() const
    {
        cout << "\nPid: " << pid << endl;
        cout << "Process Time Start: " << time_start << endl;
        cout << "Process Time End: " << time_end << endl;
        cout << "Process Time Life: " << time_life << endl;
        cout << "Memory Block Count: " << mem_block_count << endl;
        cout << "Total Memory: " << total_memory << endl << endl;
    }
};

bool compareEndTime(const Process & a, const Process & b)
{
    // smallest end time comes first
    return a.time_end > b.time_end;
}


struct Memory_Block
{
    int start;
    int end;
    int process_number;
    int page_number;
    int total_size;
    int release_time;
    bool available;
};

void readFile(vector<Process> & process_queue, vector<int> & events);
void buildMemoryMap(vector<Memory_Block> & memory_block, int memory_size, int page_size);
bool checkAvailableMemory(vector<Memory_Block> & memory_block, Process process, int page_size, vector<int> & pages);
bool checkForContMemory(vector<Memory_Block> & memory_block, Process process, vector<int> &pages, int & page_number);
void addProcessToMem(vector<Memory_Block> & memory_block, Process process, vector<int> &pages);

int main()
{
    string file = "in1.txt";
    int memory_size = 0, page_size = 0, choice;
    vector<Process> process_queue;
    vector<Memory_Block> memory_block;
    vector<int> events;
    vector<int> pages;
    vector<Process> ready_queue;
    Process process;
    
    cout << "Enter the memory size(Kbytes): ";
    cin >> memory_size;
    cout << "Enter the page size (1:100, 2:200, 3:400): ";
    cin >> choice;
    
    switch (choice)
    {
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
    int size_of_process_queue = 0;
    bool end_of_processes = false;
    vector<Process> in_memory;
    
    for (int time = 0; time < events.back() + 1; time++)
    {
        //cout << "time in miliseconds: " << time << endl;
        while (events[e_time] == time)
        {
            //get the first process in the queue
            if(process_queue.size() < 1)
            {
                end_of_processes = true;
            }
            
            if(end_of_processes == false)
            {
                process_queue.end();
                size_of_process_queue = (int)process_queue.size() - 1;
                
                process.pid = process_queue[size_of_process_queue].pid;
                process.time_start = process_queue[size_of_process_queue].time_start;
                process.time_end = process_queue[size_of_process_queue].time_end;
                process.time_life = process_queue[size_of_process_queue].time_life;
                process.total_memory = process_queue[size_of_process_queue].total_memory;
            }
            
            cout << "\nTime (ms): " << time << endl;
            
            //release finished processes
            for(int i = 0; i < memory_block.size(); i++)
            {
                if(memory_block[i].release_time == time)
                {
                    memory_block[i].available = true;
                }
            }
            
            if (in_memory.size() > 0 && in_memory.back().time_end == time)
            {
                while(in_memory.back().time_end == time)
                {
                    cout << "\nRemoving Process: " << in_memory.back().pid << endl;
                    in_memory.back().printProcess();
                    in_memory.pop_back();
                    cout << endl;
                }
                
            }
            
            if ((end_of_processes == false) && (process.time_start == time) && checkAvailableMemory(memory_block, process, page_size, pages))
            {
                
                //add processes in ready queue first
                
                in_memory.push_back(process);
                sort(in_memory.begin(), in_memory.end(), compareEndTime);
                addProcessToMem(memory_block, process, pages);
                cout << "\nAdded process to memory: \n";
                process.printProcess();
                process_queue.pop_back();
                cout << endl;
            }
            
            else if((end_of_processes == false) && (process.time_start == time))
            {
                cout << "Add this process to ready queue.\n";
                ready_queue.push_back(process);
                ready_queue.back().printProcess();
                process_queue.pop_back();
                
                ready_queue.back().time_start++;
                ready_queue.back().time_end++;
                
                cout << endl;
            }
            else
            {
                e_time++;
            }
        }
        
        while (ready_queue.size() > 0 && ((ready_queue.back().time_start == time)||(ready_queue.back().time_end == time)))
        {
            if(checkAvailableMemory(memory_block, ready_queue.back(), page_size, pages))
            {
                
                in_memory.push_back(ready_queue.back());
                sort(in_memory.begin(), in_memory.end(), compareEndTime);
                addProcessToMem(memory_block,ready_queue.back(), pages);
                cout << "\nAdded process to memory: \n";
                ready_queue.back().printProcess();
                ready_queue.pop_back();
            }
            
            else
            {
                ready_queue.back().time_start++;
                ready_queue.back().time_end++;
            }
        }
        
        if(in_memory.back().time_end == time)
        {
            cout << "\nRemoving Process: " << in_memory.back().pid << endl;
            in_memory.back().printProcess();
            in_memory.pop_back();
            cout << endl;
        }
        
    }
    
    /*cout << "Ready Queue: ";
     for (int i = 0; i < ready_queue.size(); i++) {
     cout << ready_queue[i].pid << " ";
     }*/
    
    //system("pause");
    return 0;
}

void readFile(vector<Process> & process_queue, vector<int> & events) {
    
    Process process;
    vector<int> mem_blocks;
    int process_count, mem_block;
    
    ifstream input_file;
    input_file.open("in1.txt");
    
    input_file >> process_count;
    
    //Read in text file and buid a priority queue of processes
    for (int i = 0; i < process_count; i++)
        
    {
        input_file >> process.pid;
        
        input_file >> process.time_start;
        input_file >> process.time_life;
        input_file >> process.mem_block_count;
        
        for (int j = 0; j < process.mem_block_count; j++)
        {
            input_file >> mem_block;
            process.total_memory += mem_block;
            mem_blocks.push_back(mem_block);
        }
        
        process.mem_blocks = mem_blocks;
        process.time_end = process.time_start + process.time_life;
        
        //initialize events of processes
        events.push_back(process.time_start);
        events.push_back(process.time_end);
        process_queue.push_back(process);
        process.total_memory = 0;
    }
    
    sort(process_queue.begin(), process_queue.end());
    sort(events.begin(), events.end());
    
    
    /*//Check process events
     for (int k = 0; k < events.size(); k++) {
     cout << events[k] << " ";
     }
     cout << endl;
     */
    // print the queue
    /*for (int k = 0; k < process_count; k++) {
     cout << k << ": ";
     process_queue[k].printProcess();
     }*/
}

void buildMemoryMap(vector<Memory_Block> & memory_blocks, int memory_size, int page_size) {
    
    Memory_Block mem_block;
    
    for (int i = 0; i < (memory_size / page_size); i++)
    {
        mem_block.start = i * page_size;
        mem_block.end = ((i + 1) * page_size) - 1;
        mem_block.process_number = -1;
        mem_block.page_number = i;
        mem_block.available = true;
        mem_block.total_size = (mem_block.end - mem_block.start) + 1;
        memory_blocks.push_back(mem_block);
    }
}

bool checkAvailableMemory(vector<Memory_Block> & memory_blocks, Process process, int page_size, vector<int> & pages) {
    
    bool available = false;
    int page_number = 0;
    int pages_size = (int)pages.size();
    
    while ((page_number < memory_blocks.size()) && (available == false))
    {
        
        if ((memory_blocks[page_number].available) && (process.total_memory <= memory_blocks[page_number].total_size))
        {
            
            for(int i = 0; i < pages_size; i++)
            {
                pages.pop_back();
            }
            
            pages.push_back(page_number);
            return available = true;
        }
        
        else if ((memory_blocks[page_number].available) && (process.total_memory > memory_blocks[page_number].total_size))
        {
            available = checkForContMemory(memory_blocks, process, pages, page_number);
            cout << "Is there avaliable contiguous memory? True or false?: " << available << endl;
        }
        page_number++;
    }
    return available;
}

void addProcessToMem(vector<Memory_Block> & memory_block, Process process, vector<int> & pages)
{
    for(int i = 0; i < pages.size(); i++)
    {
        for(int k = 0; k < memory_block.size(); k++)
        {
            if(pages[i] == memory_block[k].page_number)
            {
                memory_block[k].process_number = process.pid;
                memory_block[k].release_time = process.time_end;
                memory_block[k].available = false;
                cout << "Process " << process.pid << " was allocated to page: " << memory_block[k].page_number << endl;
            }
        }
    }
}

bool checkForContMemory(vector<Memory_Block> & memory_block, Process process, vector<int> & pages, int & page_number){
    
    bool available = false;
    int needed_memory = process.total_memory;
    //clear pages vector
    int pages_size = (int)pages.size();
    for(int i = 0; i < pages_size; i++)
    {
        pages.pop_back();
    }
    
    if(memory_block[page_number].available)
    {
        while ((memory_block[page_number].available) && (needed_memory > memory_block[page_number].total_size) && (page_number < memory_block.size()))
        {
            
            cout << "\nPage: " << page_number << " \nPage Size: " << memory_block[page_number].total_size
            << "\nNeeded Memory for process " << process.pid << ": " << needed_memory << "\nAvailabe: " << memory_block[page_number].available << endl;
            
            needed_memory -= memory_block[page_number].total_size;
            pages.push_back(memory_block[page_number].page_number);
            //memory_block[page_number].available = false;
            page_number++;
        }
        
        if((needed_memory <= memory_block[page_number].total_size) && (page_number < memory_block.size()))
        {
            cout << "\nThis page is the last page to allocate! \n";
            cout << "Page: " << page_number << " \nPage Size: " << memory_block[page_number].total_size
            << "\nNeeded Memory for process " << process.pid << ": " << needed_memory << "\nAvailabe: " << memory_block[page_number].available << endl;
            pages.push_back(memory_block[page_number].page_number);
            //memory_block[page_number].available = false;
            available = true;
        }
    }
    return available;
}






