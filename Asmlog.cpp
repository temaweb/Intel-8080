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

#include <cstdint>
#include <iomanip>
#include <iostream>

#include "Cpu.hpp"
#include "Asmlog.hpp"

template<class T>
void Asmlog::print(int width, std::string prefix, T value)
{
    std::cout << std::setfill(delimiter) << prefix
              << std::setfill(filler) << std::setw(width) << std::right << std::hex << unsigned(value);
}

void Asmlog::printDivider(int width)
{
    std::cout << std::setfill(delimiter) << std::setw(width);
}

void Asmlog::log(uint16_t counter, const Cpu * cpu)
{
    auto opcode  = cpu -> opcode;
    auto command = cpu -> commands[opcode];
    
    std::cout << std::uppercase;

    print(4, "0x", counter);
    print(2,  " ", opcode);

    if (command.addrmod == &Cpu::DIR)
    {
        auto lo = cpu -> read(counter + 1);
        auto hi = cpu -> read(counter + 2);

        auto value = cpu -> read((hi << 8) | lo);

        print(2, " ",  lo);
        print(2, " ",  hi);
        print(2, " $", value);

        printDivider(6);
    }
    else if (command.addrmod == &Cpu::IMM)
    {
        auto value = cpu -> read(counter + 1);
        
        print(2, " ", value);
        printDivider(13);
    }
    else
    {
        printDivider(16);
    }

    std::cout << std::right << command.name;

    print(2, " A:", cpu -> registers[cpu -> A]);
    print(2, " B:", cpu -> registers[cpu -> B]);
    print(2, " C:", cpu -> registers[cpu -> C]);
    print(2, " D:", cpu -> registers[cpu -> D]);
    print(2, " E:", cpu -> registers[cpu -> E]);
    print(2, " H:", cpu -> registers[cpu -> H]);
    print(2, " L:", cpu -> registers[cpu -> L]);

    auto printReg = [&](uint8_t index)
    {
        auto address = cpu -> readpair(index);
        auto value   = cpu -> read(address);
        
        print(2, " $", value);
    };
    
    printReg(cpu -> BC);
    printReg(cpu -> DE);
    printReg(cpu -> HL);

    std::cout << "  S="  << unsigned(cpu ->status.GetSign());
    std::cout << "  Z="  << unsigned(cpu ->status.GetZero());
    std::cout << "  AC=" << unsigned(cpu ->status.GetAux());
    std::cout << "  P="  << unsigned(cpu ->status.GetParity());
    std::cout << "  C="  << unsigned(cpu ->status.GetCarry());

    print(4, "  0x", cpu -> stack);
    
    std::cout << std::endl;
}
