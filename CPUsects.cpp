#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <fstream>
#include <sstream>
#include <cmath>
#include "CPUsects.h"
//#include "memory.h"
using namespace CPU;
using namespace std;




vector<string> CPUsects::fetch(string input) {
    string opcode = input.substr(0, 6);

    std::vector<string> pass;
    //R-Type function
    if (!opcode.compare("000000")) {

        //pass = string[6];
        pass.push_back(opcode); // opcode
        pass.push_back(input.substr(6, 5)); // $rs
        pass.push_back(input.substr(11, 5)); // $rt
        pass.push_back(input.substr(16, 5)); // $rd
        pass.push_back(input.substr(22, 5)); //Shamt
        pass.push_back(input.substr(26, 6)); // funct


        string funct = "000000";
        for (int i = 0; i < 6; i++) {
            funct[i] = input[26 + i];
        }
        //Add
        if (!funct.compare("100000")) {
            //add is 16
            type = 16;

        }
        //Sub
        else if (!funct.compare("100010")) {
            //sub is 34
            type = 34;

        }
        //And
        else if (!funct.compare("100100")) {
            //and is 36 
            type = 36;
        }
        //Or
        else if (!funct.compare("100101")) {
            //Or is 37
            type = 37;
        }
        //Slt
        else if (!funct.compare("101010")) {
            //Slt is 42
        }
        //InstructionDecode(pass);
    }
    //Lw
    else if (!opcode.compare("100011")) {
        //lw is 35 in i type
        type = 35; // the types had been the opcode converted to decimal, since 34 was used we incremented it to 35
        pass.push_back(opcode);
        pass.push_back(input.substr(6, 5)); // $RS
        pass.push_back(input.substr(11, 5)); // $RT
        pass.push_back(input.substr(16, 16)); // imm 
        //InstructionDecode(pass);
    }
    //Sw
    else if (!opcode.compare("101011")) {
        //sw is 43
        type = 43;
        pass.push_back(opcode);
        pass.push_back(input.substr(6, 5));
        pass.push_back(input.substr(11, 5));
        pass.push_back(input.substr(16, 16));
        //InstructionDecode(pass);
    }
    //Beq
    else if (!opcode.compare("000100")) {
        //beq is 4
        type = 4;
        pass.push_back(opcode);
        pass.push_back(input.substr(6, 5));
        pass.push_back(input.substr(11, 5));
        pass.push_back(input.substr(16, 16));
        //InstructionDecode(pass);
    }
    //J
    else if (!opcode.compare("000010")) {
        //J  is 2
        type = 2;
        pass.push_back(opcode);
        pass.push_back(input.substr(6, 26));
        //InstructionDecode(pass);
    }
    // addi 
    else if (!opcode.compare("001000")) {
        //addi is 8

         
        type = 8;
        pass.push_back(opcode);
        pass.push_back(input.substr(6, 5));
        pass.push_back(input.substr(11, 5));
        pass.push_back(input.substr(16, 16));
        //InstructionDecode(pass);
    }
    //cout << "pass vector: " << pass[0] << "\n";
    return pass;
}

//register file read
int CPUsects::InstructionDecode(vector<string> ins)
{
    //cout << ins[0];
    //three registers used
    if (type == 16 || type == 34 || type == 36 || type == 37 || type == 42)
    {
        //generally a la + lw has been called before 
        // checks out. 00100 = 4 - right now every register contains the value 0
        //using an arithmetic operation
        //cout << ins[1] << "\n";
        r1 = binToInt(ins[1]);
        r2 = binToInt(ins[2]);
        targetr = binToInt(ins[3]);
        return 1;
    }
    //3 registers  and one immediate
    else if (type == 35 || type == 43 || type == 4 || type == 8)
    {
        //need to implement register use

        r1 = binToInt(ins[1]);
        targetr = binToInt(ins[2]);
        imm = binToInt(ins[3]);
        return 2;
    }
    //no registers only imm
    else if (type == 43)
    {
        //J
        imm = binToInt(ins[1]);
        return 3;
    }

    return 0;
}


//address calculation
string CPUsects::InstructionExectue(vector<string> ins)
{
    if (type == 16) { // add
        registers[targetr] = registers[r1] + registers[r2];
        addCount++;
    }
    else if (type == 34) { // sub
        registers[targetr] = registers[r1] - registers[r2];
        subCount++;
    }
    else if (type == 36) { // and
        registers[targetr] = (registers[r1] & registers[r2]);
        andCount++;
    }
    else if (type == 37) { // or
        registers[targetr] = (registers[r1] | registers[r2]);
        orCount++;
    }
    else if (type == 42) { // slt
        registers[targetr] = (registers[r1] < registers[r2]);
        sltCount++;
    }
    else if (type == 35) { // lw
        MemoryAccess(ins);
        lwCount++;
        return "lw";
    }
    else if (type == 43) { // sw
        MemoryAccess(ins);
        swCount++;
        return "sw";
    }
    else if (type == 4) { // BEQ
        beqCount++;
        if (r1 == targetr) {

            PC += imm;
            return "beq";
        }
    }
    else if (type == 2) { // Jump
        jCount++;
        PC = imm;
        return "j";
    }
    else if (type == 8) { // ADDI
        registers[targetr] = imm + registers[r1];
        addICount++;
        //cout << "val: " << targetr << "\nreg: " << registers[targetr] << "\n";
    }

    return "0";

}

//access memory
string CPUsects::MemoryAccess(vector<string> ins)
{
    if (type == 35)
    {
        //load
        int address = binToInt(ins[2]);
        int value = meme.readMemory(address);
        registers[targetr] = value;
        return intToBin(value, 8);
    }
    else if (type == 43)
    {
        //store
        int address = binToInt(ins[2]);
        int value = binToInt(ins[3]);
        targetr = value;
        meme.writeMemory(address, value);
        return intToBin(value, 8);
    }
    else
    {
        return "not needed";
    }
    return "not implemented";
}

string CPUsects::WriteBack(string ins)
{
    PC += 4; //moves to next instruction

    return "not implemented";
}

int CPUsects::binToInt(string bins) {
    int result = 0;
    int multiplier = 1; // multiplier for each bit position

    // Start from the rightmost (least significant) bit
    for (int i = bins.length() - 1; i >= 0; --i) {
        // If the current bit is '1', add the corresponding value to the result
        if (bins[i] == '1') {
            result += multiplier;
        }
        // Increment the multiplier for the next bit position
        multiplier *= 2;
    }

    return result;
}

string CPUsects::intToBin(int num, int length) {
    if (length <= 0) {
        std::cerr << "Error: Length must be greater than zero" << std::endl;
        return "";
    }

    std::string binary;
    while (num > 0) {
        binary = (num % 2 == 0 ? "0" : "1") + binary;
        num /= 2;
    }

    // Pad with leading zeros to reach desired length
    while (binary.length() < length) {
        binary = "0" + binary;
    }

    return binary;
}