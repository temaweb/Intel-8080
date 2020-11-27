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

#ifndef IO_hpp
#define IO_hpp

#include <iostream>

template<class T>
class IO
{
public:
    
    virtual ~IO() = default;
    
public:
    virtual uint8_t read(T address) const = 0;
    virtual void write(T address, uint8_t data) = 0;
};

template<class T>
class DefaultIO : public IO<T>
{
public:
    virtual uint8_t read(T address) const override final
    {
        return 0x00;
    }
    
    virtual void write(T address, uint8_t data) override final
    {
        
    }
};

#endif /* IO_hpp */
