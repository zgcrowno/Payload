#pragma once

#include "Payload.h"

namespace payload
{
    class Command
    {
    public:
        virtual ~Command();
        virtual void Execute() = 0;
        virtual void Undo() = 0;
    };
}
