//
//  main.cpp
//  Memory_management
//
//  Created by Angel on 4/25/16.
//  Copyright © 2016 Angel Jimenez. All rights reserved.
//

#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
using namespace std;

struct Process {
    int pid;
    int time_start;
    int time_end;
    int mem_block_count;
    vector<int> mem_blocks;
};
/*
    2 //process id
    0 //arrival_time  2000 //life_time
    2 //number_of_pieces 200 400 //address space blocks, total = 200 + 400 = 600
*/
void readFile(queue<Process> & process_queue);

int main()
{
    string file = "in1.txt";
    int memory_size = 0;
    int page_size= 0;
    int choice;
    queue<Process> process_queue;
    
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
    
    //DO SOMETHING...
    readFile(process_queue);
    //findTimes();
    //printOutput();
    
    return 0;
}

void readFile(queue<Process> & process_queue){
    
    Process process;
    vector<int> mem_blocks;
    int process_count, mem_block;
    
    ifstream input_file;
    input_file.open("in1.txt");
    
    input_file >> process_count;
    
    for(int i = 0; i < process_count; i++){
        input_file >> process.pid;
        input_file >> process.time_start;
        input_file >> process.time_end;
        input_file >> process.mem_block_count;
       
        for(int j = 0; j < process.mem_block_count; j++){
            input_file >> mem_block;
            mem_blocks.push_back(mem_block);
        }
        
        process.mem_blocks = mem_blocks;
        process_queue.push(process);
    }
    
}




























