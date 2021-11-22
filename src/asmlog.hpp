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

#ifndef ASMLOG_HPP
#define ASMLOG_HPP

#include <cstdint>
#include <string>

class Cpu;

class Asmlog
{
private:
    constexpr static char delimiter = ' ';
    constexpr static char filler    = '0';
    
    template<typename T>
    static void print(int width, std::string prefix, T value);
    
    static void printDivider(int width);
    
public:
    static void log(uint16_t counter, const Cpu * cpu);
};

#endif /* ASMLOG_HPP */
