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
#include "Dashboard.h"
#include "EventType.h"
#include "Firewall.h"
#include "Goal.h"
#include "Infection.h"
#include "MemorySetCartesian1D.h"
#include "MemorySetCartesian2D.h"
#include "MemorySetPolar1D.h"
#include "MemorySetPolar2D.h"
#include "Moon.h"
#include "NuklearButton.h"
#include "NuklearCheckBox.h"
#include "NuklearCombo.h"
#include "NuklearRow.h"
#include "NuklearRowBegin.h"
#include "NuklearRowDynamic.h"
#include "NuklearRowSpaceBegin.h"
#include "NuklearRowStatic.h"
#include "NuklearRowTemplateBegin.h"
#include "NuklearText.h"
#include "NuklearWindow.h"
#include "NuklearWindowElement.h"
#include "PlayerPayload.h"
#include "Protocol.h"
#include "Proxy.h"
#include "Recursive.h"
#include "SceneBoot.h"
#include "SceneDashboard.h"
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
    //CreateObject("O-SceneDashboard");
    CreateObject("O-SceneBoot");
    //CreateObject("O-SceneLevel");

    return result;
}

void Payload::BindObjects()
{
    ScrollBindObject<Bypass>("O-Bypass");
    ScrollBindObject<Dashboard>("O-Dashboard");
    ScrollBindObject<Firewall>("O-Firewall");
    ScrollBindObject<Goal>("O-Goal");
    ScrollBindObject<Infection>("O-Infection");
    ScrollBindObject<MemorySetCartesian1D>("O-MemorySetCartesian1D");
    ScrollBindObject<MemorySetCartesian2D>("O-MemorySetCartesian2D");
    ScrollBindObject<MemorySetPolar1D>("O-MemorySetPolar1D");
    ScrollBindObject<MemorySetPolar2D>("O-MemorySetPolar2D");
    ScrollBindObject<Moon>("O-Moon");
    ScrollBindObject<NuklearButton>("O-NuklearButton");
    ScrollBindObject<NuklearCheckBox>("O-NuklearCheckBox");
    ScrollBindObject<NuklearCombo>("O-NuklearCombo");
    ScrollBindObject<NuklearRow>("O-NuklearRow");
    ScrollBindObject<NuklearRowBegin>("O-NuklearRowBegin");
    ScrollBindObject<NuklearRowDynamic>("O-NuklearRowDynamic");
    ScrollBindObject<NuklearRowSpaceBegin>("O-NuklearRowSpaceBegin");
    ScrollBindObject<NuklearRowStatic>("O-NuklearRowStatic");
    ScrollBindObject<NuklearRowTemplateBegin>("O-NuklearRowTemplateBegin");
    ScrollBindObject<NuklearText>("O-NuklearText");
    ScrollBindObject<NuklearWindow>("O-NuklearWindow");
    ScrollBindObject<NuklearWindowElement>("O-NuklearWindowElement");
    ScrollBindObject<PlayerPayload>("O-PlayerPayload");
    ScrollBindObject<Protocol>("O-Protocol");
    ScrollBindObject<Proxy>("O-Proxy");
    ScrollBindObject<Recursive>("O-Recursive");
    ScrollBindObject<SceneBoot>("O-SceneBoot");
    ScrollBindObject<SceneDashboard>("O-SceneDashboard");
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
        DrawNuklearWindow(nuklearWindow);
    }
}

void Payload::DrawNuklearWindow(ScrollObject *_nWin, bool _bIsSubWindow)
{
    // The fully cast NuklearWindow.
    NuklearWindow *nWin = static_cast<NuklearWindow*>(_nWin);
    // Only draw the NuklearWindow if it's enabled.
    if (nWin->IsEnabled())
    {
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
        if (nWin->m_bUsesCustomSkin)
        {
            sstNuklear.stContext.style.window.fixed_background = nk_style_item_image(sstNuklear.astSkins[nWin->m_skinIndex].stImage);
        }
        else
        {
            sstNuklear.stContext.style.window.fixed_background.data.color = { nk_byte(nWin->m_backgroundColor.fX), nk_byte(nWin->m_backgroundColor.fY), nk_byte(nWin->m_backgroundColor.fZ), nk_byte(nWin->m_backgroundAlpha) };
            sstNuklear.stContext.style.window.border_color = { nk_byte(nWin->m_borderColor.fX), nk_byte(nWin->m_borderColor.fY), nk_byte(nWin->m_borderColor.fZ), nk_byte(nWin->m_borderAlpha) };
        }
        if (_bIsSubWindow)
        {
            if (nk_group_begin(&sstNuklear.stContext, nWin->m_title.c_str(), flags))
            {
                DrawNuklearWindowElements(nWin);

                nk_group_end(&sstNuklear.stContext);
            }
        }
        else
        {
            if (nk_begin(&sstNuklear.stContext, nWin->m_title.c_str(), nk_rect(nWinStartingPos.fX, nWinStartingPos.fY, nWinStartingSize.fX, nWinStartingSize.fY), flags))
            {
                DrawNuklearWindowElements(nWin);
            }
            nk_end(&sstNuklear.stContext);
        }
    }
}

void Payload::DrawNuklearWindowElements(ScrollObject *_nWin)
{
    // The fully cast NuklearWindow.
    NuklearWindow *nWin = static_cast<NuklearWindow*>(_nWin);
    // Draw the NuklearWindow's elements.
    for (NuklearLayoutRow *layoutRow : nWin->m_elements)
    {
        DrawNuklearLayoutRow(layoutRow);
    }
}

void Payload::DrawNuklearLayoutRow(ScrollObject *_nuklearLayoutRow)
{
    NuklearLayoutRow *layoutRow = dynamic_cast<NuklearLayoutRow*>(_nuklearLayoutRow);
    if (layoutRow != nullptr)
    {
        // Determine the row's format.
        nk_layout_format format;
        switch (layoutRow->m_format)
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
        NuklearRow *row = dynamic_cast<NuklearRow*>(layoutRow);
        NuklearRowBegin *rowBegin = dynamic_cast<NuklearRowBegin*>(layoutRow);
        NuklearRowDynamic *rowDynamic = dynamic_cast<NuklearRowDynamic*>(layoutRow);
        NuklearRowSpaceBegin *rowSpaceBegin = dynamic_cast<NuklearRowSpaceBegin*>(layoutRow);
        NuklearRowStatic *rowStatic = dynamic_cast<NuklearRowStatic*>(layoutRow);
        NuklearRowTemplateBegin *rowTemplateBegin = dynamic_cast<NuklearRowTemplateBegin*>(layoutRow);
        if (row != nullptr)
        {
            nk_layout_row(&sstNuklear.stContext, format, row->m_height, row->m_elements.size(), row->m_sizeOrRatio.data());
        }
        else if (rowBegin != nullptr)
        {
            nk_layout_row_begin(&sstNuklear.stContext, format, rowBegin->m_height, rowBegin->m_elements.size());
        }
        else if (rowDynamic != nullptr)
        {
            nk_layout_row_dynamic(&sstNuklear.stContext, rowDynamic->m_height, rowDynamic->m_elements.size());
        }
        else if (rowSpaceBegin != nullptr)
        {
            nk_layout_space_begin(&sstNuklear.stContext, format, rowSpaceBegin->m_height, rowSpaceBegin->m_elements.size());
        }
        else if (rowStatic != nullptr)
        {
            nk_layout_row_static(&sstNuklear.stContext, rowStatic->m_height, rowStatic->m_elementWidth, rowStatic->m_elements.size());
        }
        else if (rowTemplateBegin != nullptr)
        {
            nk_layout_row_template_begin(&sstNuklear.stContext, rowTemplateBegin->m_height);
        }
        // Draw the NuklearLayoutRow's individual elements, and handle their behaviors.
        for (int i = 0; i < layoutRow->m_elements.size(); i++)
        {
            NuklearWindowElement *ele = layoutRow->m_elements.at(i);
            // Only draw the NuklearWindowElement if it's enabled.
            if (ele->IsEnabled())
            {
                // Push any necessary values for the row.
                if (rowBegin != nullptr)
                {
                    nk_layout_row_push(&sstNuklear.stContext, rowBegin->m_widthsOrRatios.at(i));
                }
                else if (rowSpaceBegin != nullptr)
                {
                    orxVECTOR elementStartingPosition = rowSpaceBegin->m_elementStartingPositions.at(i);
                    orxVECTOR elementStartingSize = rowSpaceBegin->m_elementStartingSizes.at(i);
                    nk_layout_space_push(
                        &sstNuklear.stContext,
                        nk_rect(elementStartingPosition.fX, elementStartingPosition.fY, elementStartingSize.fX, elementStartingSize.fY));
                }
                else if (rowTemplateBegin != nullptr)
                {
                    switch (rowTemplateBegin->m_format)
                    {
                    case NuklearLayoutFormat::Static:
                        nk_layout_row_template_push_static(&sstNuklear.stContext, rowTemplateBegin->m_elementWidths.at(i));
                        break;
                    case NuklearLayoutFormat::Dynamic:
                        nk_layout_row_template_push_dynamic(&sstNuklear.stContext);
                        break;
                    case NuklearLayoutFormat::Variable:
                        nk_layout_row_template_push_variable(&sstNuklear.stContext, rowTemplateBegin->m_elementMinWidths.at(i));
                        break;
                    }
                }
                // Handle the widgets.
                NuklearButton *button = dynamic_cast<NuklearButton*>(ele);
                if (button != nullptr)
                {
                    if (button->m_bUsesCustomSkin && button->m_bHasText)
                    {
                        // Push the font.
                        nk_style_push_font(&sstNuklear.stContext, &sstNuklear.apstFonts[button->m_fontIndex]->handle);
                        // Set the nk_text_align flags as appropriate.
                        nk_flags buttonTextAlignmentFlags = 0;
                        if (button->m_bTextIsAlignedBottom)
                        {
                            buttonTextAlignmentFlags |= NK_TEXT_ALIGN_BOTTOM;
                        }
                        if (button->m_bTextIsAlignedCenteredHorizontal)
                        {
                            buttonTextAlignmentFlags |= NK_TEXT_ALIGN_CENTERED;
                        }
                        if (button->m_bTextIsAlignedLeft)
                        {
                            buttonTextAlignmentFlags |= NK_TEXT_ALIGN_LEFT;
                        }
                        if (button->m_bTextIsAlignedCenteredVertical)
                        {
                            buttonTextAlignmentFlags |= NK_TEXT_ALIGN_MIDDLE;
                        }
                        if (button->m_bTextIsAlignedRight)
                        {
                            buttonTextAlignmentFlags |= NK_TEXT_ALIGN_RIGHT;
                        }
                        if (button->m_bTextIsAlignedTop)
                        {
                            buttonTextAlignmentFlags |= NK_TEXT_ALIGN_TOP;
                        }

                        if (nk_button_image_text(
                            &sstNuklear.stContext,
                            sstNuklear.astSkins[button->m_skinIndex].stImage,
                            button->m_buttonText.c_str(),
                            button->m_buttonText.length(),
                            buttonTextAlignmentFlags))
                        {
                            button->Interact();
                        }

                        // Pop the font.
                        nk_style_pop_font(&sstNuklear.stContext);
                    }
                    else if (button->m_bUsesCustomSkin)
                    {
                        if (nk_button_image(&sstNuklear.stContext, sstNuklear.astSkins[button->m_skinIndex].stImage))
                        {
                            button->Interact();
                        }
                    }
                    else if (button->m_bHasText)
                    {
                        // Push the font.
                        nk_style_push_font(&sstNuklear.stContext, &sstNuklear.apstFonts[button->m_fontIndex]->handle);

                        if (nk_button_text(&sstNuklear.stContext, button->m_buttonText.c_str(), button->m_buttonText.length()))
                        {
                            button->Interact();
                        }

                        // Pop the font.
                        nk_style_pop_font(&sstNuklear.stContext);
                    }
                    else
                    {
                        if (nk_button_color(
                            &sstNuklear.stContext,
                            nk_rgba(button->m_backgroundColor.fX,
                                button->m_backgroundColor.fY,
                                button->m_backgroundColor.fZ,
                                button->m_backgroundAlpha)))
                        {
                            button->Interact();
                        }
                    }
                }
                else
                {
                    NuklearCheckBox *checkBox = dynamic_cast<NuklearCheckBox*>(ele);
                    if (checkBox != nullptr)
                    {
                        nk_style_set_font(&sstNuklear.stContext, &sstNuklear.apstFonts[checkBox->m_fontIndex]->handle);

                        if (nk_checkbox_label(&sstNuklear.stContext, checkBox->m_label.c_str(), &checkBox->m_active))
                        {
                            checkBox->Interact();
                        }
                    }
                    else
                    {
                        NuklearCombo *combo = dynamic_cast<NuklearCombo*>(ele);
                        if (combo != nullptr)
                        {
                            // Set symbol to use.
                            sstNuklear.stContext.style.combo.sym_normal = nk_symbol_type(combo->m_symbolNormal);
                            sstNuklear.stContext.style.combo.sym_hover = nk_symbol_type(combo->m_symbolHover);
                            sstNuklear.stContext.style.combo.sym_active = nk_symbol_type(combo->m_symbolActive);
                            // Set symbol background color.
                            sstNuklear.stContext.style.combo.button.normal =
                                nk_style_item_color(
                                    nk_rgba(
                                        combo->m_symbolBackgroundColorNormal.fX,
                                        combo->m_symbolBackgroundColorNormal.fY,
                                        combo->m_symbolBackgroundColorNormal.fZ,
                                        combo->m_symbolBackgroundAlphaNormal));
                            sstNuklear.stContext.style.combo.button.hover =
                                nk_style_item_color(
                                    nk_rgba(
                                        combo->m_symbolBackgroundColorHover.fX,
                                        combo->m_symbolBackgroundColorHover.fY,
                                        combo->m_symbolBackgroundColorHover.fZ,
                                        combo->m_symbolBackgroundAlphaHover));
                            sstNuklear.stContext.style.combo.button.active =
                                nk_style_item_color(
                                    nk_rgba(
                                        combo->m_symbolBackgroundColorActive.fX,
                                        combo->m_symbolBackgroundColorActive.fY,
                                        combo->m_symbolBackgroundColorActive.fZ,
                                        combo->m_symbolBackgroundAlphaActive));
                            // Set combo background color.
                            sstNuklear.stContext.style.combo.normal =
                                nk_style_item_color(
                                    nk_rgba(
                                        combo->m_backgroundColorNormal.fX,
                                        combo->m_backgroundColorNormal.fY,
                                        combo->m_backgroundColorNormal.fZ,
                                        combo->m_backgroundAlphaNormal));
                            sstNuklear.stContext.style.combo.hover =
                                nk_style_item_color(
                                    nk_rgba(
                                        combo->m_backgroundColorHover.fX,
                                        combo->m_backgroundColorHover.fY,
                                        combo->m_backgroundColorHover.fZ,
                                        combo->m_backgroundAlphaHover));
                            sstNuklear.stContext.style.combo.active =
                                nk_style_item_color(
                                    nk_rgba(
                                        combo->m_backgroundColorActive.fX,
                                        combo->m_backgroundColorActive.fY,
                                        combo->m_backgroundColorActive.fZ,
                                        combo->m_backgroundAlphaActive));
                            // Set combo text color.
                            sstNuklear.stContext.style.combo.label_normal =
                                nk_rgba(
                                    combo->m_textColorNormal.fX,
                                    combo->m_textColorNormal.fY,
                                    combo->m_textColorNormal.fZ,
                                    combo->m_textAlphaNormal);
                            sstNuklear.stContext.style.combo.label_hover =
                                nk_rgba(
                                    combo->m_textColorHover.fX,
                                    combo->m_textColorHover.fY,
                                    combo->m_textColorHover.fZ,
                                    combo->m_textAlphaHover);
                            sstNuklear.stContext.style.combo.label_active =
                                nk_rgba(
                                    combo->m_textColorActive.fX,
                                    combo->m_textColorActive.fY,
                                    combo->m_textColorActive.fZ,
                                    combo->m_textAlphaActive);
                            // Set the font.
                            nk_style_set_font(&sstNuklear.stContext, &sstNuklear.apstFonts[combo->m_fontIndex]->handle);
                            
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
                        else
                        {
                            NuklearText *text = dynamic_cast<NuklearText*>(ele);
                            if (text != nullptr)
                            {
                                nk_style_set_font(&sstNuklear.stContext, &sstNuklear.apstFonts[text->m_fontIndex]->handle);
                                if (text->m_wrap)
                                {
                                    nk_text_wrap_colored(
                                        &sstNuklear.stContext,
                                        text->m_staticContent.c_str(),
                                        text->m_staticContent.length(),
                                        nk_rgba(text->m_textColor.fR, text->m_textColor.fG, text->m_textColor.fB, text->m_textAlpha));
                                }
                                else
                                {
                                    // Set the nk_text_align flags as appropriate.
                                    nk_flags alignmentFlags = 0;
                                    if (text->m_bIsAlignedBottom)
                                    {
                                        alignmentFlags |= NK_TEXT_ALIGN_BOTTOM;
                                    }
                                    if (text->m_bIsAlignedCenteredHorizontal)
                                    {
                                        alignmentFlags |= NK_TEXT_ALIGN_CENTERED;
                                    }
                                    if (text->m_bIsAlignedLeft)
                                    {
                                        alignmentFlags |= NK_TEXT_ALIGN_LEFT;
                                    }
                                    if (text->m_bIsAlignedCenteredVertical)
                                    {
                                        alignmentFlags |= NK_TEXT_ALIGN_MIDDLE;
                                    }
                                    if (text->m_bIsAlignedRight)
                                    {
                                        alignmentFlags |= NK_TEXT_ALIGN_RIGHT;
                                    }
                                    if (text->m_bIsAlignedTop)
                                    {
                                        alignmentFlags |= NK_TEXT_ALIGN_TOP;
                                    }
                                    nk_text_colored(
                                        &sstNuklear.stContext,
                                        text->m_staticContent.c_str(),
                                        text->m_staticContent.length(),
                                        alignmentFlags,
                                        nk_rgba(text->m_textColor.fR, text->m_textColor.fG, text->m_textColor.fB, text->m_textAlpha));
                                }
                            }
                            else
                            {
                                NuklearWindow *window = dynamic_cast<NuklearWindow*>(ele);
                                if (window != nullptr)
                                {
                                    DrawNuklearWindow(window, true);
                                }
                            }
                        }
                    }
                }
                // End the rows as necessary.
                if (rowBegin != nullptr)
                {
                    nk_layout_row_end(&sstNuklear.stContext);
                }
                else if (rowSpaceBegin != nullptr)
                {
                    nk_layout_space_end(&sstNuklear.stContext);
                }
                else if (rowTemplateBegin != nullptr)
                {
                    nk_layout_row_template_end(&sstNuklear.stContext);
                }
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
