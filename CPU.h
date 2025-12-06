#ifndef CPU_H
#define CPU_H

#include <vector>
#include <string>
#include <map>
#include <cstdint> // For uint8_t

// Represents a single parsed assembly instruction
struct Instruction {
    std::string opcode;
    std::string arg1;
    std::string arg2;
};

class CPU {
public:
    CPU(int memory_size = 256, int stack_size = 32);
    void loadProgram(const std::string& assembly_code);
    void run();
    void printMemory(int start, int count);
    void printState();

private:
    // CPU Registers and State
    uint8_t reg_A;
    uint8_t reg_B;
    uint8_t pc;         // Program Counter
    uint8_t sp;         // Stack Pointer

    // CPU Flags
    bool zero_flag;
    bool carry_flag;

    // System Memory
    std::vector<uint8_t> memory;
    int stack_base;

    // Parsed Program
    std::vector<Instruction> instructions;
    std::map<std::string, uint8_t> labels;

    // Private methods
    void parse(const std::string& assembly_code);
    uint8_t getValue(const std::string& arg);
    void execute(const Instruction& instr);
};

#endif // CPU_H
