//! Includes
// The following define/undef is done only once in the project. It should be
// done before including the interface of the class deriving from
// Scroll (as follows).
#define __SCROLL_IMPL__
#include "Payload.h"
#include "ScrollMod.h"
#include "Tile.h"
#include "TileSet.h"
#undef __SCROLL_IMPL__

using namespace payload;

orxSTATUS Payload::Bootstrap() const
{
    // Add "../data/config" to the list of locations from which config files can be loaded
    orxResource_AddStorage(orxCONFIG_KZ_RESOURCE_GROUP, "../data/config", orxFALSE);

    // Loads a specific config file
    orxConfig_Load("Payload.ini");

    return orxSTATUS_FAILURE;
}

orxSTATUS Payload::Init()
{
    orxSTATUS result = orxSTATUS_SUCCESS;

    // Instantiate game objects
    CreateObject("O-TileSet");
    //CreateObject("O-Tile");
    //CreateObject("O-SceneMain");

    return result;
}

void Payload::BindObjects()
{
    ScrollBindObject<ScrollMod>("O-ScrollMod");
    ScrollBindObject<Tile>("O-Tile");
    ScrollBindObject<TileSet>("O-TileSet");
}

orxSTATUS Payload::Run()
{
    orxSTATUS retVal = orxSTATUS_SUCCESS;

    if (orxInput_IsActive("Quit"))
    {
        retVal = orxSTATUS_FAILURE;
    }

    return retVal;
}

void Payload::Exit()
{
}

int main(int argc, char **argv)
{
    // Executes game
    Payload::GetInstance().Execute(argc, argv);

    // Done!
    return EXIT_SUCCESS;
}
