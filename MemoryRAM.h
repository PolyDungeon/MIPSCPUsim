#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <fstream>
#include <sstream>
#include <unordered_map>

using namespace std;

namespace CPU {
#ifndef MemoryRAM_H
#define MemoryRAM_H
    class MemoryRAM
    {
    private:
        std::vector<int> memory; // Vector to represent memory contents
        std::unordered_map<int, int> cache; // Cache to store memory values for faster access
    public:
        MemoryRAM() {
            // Initialize memory with some default values
            memory = std::vector<int>(0x1000, 0); // Assuming a memory size of 1000 locations, initialized to 0
        }

        int readMemory(int address);

        void writeMemory(int address, int data);

        void clearCache();


    };

#endif
}