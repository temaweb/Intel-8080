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

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>

struct Command
{
    // Operation name
    std::string name;
    
    // Operation cycles
    uint8_t cycles = 0x00;

    uint8_t (Cpu::*operate) (void) = nullptr;
    void    (Cpu::*addrmod) (void) = nullptr;
    
    bool isImplied();
    bool isIndirect();
};

#endif /* Command_h */
