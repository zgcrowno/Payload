// The following define/undef is done only once in the project. It should be
// done before including the interface of the class deriving from
// Scroll (as follows).
#define __SCROLL_IMPL__
#include "Payload.h"
#undef __SCROLL_IMPL__

// The following define/undef is done only once in the project. It should be
// done before including ORX's Nuklear implementation.
#define orxNUKLEAR_IMPL
#include "orxNuklear.h"
#undef orxNUKLEAR_IMPL

#include "Bypass.h"
#include "EventType.h"
#include "Firewall.h"
#include "Goal.h"
#include "Infection.h"
#include "MemorySetCartesian1D.h"
#include "MemorySetCartesian2D.h"
#include "MemorySetPolar1D.h"
#include "MemorySetPolar2D.h"
#include "NuklearWindow.h"
#include "PlayerPayload.h"
#include "Protocol.h"
#include "Proxy.h"
#include "Recursive.h"
#include "ScrollMod.h"
#include "Tile.h"
#include "TileSet.h"
#include "Unreachable.h"
#include "Virus.h"

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

    // NUKLEAR INITIALIZATION.
    orxNuklear_Init();
    // EVENT HANDLERS.
    orxEvent_AddHandler(EVENT_TYPE_TILE_INHABITANT, EventHandler);
    orxEvent_AddHandler(EVENT_TYPE_MEMORY_SET, EventHandler);
    // GAME OBJECT INSTANTIATION.
    CreateObject("O-SceneLevel");

    return result;
}

void Payload::BindObjects()
{
    ScrollBindObject<Bypass>("O-Bypass");
    ScrollBindObject<Firewall>("O-Firewall");
    ScrollBindObject<Goal>("O-Goal");
    ScrollBindObject<Infection>("O-Infection");
    ScrollBindObject<MemorySetCartesian1D>("O-MemorySetCartesian1D");
    ScrollBindObject<MemorySetCartesian2D>("O-MemorySetCartesian2D");
    ScrollBindObject<MemorySetPolar1D>("O-MemorySetPolar1D");
    ScrollBindObject<MemorySetPolar2D>("O-MemorySetPolar2D");
    ScrollBindObject<NuklearWindow>("O-NuklearWindow");
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

    // Draw NuklearWindows as appropriate.
    DrawNuklearWindows();
    // Allow debug-based quitting out.
    if (orxInput_IsActive("Quit"))
    {
        retVal = orxSTATUS_FAILURE;
    }

    return retVal;
}

void Payload::Exit()
{
    orxNuklear_Exit();
}

void Payload::DrawNuklearWindows()
{
    std::vector<ScrollObject*> nuklearWindows = GetNuklearWindows();
    for (ScrollObject *nuklearWindow : nuklearWindows)
    {
        // The fully cast NuklearWindow.
        NuklearWindow nWin = *static_cast<NuklearWindow*>(nuklearWindow);
        // The NuklearWindow's starting position.
        orxVECTOR nWinStartingPos = nWin.GetPosition();
        // The NuklearWindow's starting size.
        orxVECTOR nWinStartingSize = nWin.GetScaledSize();
        // Set the nk_panel_flags as appropriate.
        nk_flags flags = 0;
        if (nWin.m_bIsBordered)
        {
            flags |= NK_WINDOW_BORDER;
        }
        if (nWin.m_bIsMovable)
        {
            flags |= NK_WINDOW_MOVABLE;
        }
        if (nWin.m_bIsScalable)
        {
            flags |= NK_WINDOW_SCALABLE;
        }
        if (nWin.m_bIsClosable)
        {
            flags |= NK_WINDOW_CLOSABLE;
        }
        if (nWin.m_bIsMinimizable)
        {
            flags |= NK_WINDOW_MINIMIZABLE;
        }
        if (nWin.m_bHasNoScrollbar)
        {
            flags |= NK_WINDOW_NO_SCROLLBAR;
        }
        if (nWin.m_bHasTitle)
        {
            flags |= NK_WINDOW_TITLE;
        }
        if (nWin.m_bAutoHidesScrollbar)
        {
            flags |= NK_WINDOW_SCROLL_AUTO_HIDE;
        }
        if (nWin.m_bIsKeptInBackground)
        {
            flags |= NK_WINDOW_BACKGROUND;
        }
        if (nWin.m_bIsScaledLeft)
        {
            flags |= NK_WINDOW_SCALE_LEFT;
        }
        if (nWin.m_bDisallowsInput)
        {
            flags |= NK_WINDOW_NO_INPUT;
        }
        // Draw the NuklearWindow.
        if (nk_begin(&sstNuklear.stContext, nWin.m_title.c_str(), nk_rect(nWinStartingPos.fX, nWinStartingPos.fY, nWinStartingSize.fX, nWinStartingSize.fY), flags))
        {
            enum { EASY, HARD };
            static orxS32 Op = EASY;
            static int Property = 20;

            nk_layout_row_static(&sstNuklear.stContext, 30, 80, 1);
            if (nk_button_label(&sstNuklear.stContext, "button"))
            {
                orxLOG("Nuklear button pressed.");
            }
            nk_layout_row_dynamic(&sstNuklear.stContext, 30, 2);
            if (nk_option_label(&sstNuklear.stContext, "easy", Op == EASY))
            {
                Op = EASY;
            }
            if (nk_option_label(&sstNuklear.stContext, "hard", Op == HARD))
            {
                Op = HARD;
            }
            nk_layout_row_dynamic(&sstNuklear.stContext, 25, 1);
            nk_property_int(&sstNuklear.stContext, "Compression:", 0, &Property, 100, 10, 1);
        }
        nk_end(&sstNuklear.stContext);
    }
}

const int Payload::GetPayloadRow()
{
    PlayerPayload *payload = GetNextObject<PlayerPayload>();
    if (payload->m_target == nullptr)
    {
        // If the payload's target is null, we want to use its last associated row.
        return payload->m_priorTargetStack.top().first->m_row;
    }
    else
    {
        return payload->m_target->m_row;
    }
}

ScrollObject *Payload::GetTileSet()
{
    return GetNextObject<TileSet>();
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
        if (orxString_SearchString(firewall->GetModelName().c_str(), "O-Firewall") != nullptr)
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
        if (orxString_SearchString(protocol->GetModelName().c_str(), "O-Protocol") != nullptr)
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
    // Infection
    for (Infection *infection = GetNextObject<Infection>(); infection != nullptr; infection = GetNextObject<Infection>(infection))
    {
        if (!orxString_Compare(infection->GetModelName().c_str(), "O-Infection"))
        {
            retVal.push_back(infection);
        }
    }

    return retVal;
}

std::vector<ScrollObject*> Payload::GetFirewalls()
{
    std::vector<ScrollObject*> retVal;

    for (Firewall *firewall = GetNextObject<Firewall>(); firewall != nullptr; firewall = GetNextObject<Firewall>(firewall))
    {
        if (!orxString_Compare(firewall->GetModelName().c_str(), "O-Firewall"))
        {
            retVal.push_back(firewall);
        }
    }

    return retVal;
}

std::vector<ScrollObject*> Payload::GetNuklearWindows()
{
    std::vector<ScrollObject*> retVal;

    for (NuklearWindow *nuklearWindow = GetNextObject<NuklearWindow>(); nuklearWindow != nullptr; nuklearWindow = GetNextObject<NuklearWindow>(nuklearWindow))
    {
        retVal.push_back(nuklearWindow);
    }

    return retVal;
}

orxSTATUS orxFASTCALL Payload::EventHandler(const orxEVENT *_pstEvent)
{
    if (_pstEvent->eType == EVENT_TYPE_TILE_INHABITANT || _pstEvent->eType == EVENT_TYPE_MEMORY_SET)
    {
        TileSet *tileSet = static_cast<TileSet*>(_pstEvent->hRecipient);
        if (_pstEvent->eType == EVENT_TYPE_TILE_INHABITANT)
        {
            float timeToExecuteAction = *static_cast<float*>(_pstEvent->pstPayload);
            tileSet->m_priorDoers.push({ { static_cast<Doer*>(_pstEvent->hSender), tileSet->m_timeSpentShifting + timeToExecuteAction }, tileSet->m_shiftStatus != TileSetShiftStatus::None });
        }
        else
        {
            tileSet->m_priorDoers.push({ { static_cast<Doer*>(_pstEvent->hSender), 0.0f }, tileSet->m_shiftStatus != TileSetShiftStatus::None });
        }
    }

    return orxSTATUS_SUCCESS;
}

int main(int argc, char **argv)
{
    // Executes game
    Payload::GetInstance().Execute(argc, argv);

    // Done!
    return EXIT_SUCCESS;
}
