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

#ifndef Status_hpp
#define Status_hpp

#include <cstdint>

class Status
{
private:
    
    // Default status
    static const uint8_t defaultState = 0x02;
    
    // CPU status flags
    // -----------------------------------
    // HI  7 - (S)  - Sign flag
    //     6 - (Z)  - Zero flag
    //     5 - (0)  - None / Always zero
    //     4 - (AC) - Auxiliary carry flag
    //     3 - (0)  - None / Always zero
    //     2 - (P)  - Parity flag
    //     1 - (1)  - None / Always set
    // LO  0 - (C)  - Carry flag
    
    uint8_t status = defaultState;
    
    enum Flags
    {
        S  = (1 << 7),
        Z  = (1 << 6),
        AC = (1 << 4),
        P  = (1 << 2),
        C  = (1 << 0)
    };

    void SetFlag(Flags flag, bool value);
  
public:
    
    Status& operator=(const uint8_t & status);
    operator uint8_t ();
    
public:

    void Reset();
    void InvertCarry();
    
    void SetSign   (bool flag);
    void SetSign   (uint16_t value);
    void SetZero   (bool flag);
    void SetZero   (uint16_t value);
    void SetAux    (bool flag);
    void SetAux    (uint16_t value);
    void SetParity (bool flag);
    void SetParity (uint16_t value);
    void SetCarry  (bool flag);
    void SetCarry  (uint16_t value);
    
    void SetAllFlags (uint16_t value);
    void SetAuxFlags (uint16_t value);
    void SetDecFlags (uint16_t value);
    
public:
    
    uint8_t GetSign();
    uint8_t GetZero();
    uint8_t GetAux();
    uint8_t GetParity();
    uint8_t GetCarry();
};

#endif /* Status_hpp */
