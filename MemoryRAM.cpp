#include "MemoryRAM.h"
//using namespace CPU;
using namespace std;
using namespace CPU;

int MemoryRAM::readMemory(int address)
{
  // Check if the data is available in the cache
  if (cache.find(address) != cache.end()) {
      //std::cout << "Cache hit!" << std::endl;
      return cache[address];
  }

  if (address < 0x0 || address >= memory.size()) {
      std::cerr << "Error: Invalid memory address" << std::endl;
      return 0; // Return default value if address is out of bounds
  }

  // Read from memory and update cache
  int data = memory[address];
  cache[address] = data;
  return data;
}

void MemoryRAM::writeMemory(int address, int data) {
    if (address < 0x0 || address >= memory.size()) {
        std::cerr << "Error: Invalid memory address" << std::endl;
        return; // Don't write if address is out of bounds
    }

    // Write to memory and update cache
    memory[address] = data;
    cache[address] = data;
}

// Method to clear the cache
void MemoryRAM::clearCache() {
    cache.clear();
}