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
#include "NuklearButton.h"
#include "NuklearCheckBox.h"
#include "NuklearCombo.h"
#include "NuklearLayoutRow.h"
#include "NuklearLayoutType.h"
#include "NuklearText.h"
#include "NuklearTextAlignment.h"
#include "NuklearWindow.h"
#include "NuklearWindowElement.h"
#include "NuklearWindowElementType.h"
#include "PlayerPayload.h"
#include "Protocol.h"
#include "Proxy.h"
#include "Recursive.h"
#include "ScrollMod.h"
#include "Tile.h"
#include "TileSet.h"
#include "Unreachable.h"
#include "Virus.h"
#include <algorithm>

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
    CreateObject("O-SceneDashboard");
    //CreateObject("O-SceneBoot");
    //CreateObject("O-SceneLevel");

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
    ScrollBindObject<NuklearButton>("O-NuklearButton");
    ScrollBindObject<NuklearCheckBox>("O-NuklearCheckBox");
    ScrollBindObject<NuklearCombo>("O-NuklearCombo");
    ScrollBindObject<NuklearLayoutRow>("O-NuklearLayoutRow");
    ScrollBindObject<NuklearText>("O-NuklearText");
    ScrollBindObject<NuklearWindow>("O-NuklearWindow");
    ScrollBindObject<NuklearWindowElement>("O-NuklearWindowElement");
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

void Payload::DrawNuklearWindow(ScrollObject *_nWin, bool _bIsSubWindow)
{
    // The fully cast NuklearWindow.
    NuklearWindow *nWin = static_cast<NuklearWindow*>(_nWin);
    // The NuklearWindow's starting position.
    orxVECTOR nWinStartingPos = nWin->GetPosition();
    // The NuklearWindow's starting size.
    orxVECTOR nWinStartingSize = nWin->GetScaledSize();
    // Set the nk_panel_flags as appropriate.
    nk_flags flags = 0;
    if (nWin->m_bIsBordered)
    {
        flags |= NK_WINDOW_BORDER;
    }
    if (nWin->m_bIsMovable)
    {
        flags |= NK_WINDOW_MOVABLE;
    }
    if (nWin->m_bIsScalable)
    {
        flags |= NK_WINDOW_SCALABLE;
    }
    if (nWin->m_bIsClosable)
    {
        flags |= NK_WINDOW_CLOSABLE;
    }
    if (nWin->m_bIsMinimizable)
    {
        flags |= NK_WINDOW_MINIMIZABLE;
    }
    if (nWin->m_bHasNoScrollbar)
    {
        flags |= NK_WINDOW_NO_SCROLLBAR;
    }
    if (nWin->m_bHasTitle)
    {
        flags |= NK_WINDOW_TITLE;
    }
    if (nWin->m_bAutoHidesScrollbar)
    {
        flags |= NK_WINDOW_SCROLL_AUTO_HIDE;
    }
    if (nWin->m_bIsKeptInBackground)
    {
        flags |= NK_WINDOW_BACKGROUND;
    }
    if (nWin->m_bIsScaledLeft)
    {
        flags |= NK_WINDOW_SCALE_LEFT;
    }
    if (nWin->m_bDisallowsInput)
    {
        flags |= NK_WINDOW_NO_INPUT;
    }
    // Draw the NuklearWindow.
    if (_bIsSubWindow)
    {
        if (nk_group_begin(&sstNuklear.stContext, nWin->m_title.c_str(), flags))
        {
            DrawNuklearLayoutRows(nWin);

            nk_group_end(&sstNuklear.stContext);
        }
    }
    else
    {
        if (nk_begin(&sstNuklear.stContext, nWin->m_title.c_str(), nk_rect(nWinStartingPos.fX, nWinStartingPos.fY, nWinStartingSize.fX, nWinStartingSize.fY), flags))
        {
            DrawNuklearLayoutRows(nWin);
            /*enum { EASY, HARD };
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
            nk_layout_row_dynamic(&sstNuklear.stContext, 50, 1);
            nk_text_wrap_colored(&sstNuklear.stContext, "Some text for ya!", strlen("Some text for ya!"), {255, 255, 255, 255});
            nk_layout_row_dynamic(&sstNuklear.stContext, 25, 1);
            nk_button_image(&sstNuklear.stContext, nk_image_id(0));
            nk_layout_row_dynamic(&sstNuklear.stContext, 25, 1);
            int selected = 0;
            const char *items[] = { "item1", "item2", "item3" };
            nk_combobox(&sstNuklear.stContext, items, 3, &selected, 25, { 100, 100 });
            nk_layout_row_dynamic(&sstNuklear.stContext, 25, 1);
            nk_combo(&sstNuklear.stContext, items, 3, selected, 25, { 100, 100 });
            nk_layout_row_dynamic(&sstNuklear.stContext, 25, 1);
            int sliderVal = 5;
            nk_slider_int(&sstNuklear.stContext, 0, &sliderVal, 10, 1);
            nk_layout_row_dynamic(&sstNuklear.stContext, 25, 1);
            int checkBoxInactive = 0;
            int checkBoxActive = 1;
            nk_checkbox_label(&sstNuklear.stContext, "Checkbox Label 1", &checkBoxActive);
            nk_layout_row_dynamic(&sstNuklear.stContext, 25, 1);
            nk_checkbox_label(&sstNuklear.stContext, "Checkbox Label 2", &checkBoxInactive);*/
        }
        nk_end(&sstNuklear.stContext);
    }
}

void Payload::DrawNuklearWindows()
{
    std::vector<ScrollObject*> nuklearWindows = GetNuklearWindows();
    for (ScrollObject *nuklearWindow : nuklearWindows)
    {
        DrawNuklearWindow(nuklearWindow);
    }
}

void Payload::DrawNuklearLayoutRows(ScrollObject *_nWin)
{
    // The fully cast NuklearWindow.
    NuklearWindow *nWin = static_cast<NuklearWindow*>(_nWin);
    // Draw the NuklearWindow's NuklearLayoutRows.
    for (NuklearLayoutRow *row : nWin->m_layoutRows)
    {
        // Determine the row's format.
        nk_layout_format format;
        switch (row->m_format)
        {
        case NuklearLayoutFormat::Static:
            format = NK_STATIC;
            break;
        case NuklearLayoutFormat::Dynamic:
            format = NK_DYNAMIC;
            break;
        default:
            break;
        }
        // Set up the row.
        switch (row->m_type)
        {
        case NuklearLayoutType::Row:
            nk_layout_row(&sstNuklear.stContext, format, row->m_height, row->m_elements.size(), row->m_sizeOrRatio.data());
            break;
        case NuklearLayoutType::RowBegin:
            nk_layout_row_begin(&sstNuklear.stContext, format, row->m_height, row->m_elements.size());
            break;
        case NuklearLayoutType::RowStatic:
            nk_layout_row_static(&sstNuklear.stContext, row->m_height, row->m_elementWidth, row->m_elements.size());
            break;
        case NuklearLayoutType::RowDynamic:
            nk_layout_row_dynamic(&sstNuklear.stContext, row->m_height, row->m_elements.size());
            break;
        case NuklearLayoutType::RowTemplateBegin:
            nk_layout_row_template_begin(&sstNuklear.stContext, row->m_height);
            break;
        case NuklearLayoutType::SpaceBegin:
            nk_layout_space_begin(&sstNuklear.stContext, format, row->m_height, row->m_elements.size());
            break;
        }
        // Draw the NuklearLayoutRow's individual elements, and handle their behaviors.
        for (int i = 0; i < row->m_elements.size(); i++)
        {
            NuklearWindowElement *ele = row->m_elements.at(i);
            // Push any necessary values for the row.
            switch (row->m_type)
            {
            case NuklearLayoutType::RowBegin:
                nk_layout_row_push(&sstNuklear.stContext, row->m_widthsOrRatios.at(i));
                break;
            case NuklearLayoutType::RowTemplateBegin:
                switch (row->m_format)
                {
                case NuklearLayoutFormat::Static:
                    nk_layout_row_template_push_static(&sstNuklear.stContext, row->m_elementWidths.at(i));
                    break;
                case NuklearLayoutFormat::Dynamic:
                    nk_layout_row_template_push_dynamic(&sstNuklear.stContext);
                    break;
                case NuklearLayoutFormat::Variable:
                    nk_layout_row_template_push_variable(&sstNuklear.stContext, row->m_elementMinWidths.at(i));
                    break;
                }
                break;
            case NuklearLayoutType::SpaceBegin:
                orxVECTOR elementStartingPosition = row->m_elementStartingPositions.at(i);
                orxVECTOR elementStartingSize = row->m_elementStartingSizes.at(i);
                nk_layout_space_push(
                    &sstNuklear.stContext,
                    nk_rect(elementStartingPosition.fX, elementStartingPosition.fY, elementStartingSize.fX, elementStartingSize.fY));
                break;
            default:
                break;
            }
            // Handle the widgets.
            switch (ele->m_type)
            {
            case NuklearWindowElementType::Button:
            {
                NuklearButton *button = static_cast<NuklearButton*>(ele);
                if (nk_button_image(&sstNuklear.stContext, nk_image_ptr(button->m_textureBitmap)))
                {
                    button->Interact();
                }
            }
            break;
            case NuklearWindowElementType::CheckBox:
            {
                NuklearCheckBox *checkBox = static_cast<NuklearCheckBox*>(ele);
                if (nk_checkbox_label(&sstNuklear.stContext, checkBox->m_label.c_str(), &checkBox->m_active))
                {
                    checkBox->Interact();
                }
            }
            break;
            case NuklearWindowElementType::Combo:
            {
                NuklearCombo *combo = static_cast<NuklearCombo*>(ele);
                if (nk_combo(
                    &sstNuklear.stContext,
                    combo->m_elements.data(),
                    combo->m_elements.size(),
                    combo->m_selectedIndex,
                    combo->m_elementHeight,
                    { combo->m_comboSize.fX, combo->m_comboSize.fY }))
                {
                    combo->Interact();
                }
            }
            break;
            case NuklearWindowElementType::Text:
            {
                NuklearText *text = static_cast<NuklearText*>(ele);
                nk_style_set_font(&sstNuklear.stContext, &sstNuklear.apstFonts[text->m_fontIndex]->handle);
                if (text->m_wrap)
                {
                    nk_text_wrap_colored(
                        &sstNuklear.stContext,
                        text->m_staticContent.c_str(),
                        text->m_staticContent.length(),
                        { static_cast<nk_byte>(text->m_textColor.fR), static_cast<nk_byte>(text->m_textColor.fG), static_cast<nk_byte>(text->m_textColor.fB), static_cast<nk_byte>(text->m_textAlpha) });
                }
                else
                {
                    nk_text_align nkAlignment;
                    switch (text->m_alignment)
                    {
                    case NuklearTextAlignment::Bottom:
                        nkAlignment = NK_TEXT_ALIGN_BOTTOM;
                        break;
                    case NuklearTextAlignment::Centered:
                        nkAlignment = NK_TEXT_ALIGN_CENTERED;
                        break;
                    case NuklearTextAlignment::Left:
                        nkAlignment = NK_TEXT_ALIGN_LEFT;
                        break;
                    case NuklearTextAlignment::Middle:
                        nkAlignment = NK_TEXT_ALIGN_MIDDLE;
                        break;
                    case NuklearTextAlignment::Right:
                        nkAlignment = NK_TEXT_ALIGN_RIGHT;
                        break;
                    case NuklearTextAlignment::Top:
                        nkAlignment = NK_TEXT_ALIGN_TOP;
                        break;
                    }
                    nk_text_colored(
                        &sstNuklear.stContext,
                        text->m_staticContent.c_str(),
                        text->m_staticContent.length(),
                        nkAlignment,
                        { static_cast<nk_byte>(text->m_textColor.fR), static_cast<nk_byte>(text->m_textColor.fG), static_cast<nk_byte>(text->m_textColor.fB), static_cast<nk_byte>(text->m_textAlpha) });
                }
            }
            break;
            case NuklearWindowElementType::Window:
            {
                NuklearWindow *window = static_cast<NuklearWindow*>(ele);
                DrawNuklearWindow(window, true);
            }
            break;
            }
            // End the rows as necessary.
            switch (row->m_type)
            {
            case NuklearLayoutType::RowBegin:
                nk_layout_row_end(&sstNuklear.stContext);
                break;
            case NuklearLayoutType::RowTemplateBegin:
                nk_layout_row_template_end(&sstNuklear.stContext);
                break;
            case NuklearLayoutType::SpaceBegin:
                nk_layout_space_end(&sstNuklear.stContext);
                break;
            default:
                break;
            }
        }
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
