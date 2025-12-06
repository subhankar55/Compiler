#include "Simulator.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

Simulator::Simulator(int memory_size) : memory(memory_size, 0), reg_A(0), reg_B(0), pc(0), zero_flag(false) {}

void Simulator::loadProgram(const std::string& assembly_code) {
    parse(assembly_code);
}

// Main execution loop
void Simulator::run() {
    pc = 0;
    while (pc < instructions.size()) {
        const auto& instr = instructions[pc];

        if (instr.opcode == "MOV") {
            int value = getValue(instr.arg2);
            if (instr.arg1 == "A") {
                reg_A = value;
            } else if (instr.arg1 == "B") {
                reg_B = value;
            } else { // It's a memory address like [0]
                int address = std::stoi(instr.arg1.substr(1, instr.arg1.length() - 2));
                memory[address] = value;
            }
        } else if (instr.opcode == "ADD") {
            reg_A += getValue(instr.arg2);
        } else if (instr.opcode == "SUB") {
            reg_A -= getValue(instr.arg2);
        } else if (instr.opcode == "CMP") {
            int val1 = getValue(instr.arg1);
            int val2 = getValue(instr.arg2);
            zero_flag = (val1 == val2);
        } else if (instr.opcode == "JNZ") {
            if (!zero_flag) {
                pc = labels.at(instr.arg1);
                continue; // Skip the PC increment
            }
        } else if (instr.opcode == "HALT") {
            break;
        }

        pc++;
    }
}

void Simulator::printMemory(int count) {
    std::cout << "--- CPU Memory State ---" << std::endl;
    for (int i = 0; i < count; ++i) {
        std::cout << "Address [" << i << "]: " << memory[i] << std::endl;
    }
}

// Helper to resolve an argument to its integer value
int Simulator::getValue(const std::string& arg) {
    if (arg == "A") return reg_A;
    if (arg == "B") return reg_B;
    if (arg[0] == '[') { // Memory access
        int address = std::stoi(arg.substr(1, arg.length() - 2));
        return memory[address];
    }
    // Otherwise, it's a literal number
    return std::stoi(arg);
}


// --- Assembly Parser ---

void Simulator::parse(const std::string& assembly_code) {
    std::stringstream ss(assembly_code);
    std::string line;
    int line_number = 0;
    
    // First pass: identify labels
    while (std::getline(ss, line)) {
        // Remove comments
        size_t comment_pos = line.find(';');
        if (comment_pos != std::string::npos) {
            line = line.substr(0, comment_pos);
        }
        // Trim whitespace (simple version)
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty()) continue;

        if (line.back() == ':') {
            std::string label = line.substr(0, line.length() - 1);
            labels[label] = line_number;
        } else {
            line_number++;
        }
    }
    
    // Second pass: parse instructions
    ss.clear();
    ss.seekg(0);
    while (std::getline(ss, line)) {
        // Skip comments and labels
        size_t comment_pos = line.find(';');
        if (comment_pos != std::string::npos) line = line.substr(0, comment_pos);
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        if (line.empty() || line.back() == ':') continue;

        instructions.push_back(parseLine(line));
    }
}

Instruction Simulator::parseLine(const std::string& line) {
    Instruction instr;
    std::stringstream line_ss(line);
    line_ss >> instr.opcode;

    std::string arg;
    if (line_ss >> arg) {
        // Remove comma if it exists (e.g., "A, B")
        if (arg.back() == ',') arg.pop_back();
        instr.arg1 = arg;
    }
    if (line_ss >> arg) {
        instr.arg2 = arg;
    }
    return instr;
}
