//
//  Command.cpp
//  8080
//
//  Created by Артём Оконечников on 05.01.2021.
//

#include "Cpu.hpp"
#include "Command.hpp"

bool Command::isImplied()
{
    return addrmod == &Cpu::IMP;
}

bool Command::isIndirect()
{
    return addrmod == &Cpu::DIR;
}
