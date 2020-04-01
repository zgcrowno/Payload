#pragma once

#include "Payload.h"

namespace payload
{
    enum class TileSetShiftStatus
    {
        None, // The TileSet is not shifting.
        D1, // The TileSet is shifting to 1D.
        D2, // The TileSet is shifting to 2D.
        Cartesian, // The TileSet is shifting to Cartesian.
        Polar, // The TileSet is shifting to Polar.
        D1Tiles // The TileSet is shifting its tiles after having shifted to 1D.
    };
}
