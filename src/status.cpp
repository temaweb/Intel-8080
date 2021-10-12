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

#include "status.hpp"

Status::Status(uint8_t status)
{
    this -> status = (status & 0xD7) | defaultState;
}

Status::operator uint8_t() const
{
    return this -> status;
}

#pragma mark -
#pragma mark Set flags

void Status::SetFlag(Status::Flags flag, bool value)
{
    if (value) {
        status |= flag;
    } else {
        status &= ~flag;
    }
}

void Status::SetSign (bool flag)
{
    SetFlag(S, flag);
}

void Status::SetSign (uint16_t value)
{
    SetSign((bool)(value & 0x0080));
}

void Status::SetZero (bool flag)
{
    SetFlag(Z, flag);
}

void Status::SetZero (uint16_t value)
{
    SetZero((value & 0x00FF) == 0x0000);
}

void Status::SetAux (bool flag)
{
    SetFlag(AC, flag);
}

void Status::SetAux (uint16_t value)
{
    SetAux((value & 0x010) != 0);
}

void Status::SetParity (bool flag)
{
    SetFlag(P, flag);
}

void Status::SetParity (uint16_t value)
{
    value &= 0x00FF;
    
    value ^= value >> 4;
    value ^= value >> 2;
    value ^= value >> 1;
    
    SetParity((bool)((~value) & 1));
}

void Status::SetCarry (bool flag)
{
    SetFlag(C, flag);
}

void Status::SetCarry (uint16_t value)
{
    SetCarry((value & 0x100) != 0);
}

#pragma mark -
#pragma mark Set all

void Status::SetAllFlags (uint16_t value)
{
    SetCarry    (value);
    SetAuxFlags (value);
}

void Status::SetAuxFlags (uint16_t value)
{
    SetAux      (value);
    SetDecFlags (value);
}

void Status::SetDecFlags (uint16_t value)
{
    SetSign    (value);
    SetZero    (value);
    SetParity  (value);
}

#pragma mark -
#pragma mark Get flags

uint8_t Status::GetSign() const
{
    return (status & S) >> 7;
}

uint8_t Status::GetZero() const
{
    return (status & Z) >> 6;
}

uint8_t Status::GetAux() const
{
    return (status & AC) >> 4;
}

uint8_t Status::GetParity() const
{
    return (status & P) >> 2;
}

uint8_t Status::GetCarry() const
{
    return (status & C);
}

#pragma mark -
#pragma mark Other

void Status::Reset()
{
    status = defaultState;
}

void Status::InvertCarry()
{
    status ^= C;
}
