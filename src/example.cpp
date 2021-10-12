/*
 * This file is part of the 8080 distribution (https://github.com/temaweb/8080).
 * Copyright (c) 2020 Artem Okonechnikov.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <fstream>
#include <array>

#include "IO.hpp"
#include "cpu.hpp"

// Test starting at
static const uint16_t offset = 0x0100;

class Bus : public IO<uint16_t>
{
private:
    // 64 KB
    // This is max address space for Intel 8080
    using ram = std::array<uint8_t, 64 * 1024>;

    // Memory array
    ram memory {};
    
    // Bus test initializer
    friend void load(std::string path, const std::shared_ptr<Bus> & bus);
    
public:
    Bus()
    {
        write(0x0000, 0xD3); // 0000: OUT 00
        write(0x0001, 0x00);
        
        write(0x0005, 0xD3); // 0005: OUT 01
        write(0x0006, 0x01);
        
        write(0x0007, 0xC9); // 0007: RET
    }
    
    virtual uint8_t read(uint16_t address) const override {
        return memory[address];
    }
    
    virtual void write(uint16_t address, uint8_t data) override {
        memory[address] = data;
    }
    
private:
    ram::iterator begin()
    {
        return memory.begin() + ::offset;
    }
    
    ram::size_type size()
    {
        return memory.size();
    }
};


void load(std::string path, const std::shared_ptr<Bus> & bus)
{
    std::ifstream file(path, std::ios::in | std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "File not found " << path;
        exit(EXIT_FAILURE);
        
        return;
    }
    
    file.read((std::ifstream::char_type *) bus -> begin(), (std::streamsize) bus -> size());
    file.close();
}

// Run test
void execute(std::string exerciser)
{
    auto bus = std::make_shared<Bus>();
    auto cpu = std::make_unique<Cpu>();
    
    // Communication betweet RAM and CPU
    cpu -> connect(bus);
    
    // Set program counter to begin test (0x0100)
    cpu -> setCounter(offset);
    
    load(exerciser, bus);

    while (cpu -> getCounter() > 0)
    {
        // Result will be recieve in OUT operation
        // See Cpu::OUT method for more information
        
        cpu -> clock();
    }
}

int main(int argc, const char * argv[])
{
#ifndef LOGTEST
    std::cout << "Recomplile with -DLOGTEST key for run tests";
    std::cout << std::endl;
    
    return 0;
#endif
    
    const std::string exercisers[4]
    {
        "../asm/CPUTEST.com",
        "../asm/8080.com",
        "../asm/8080PRE.com",
        "../asm/8080EXM.com"
    };

    for (auto exerciser : exercisers)
    {
        std::cout << "\033[1;31m";
        std::cout << exerciser;
        std::cout << "\033[0m\n";
        
        execute(exerciser);
        
        std::cout << std::endl;
        std::cout << std::endl;
    }

    return 0;
}
