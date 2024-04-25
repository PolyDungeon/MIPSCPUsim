#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "MemoryRAM.h"
#include <string>
#include <vector>
using namespace std;

namespace CPU{
#ifndef CPUsects_H
#define CPUsects_H
  class CPUsects
  {
  public:
      CPU::MemoryRAM meme;
      int r1;
      int r2;
      int targetr;
      int type;
      int imm = -1; // value that holds imm, could just asign to r2
      int PC = 0;
      int addCount = 0;
      int addICount = 0;
      int subCount = 0;
      int orCount = 0;
      int andCount = 0;
      int sltCount = 0;
      int lwCount = 0;
      int swCount = 0;
      int jCount;
      int beqCount = 0;
      vector<int> registers;

      CPUsects()
      {
          r1 = -1;
          r2 = -1;
          targetr = -1;
          type = -1;
          //programCount = 0;
          imm = -1; // value that holds imm, could just asign to r2
          registers = vector<int>(32, 0);
      }

      vector<string> fetch(string input);

      //register file read
      int InstructionDecode(vector<string> ins);
      //address calculation
      string InstructionExectue(vector<string> ins);

      //access memory
      string MemoryAccess(vector<string> ins);

      string WriteBack(string ins);
  
      int binToInt(string bins);
      string intToBin(int num, int length);
  };
#endif
  
}
