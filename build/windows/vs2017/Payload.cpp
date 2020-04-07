//! Includes
// The following define/undef is done only once in the project. It should be
// done before including the interface of the class deriving from
// Scroll (as follows).
#define __SCROLL_IMPL__
#include "Payload.h"
#include "Bypass.h"
#include "Firewall.h"
#include "Goal.h"
#include "MemorySetCartesian1D.h"
#include "MemorySetCartesian2D.h"
#include "MemorySetPolar1D.h"
#include "MemorySetPolar2D.h"
#include "PlayerPayload.h"
#include "Protocol.h"
#include "Proxy.h"
#include "Recursive.h"
#include "ScrollMod.h"
#include "Tile.h"
#include "TileSet.h"
#include "Unreachable.h"
#include "Virus.h"
#undef __SCROLL_IMPL__

// Use non-dedicated graphics card, if appropriate.
#ifdef __orxWINDOWS__
#include <windows.h>

extern "C"
{
    _declspec(dllexport) DWORD NvOptimusEnablement = 1;
    _declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif // __orxWINDOWS__

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
    //CreateObject("O-PlayerPayload");
    //CreateObject("O-Tile");
    //CreateObject("O-SceneMain");

    return result;
}

void Payload::BindObjects()
{
    ScrollBindObject<Bypass>("O-Bypass");
    ScrollBindObject<Firewall>("O-Firewall");
    ScrollBindObject<Goal>("O-Goal");
    ScrollBindObject<MemorySetCartesian1D>("O-MemorySetCartesian1D");
    ScrollBindObject<MemorySetCartesian2D>("O-MemorySetCartesian2D");
    ScrollBindObject<MemorySetPolar1D>("O-MemorySetPolar1D");
    ScrollBindObject<MemorySetPolar2D>("O-MemorySetPolar2D");
    ScrollBindObject<PlayerPayload>("O-PlayerPayload");
    ScrollBindObject<Protocol>("O-Protocol");
    ScrollBindObject<Proxy>("O-Proxy");
    ScrollBindObject<Recursive>("O-Recursive");
    ScrollBindObject<ScrollMod>("O-ScrollMod");
    ScrollBindObject<Tile>("O-Tile");
    ScrollBindObject<TileSet>("O-TileSet");
    ScrollBindObject<Unreachable>("O-Unreachable");
    ScrollBindObject<Virus>("O-Virus");
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

std::vector<ScrollObject*> Payload::GetTileInhabitants()
{
    std::vector<ScrollObject*> retVal;

    // Bypass
    for (Bypass *bypass = GetNextObject<Bypass>(); bypass != nullptr; bypass = GetNextObject<Bypass>(bypass))
    {
        if (!orxString_Compare(bypass->GetModelName().c_str(), "O-Bypass"))
        {
            retVal.push_back(bypass);
        }
    }
    // Firewall
    for (Firewall *firewall = GetNextObject<Firewall>(); firewall != nullptr; firewall = GetNextObject<Firewall>(firewall))
    {
        if (!orxString_Compare(firewall->GetModelName().c_str(), "O-Firewall"))
        {
            retVal.push_back(firewall);
        }
    }
    // Goal
    for (Goal *goal = GetNextObject<Goal>(); goal != nullptr; goal = GetNextObject<Goal>(goal))
    {
        if (!orxString_Compare(goal->GetModelName().c_str(), "O-Goal"))
        {
            retVal.push_back(goal);
        }
    }
    // PlayerPayload
    for (PlayerPayload *payload = GetNextObject<PlayerPayload>(); payload != nullptr; payload = GetNextObject<PlayerPayload>(payload))
    {
        if (!orxString_Compare(payload->GetModelName().c_str(), "O-PlayerPayload"))
        {
            retVal.push_back(payload);
        }
    }
    // Protocol
    for (Protocol *protocol = GetNextObject<Protocol>(); protocol != nullptr; protocol = GetNextObject<Protocol>(protocol))
    {
        if (!orxString_Compare(protocol->GetModelName().c_str(), "O-Protocol"))
        {
            retVal.push_back(protocol);
        }
    }
    // Proxy
    for (Proxy *proxy = GetNextObject<Proxy>(); proxy != nullptr; proxy = GetNextObject<Proxy>(proxy))
    {
        if (!orxString_Compare(proxy->GetModelName().c_str(), "O-Proxy"))
        {
            retVal.push_back(proxy);
        }
    }
    // Recursive
    for (Recursive *recursive = GetNextObject<Recursive>(); recursive != nullptr; recursive = GetNextObject<Recursive>(recursive))
    {
        if (!orxString_Compare(recursive->GetModelName().c_str(), "O-Recursive"))
        {
            retVal.push_back(recursive);
        }
    }
    // Unreachable
    for (Unreachable *unreachable = GetNextObject<Unreachable>(); unreachable != nullptr; unreachable = GetNextObject<Unreachable>(unreachable))
    {
        if (!orxString_Compare(unreachable->GetModelName().c_str(), "O-Unreachable"))
        {
            retVal.push_back(unreachable);
        }
    }
    // Virus
    for (Virus *virus = GetNextObject<Virus>(); virus != nullptr; virus = GetNextObject<Virus>(virus))
    {
        if (!orxString_Compare(virus->GetModelName().c_str(), "O-Virus"))
        {
            retVal.push_back(virus);
        }
    }

    return retVal;
}

int main(int argc, char **argv)
{
    // Executes game
    Payload::GetInstance().Execute(argc, argv);

    // Done!
    return EXIT_SUCCESS;
}
