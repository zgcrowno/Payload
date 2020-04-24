#pragma once

#include "Payload.h"
#include "Command.h"
#include "Direction.h"
#include "Tile.h"
#include "TileInhabitant.h"

namespace payload
{
    class CommandTileInhabitantSetTarget : public Command
    {
    public:
        CommandTileInhabitantSetTarget(TileInhabitant *_tileInhabitant, Tile *_target);

        virtual void Execute();
        virtual void Undo();
    private:
        TileInhabitant *m_tileInhabitant;
        Tile *m_priorTarget;
        Direction m_priorDirection;
        Tile *m_target;
        Direction m_direction;
    };
}
