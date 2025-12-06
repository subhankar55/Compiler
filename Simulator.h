#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>
#include <string>
#include <map>

// Represents a single parsed assembly instruction
struct Instruction {
    std::string opcode;
    std::string arg1;
    std::string arg2;
};

class Simulator {
public:
    Simulator(int memory_size = 256);
    void loadProgram(const std::string& assembly_code);
    void run();
    void printMemory(int count);

private:
    // CPU State
    int reg_A;
    int reg_B;
    int pc; // Program Counter
    bool zero_flag; // For CMP and JNZ

    std::vector<int> memory;
    std::vector<Instruction> instructions;
    std::map<std::string, int> labels;

    void parse(const std::string& assembly_code);
    Instruction parseLine(const std::string& line);
    int getValue(const std::string& arg);
};

#endif // SIMULATOR_H
