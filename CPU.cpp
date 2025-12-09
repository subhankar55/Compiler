#include "CPU.h"
#include <iostream>
#include <sstream>
#include <stdexcept>


CPU::CPU(int memory_size, int stack_size) : memory(memory_size, 0), stack_base(memory_size - stack_size) {
    reg_A = 0;
    reg_B = 0;
    pc = 0;
    sp = memory_size - 1; 
    zero_flag = false;
    carry_flag = false;
}


void CPU::loadProgram(const std::string& assembly_code) {
    parse(assembly_code);
}


void CPU::run() {
    pc = 0;
    while (pc < instructions.size()) {
        const auto& instr = instructions[pc];
        if (instr.opcode == "hlt") {
            break;
        }
        execute(instr);
    }
}


void CPU::printMemory(int start, int count) {
    std::cout << "--- CPU Memory State ---" << std::endl;
    for (int i = start; i < start + count; ++i) {
        std::cout << "Address [" << i << "]: " << static_cast<int>(memory[i]) << std::endl;
    }
}


void CPU::printState() {
    std::cout << "--- CPU State ---" << std::endl;
    std::cout << "A: " << static_cast<int>(reg_A) << " B: " << static_cast<int>(reg_B) << std::endl;
    std::cout << "PC: " << static_cast<int>(pc) << " SP: " << static_cast<int>(sp) << std::endl;
    std::cout << "Zero: " << zero_flag << " Carry: " << carry_flag << std::endl;
}




uint8_t CPU::getValue(const std::string& arg) {
    if (arg == "A") return reg_A;
    if (arg == "B") return reg_B;
    
    return static_cast<uint8_t>(std::stoi(arg));
}


void CPU::execute(const Instruction& instr) {
    uint8_t next_pc = pc + 1;


   
    if (instr.opcode == "ldi") { 
        uint8_t val = getValue(instr.arg2);
        if (instr.arg1 == "A") reg_A = val;
        else if (instr.arg1 == "B") reg_B = val;
    } else if (instr.opcode == "lda") { 
        reg_A = memory[getValue(instr.arg1)];
    } else if (instr.opcode == "sta") { 
        memory[getValue(instr.arg1)] = reg_A;
    } else if (instr.opcode == "mov") {
        if(instr.arg1 == "B" && instr.arg2 == "A") reg_B = reg_A;
        else if(instr.arg1 == "A" && instr.arg2 == "B") reg_A = reg_B;
    }
    
    else if (instr.opcode == "add") {
        uint16_t result = reg_A + reg_B;
        reg_A = static_cast<uint8_t>(result);
        carry_flag = (result > 255);
        zero_flag = (reg_A == 0);
    } else if (instr.opcode == "sub") {
        uint16_t result = reg_A - reg_B;
        reg_A = static_cast<uint8_t>(result);
        carry_flag = (reg_B > reg_A);
        zero_flag = (reg_A == 0);
    } else if (instr.opcode == "cmp") {
        uint8_t val_a = reg_A;
        uint8_t val_b = reg_B;
        zero_flag = (val_a == val_b);
        carry_flag = (val_b > val_a);
    }
    
    else if (instr.opcode == "jmp") {
        next_pc = labels.at(instr.arg1);
    } else if (instr.opcode == "jne") { 
        if (!zero_flag) {
            next_pc = labels.at(instr.arg1);
        }
    }
    
    else if (instr.opcode == "push") {
        memory[sp] = getValue(instr.arg1);
        sp--;
        if (sp < stack_base) throw std::runtime_error("Stack overflow");
    } else if (instr.opcode == "pop") {
        sp++;
        if (sp >= stack_base + 32) throw std::runtime_error("Stack underflow");
        if (instr.arg1 == "A") reg_A = memory[sp];
        else if (instr.arg1 == "B") reg_B = memory[sp];
    }
   
    pc = next_pc;
}



void CPU::parse(const std::string& assembly_code) {
    std::stringstream ss(assembly_code);
    std::string line;
    uint8_t line_number = 0;
   
   
    while (std::getline(ss, line)) {
        size_t comment_pos = line.find(';');
        if (comment_pos != std::string::npos) line = line.substr(0, comment_pos);
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
   
    ss.clear();
    ss.seekg(0);
    while (std::getline(ss, line)) {
        size_t comment_pos = line.find(';');
        if (comment_pos != std::string::npos) line = line.substr(0, comment_pos);
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        if (line.empty() || line.back() == ':') continue;


        Instruction instr;
        std::stringstream line_ss(line);
        line_ss >> instr.opcode;
        line_ss >> instr.arg1;
        line_ss >> instr.arg2;
        instructions.push_back(instr);
    }
}
