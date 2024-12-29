#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm> // find for vectors

const int EMPTY_MEMORY_BLOCK_SIGNIFIER = -1;

long long computeChecksum (const std::vector<int>& v) {
    long long check_sum = 0;

    for(int i = 0; i < v.size(); i++) {
        if(v[i] != EMPTY_MEMORY_BLOCK_SIGNIFIER)
            check_sum += v[i] * i;
    }
    return check_sum;
}

// Function that visualizes the memory block representation of the diskmap
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

void convertDiskMapToBlockForm(const std::string& disk_map, std::vector<int>& block_rep, std::vector<int>& file_sizes) {
    int id_number = 0;

    for(int i = 0; i < disk_map.size(); i++) {

        int number_of_memory_blocks = disk_map[i] - '0'; // - '0' converts char into an integer

        if(i % 2 == 0) { // if the disk map number has an even index, the number denotes file memory
            appendBlocks(block_rep, id_number, number_of_memory_blocks);        
            file_sizes.push_back(number_of_memory_blocks);
            id_number++;
        } else // if the disk map number has an odd index, the number denotes empty space
            appendBlocks(block_rep, EMPTY_MEMORY_BLOCK_SIGNIFIER, number_of_memory_blocks);  
    }
}

void defragment(std::vector<int>& file_sizes, std::vector<int>& block_rep) {

    // how we select the next file to look at based on fileID. start from largest file ID and go backwards
    std::vector<int>::iterator file_sizes_itr = file_sizes.end() - 1;

    while(file_sizes_itr != file_sizes.begin()) {

        // forward iterator to find first occurrence of unused memory space
        std::vector<int>::iterator fwd_itr = find(block_rep.begin(), block_rep.end(), EMPTY_MEMORY_BLOCK_SIGNIFIER);
        
        // reverse iterator to find last occurrence of a memory block with the largest file ID
        int key = file_sizes_itr - file_sizes.begin(); // key is equal to the file ID we want to use to defragment
        auto bwd_itr = find(block_rep.rbegin(), block_rep.rend(), key);
        
        // can't compare forward and reverse iterators directly
        bool memory_fragmented = fwd_itr < bwd_itr.base() - 1;

        // loop through *file_sizes_itr number of times; *files_sizes_itr equals the number of memory blocks occupied
        // by the file with file ID we are currently trying to defragment
        for(int i = 0; i < *file_sizes_itr && memory_fragmented; i++) {
            // perform swap of rightmost block of data with leftmost block of free memory
            int temp = *fwd_itr;
            *fwd_itr = *bwd_itr;
            *bwd_itr = temp;

            // update forward iterator
            fwd_itr = find(block_rep.begin(), block_rep.end(), EMPTY_MEMORY_BLOCK_SIGNIFIER);

            // update backward iterator
            bwd_itr = find(block_rep.rbegin(), block_rep.rend(), key);

            // update memory_fragmented indicator
            memory_fragmented = fwd_itr < bwd_itr.base() - 1;
        }
        file_sizes_itr--;
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
    
    // keeps track of the size of each file. vector indices represent file ID numbers, elements are file sizes
    std::vector<int> file_sizes;

    while(my_file.is_open()) {
        getline(my_file, disk_map);

        convertDiskMapToBlockForm(disk_map, block_rep, file_sizes);

        defragment(file_sizes, block_rep);

        //printBlockRep(block_rep);
        std::cout << "block_rep size is: " << block_rep.size() << std::endl;

        std::cout << "final checksum is: " << computeChecksum(block_rep) << std::endl;

        my_file.close();
    }
    return 0;
}