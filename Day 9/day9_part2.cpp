#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm> // find for vectors
#include <utility> // make pair

const int EMPTY_MEMORY_BLOCK_SIGNIFIER = -1;

long long computeChecksum (const std::vector<int>& v) {
    long long check_sum = 0;

    for(int i = 0; i < v.size(); i++) {
        if(v[i] != EMPTY_MEMORY_BLOCK_SIGNIFIER)
            check_sum += v[i] * i;
    }
    return check_sum;
}

void printBlockRep(std::vector<int>& block_rep) {
    for(const int& n : block_rep) {
        if(n >= 0)
            std::cout << '|' << n;
        else if(n == EMPTY_MEMORY_BLOCK_SIGNIFIER)
            std::cout << '|' << '.';
    }
    std::cout << std::endl;
}

// Function which appends to block_rep num copies of char c to block_rep based on whether it's empty blocks or occupied by files
void appendBlocks(std::vector<int>& block_rep, int id_num, int num_memory_blocks) {
    std::vector<int> v(num_memory_blocks, id_num);
    block_rep.insert(block_rep.end(), v.begin(), v.end()); // append v to block_rep
}

// Function that 1) builds up a memory block representation of the diskmap in block_rep, 
// 2) stores the starting index and size of every file in files, 3) stores the starting index and size of every empty memory block in empty_memory_blocks
void extractDiskMapInfo(const std::string& disk_map, std::vector<int>& block_rep, std::vector<std::pair<int,int>>& files, 
                        std::vector<std::pair<int,int>>& empty_memory_blocks) {
    int id_number = 0;

    for(int i = 0; i < disk_map.size(); i++) {

        int memory_block_size = disk_map[i] - '0'; // - '0' converts char into an integer

        if(i % 2 == 0) { // if the disk map number has an even index, the number denotes file memory
            appendBlocks(block_rep, id_number, memory_block_size);        
            files.push_back(std::make_pair(block_rep.size() - memory_block_size, memory_block_size));
            id_number++;
        } else { // if the disk map number has an odd index, the number denotes empty space
            appendBlocks(block_rep, EMPTY_MEMORY_BLOCK_SIGNIFIER, memory_block_size);
            empty_memory_blocks.push_back(std::make_pair(block_rep.size() - memory_block_size, memory_block_size));
        }
    }
}

void defragment(std::vector<int>& block_rep, std::vector<std::pair<int,int>>& files, std::vector<std::pair<int,int>>& empty_memory_blocks) {
    // how we select the next file to look at based on fileID. start from largest file ID and go backwards
    auto files_itr = files.end() - 1;

    while(files_itr != files.begin()) {
        
        // search all the empty memory blocks, starting from the leftmost
        for(auto& e : empty_memory_blocks) {
            // 1st condition: if the size of the empty block is greater than or equal to size of the file.
            // 2nd condition: make sure the starting index of the empty memory is less than that of the file we're trying to move.
            //                in other words, make sure we're not moving the file to the right
            if(e.second >= files_itr->second && e.first < files_itr->first) { 
                for(int i = 0; i < files_itr->second; i++) {
                    block_rep[e.first + i] = files_itr - files.begin(); // swap empty memory block with file memory block
                    block_rep[files_itr->first + i] = -1;
                }
                // update starting index and size of empty space now that we filled something
                e.second -= files_itr->second;
                e.first += files_itr->second;
                break;
            }
        }
        files_itr--;
    }
}

int main() {

    std::ifstream my_file;
    my_file.open("input.txt");

    std::string disk_map;

    // each element represents a block of memory (either filled or used) and is either
    //               1) the integer -1, which represents an empty block of memory, or 
    //               2) a non-negative integer that denotes the file ID associated with the in-use memory block
    std::vector<int> block_rep; 
    
    // .first gives index of where a file (whose ID is given by the pair's vector index) begins
    // .second gives the size of the file
    std::vector<std::pair<int, int>> files;

    // .first gives the index of where the empty memory block begins, .second gives the size of the memory block
    std::vector<std::pair<int,int>> empty_memory_blocks;

    while(my_file.is_open()) {
        getline(my_file, disk_map);

        extractDiskMapInfo(disk_map, block_rep, files, empty_memory_blocks);

        defragment(block_rep, files, empty_memory_blocks);

        std::cout << "block_rep size is: " << block_rep.size() << std::endl;

        std::cout << "final checksum is: " << computeChecksum(block_rep) << std::endl;

        my_file.close();
    }
    return 0;
}