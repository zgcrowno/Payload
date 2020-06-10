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
#include "Knot.h"
#include "LevelInterfaceHomeButton.h"
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
#include "NuklearStyleButton.h"
#include "NuklearStyleCombo.h"
#include "NuklearStyleText.h"
#include "NuklearStyleTextField.h"
#include "NuklearText.h"
#include "NuklearTextField.h"
#include "NuklearTextFieldFilter.h"
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
    ScrollBindObject<Knot>("O-Knot");
    ScrollBindObject<LevelInterfaceHomeButton>("O-LevelInterfaceHomeButton");
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
    ScrollBindObject<NuklearStyleButton>("O-NuklearStyleButton");
    ScrollBindObject<NuklearStyleCombo>("O-NuklearStyleCombo");
    ScrollBindObject<NuklearStyleText>("O-NuklearStyleText");
    ScrollBindObject<NuklearStyleTextField>("O-NuklearStyleTextField");
    ScrollBindObject<NuklearText>("O-NuklearText");
    ScrollBindObject<NuklearTextField>("O-NuklearTextField");
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
            sstNuklear.stContext.style.window.fixed_background.data.color = nk_rgba(nWin->m_backgroundColor.fR, nWin->m_backgroundColor.fG, nWin->m_backgroundColor.fB, nWin->m_backgroundAlpha);
            sstNuklear.stContext.style.window.border_color = nk_rgba(nWin->m_borderColor.fR, nWin->m_borderColor.fG, nWin->m_borderColor.fB, nWin->m_borderAlpha);
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
                    // STYLE THE BUTTON.
                    // Background.
                    sstNuklear.stContext.style.button.normal =
                        nk_style_item_color(
                            nk_rgba(
                                button->m_style->m_normalColor.fR,
                                button->m_style->m_normalColor.fG,
                                button->m_style->m_normalColor.fB,
                                button->m_style->m_normalAlpha));
                    sstNuklear.stContext.style.button.hover =
                        nk_style_item_color(
                            nk_rgba(
                                button->m_style->m_hoverColor.fR,
                                button->m_style->m_hoverColor.fG,
                                button->m_style->m_hoverColor.fB,
                                button->m_style->m_hoverAlpha));
                    sstNuklear.stContext.style.button.active =
                        nk_style_item_color(
                            nk_rgba(
                                button->m_style->m_activeColor.fR,
                                button->m_style->m_activeColor.fG,
                                button->m_style->m_activeColor.fB,
                                button->m_style->m_activeAlpha));
                    sstNuklear.stContext.style.button.border_color =
                        nk_rgba(
                            button->m_style->m_borderColor.fR,
                            button->m_style->m_borderColor.fG,
                            button->m_style->m_borderColor.fB,
                            button->m_style->m_borderAlpha);
                    // Text.
                    sstNuklear.stContext.style.button.text_background =
                        nk_rgba(
                            button->m_style->m_textBackgroundColor.fR,
                            button->m_style->m_textBackgroundColor.fG,
                            button->m_style->m_textBackgroundColor.fB,
                            button->m_style->m_textBackgroundAlpha);
                    sstNuklear.stContext.style.button.text_normal =
                        nk_rgba(
                            button->m_style->m_textNormalColor.fR,
                            button->m_style->m_textNormalColor.fG,
                            button->m_style->m_textNormalColor.fB,
                            button->m_style->m_textNormalAlpha);
                    sstNuklear.stContext.style.button.text_hover =
                        nk_rgba(
                            button->m_style->m_textHoverColor.fR,
                            button->m_style->m_textHoverColor.fG,
                            button->m_style->m_textHoverColor.fB,
                            button->m_style->m_textHoverAlpha);
                    sstNuklear.stContext.style.button.text_active =
                        nk_rgba(
                            button->m_style->m_textActiveColor.fR,
                            button->m_style->m_textActiveColor.fG,
                            button->m_style->m_textActiveColor.fB,
                            button->m_style->m_textActiveAlpha);
                    // Properties
                    sstNuklear.stContext.style.button.border = button->m_style->m_borderThickness;
                    sstNuklear.stContext.style.button.rounding = button->m_style->m_borderRounding;
                    sstNuklear.stContext.style.button.padding = nk_vec2(button->m_style->m_padding.fX, button->m_style->m_padding.fY);
                    sstNuklear.stContext.style.button.image_padding = nk_vec2(button->m_style->m_imagePadding.fX, button->m_style->m_imagePadding.fY);
                    sstNuklear.stContext.style.button.touch_padding = nk_vec2(button->m_style->m_touchPadding.fX, button->m_style->m_touchPadding.fY);

                    // DRAW THE BUTTON
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
                            nk_rgba(button->m_style->m_normalColor.fR,
                                button->m_style->m_normalColor.fG,
                                button->m_style->m_normalColor.fB,
                                button->m_style->m_normalAlpha)))
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
                            // STYLE THE COMBO.
                            // Background.
                            sstNuklear.stContext.style.combo.normal =
                                nk_style_item_color(
                                    nk_rgba(
                                        combo->m_style->m_normalColor.fR,
                                        combo->m_style->m_normalColor.fG,
                                        combo->m_style->m_normalColor.fB,
                                        combo->m_style->m_normalAlpha));
                            sstNuklear.stContext.style.combo.hover =
                                nk_style_item_color(
                                    nk_rgba(
                                        combo->m_style->m_hoverColor.fR,
                                        combo->m_style->m_hoverColor.fG,
                                        combo->m_style->m_hoverColor.fB,
                                        combo->m_style->m_hoverAlpha));
                            sstNuklear.stContext.style.combo.active =
                                nk_style_item_color(
                                    nk_rgba(
                                        combo->m_style->m_activeColor.fR,
                                        combo->m_style->m_activeColor.fG,
                                        combo->m_style->m_activeColor.fB,
                                        combo->m_style->m_activeAlpha));
                            sstNuklear.stContext.style.combo.border_color =
                                nk_rgba(
                                    combo->m_style->m_borderColor.fR,
                                    combo->m_style->m_borderColor.fG,
                                    combo->m_style->m_borderColor.fB,
                                    combo->m_style->m_borderAlpha);
                            // Label.
                            sstNuklear.stContext.style.combo.label_normal =
                                nk_rgba(
                                    combo->m_style->m_labelNormalColor.fR,
                                    combo->m_style->m_labelNormalColor.fG,
                                    combo->m_style->m_labelNormalColor.fB,
                                    combo->m_style->m_labelNormalAlpha);
                            sstNuklear.stContext.style.combo.label_hover =
                                nk_rgba(
                                    combo->m_style->m_labelHoverColor.fR,
                                    combo->m_style->m_labelHoverColor.fG,
                                    combo->m_style->m_labelHoverColor.fB,
                                    combo->m_style->m_labelHoverAlpha);
                            sstNuklear.stContext.style.combo.label_active =
                                nk_rgba(
                                    combo->m_style->m_labelActiveColor.fR,
                                    combo->m_style->m_labelActiveColor.fG,
                                    combo->m_style->m_labelActiveColor.fB,
                                    combo->m_style->m_labelActiveAlpha);
                            // Symbol.
                            sstNuklear.stContext.style.combo.symbol_normal =
                                nk_rgba(
                                    combo->m_style->m_symbolNormalColor.fR,
                                    combo->m_style->m_symbolNormalColor.fG,
                                    combo->m_style->m_symbolNormalColor.fB,
                                    combo->m_style->m_symbolNormalAlpha);
                            sstNuklear.stContext.style.combo.symbol_hover =
                                nk_rgba(
                                    combo->m_style->m_symbolHoverColor.fR,
                                    combo->m_style->m_symbolHoverColor.fG,
                                    combo->m_style->m_symbolHoverColor.fB,
                                    combo->m_style->m_symbolHoverAlpha);
                            sstNuklear.stContext.style.combo.symbol_active =
                                nk_rgba(
                                    combo->m_style->m_symbolActiveColor.fR,
                                    combo->m_style->m_symbolActiveColor.fG,
                                    combo->m_style->m_symbolActiveColor.fB,
                                    combo->m_style->m_symbolActiveAlpha);
                            // Button.
                            struct nk_style_button comboButtonStyle;
                            comboButtonStyle.normal =
                                nk_style_item_color(
                                    nk_rgba(
                                        combo->m_style->m_buttonStyle->m_normalColor.fR,
                                        combo->m_style->m_buttonStyle->m_normalColor.fG,
                                        combo->m_style->m_buttonStyle->m_normalColor.fB,
                                        combo->m_style->m_buttonStyle->m_normalAlpha));
                            comboButtonStyle.hover =
                                nk_style_item_color(
                                    nk_rgba(
                                        combo->m_style->m_buttonStyle->m_hoverColor.fR,
                                        combo->m_style->m_buttonStyle->m_hoverColor.fG,
                                        combo->m_style->m_buttonStyle->m_hoverColor.fB,
                                        combo->m_style->m_buttonStyle->m_hoverAlpha));
                            comboButtonStyle.active =
                                nk_style_item_color(
                                    nk_rgba(
                                        combo->m_style->m_buttonStyle->m_activeColor.fR,
                                        combo->m_style->m_buttonStyle->m_activeColor.fG,
                                        combo->m_style->m_buttonStyle->m_activeColor.fB,
                                        combo->m_style->m_buttonStyle->m_activeAlpha));
                            comboButtonStyle.border_color =
                                nk_rgba(
                                    combo->m_style->m_buttonStyle->m_borderColor.fR,
                                    combo->m_style->m_buttonStyle->m_borderColor.fG,
                                    combo->m_style->m_buttonStyle->m_borderColor.fB,
                                    combo->m_style->m_buttonStyle->m_borderAlpha);
                            comboButtonStyle.text_background =
                                nk_rgba(
                                    combo->m_style->m_buttonStyle->m_textBackgroundColor.fR,
                                    combo->m_style->m_buttonStyle->m_textBackgroundColor.fG,
                                    combo->m_style->m_buttonStyle->m_textBackgroundColor.fB,
                                    combo->m_style->m_buttonStyle->m_textBackgroundAlpha);
                            comboButtonStyle.text_normal =
                                nk_rgba(
                                    combo->m_style->m_buttonStyle->m_textNormalColor.fR,
                                    combo->m_style->m_buttonStyle->m_textNormalColor.fG,
                                    combo->m_style->m_buttonStyle->m_textNormalColor.fB,
                                    combo->m_style->m_buttonStyle->m_textNormalAlpha);
                            comboButtonStyle.text_hover =
                                nk_rgba(
                                    combo->m_style->m_buttonStyle->m_textHoverColor.fR,
                                    combo->m_style->m_buttonStyle->m_textHoverColor.fG,
                                    combo->m_style->m_buttonStyle->m_textHoverColor.fB,
                                    combo->m_style->m_buttonStyle->m_textHoverAlpha);
                            comboButtonStyle.text_active =
                                nk_rgba(
                                    combo->m_style->m_buttonStyle->m_textActiveColor.fR,
                                    combo->m_style->m_buttonStyle->m_textActiveColor.fG,
                                    combo->m_style->m_buttonStyle->m_textActiveColor.fB,
                                    combo->m_style->m_buttonStyle->m_textActiveAlpha);
                            comboButtonStyle.border = combo->m_style->m_buttonStyle->m_borderThickness;
                            comboButtonStyle.rounding = combo->m_style->m_buttonStyle->m_borderRounding;
                            comboButtonStyle.padding = nk_vec2(combo->m_style->m_buttonStyle->m_padding.fX, combo->m_style->m_buttonStyle->m_padding.fY);
                            comboButtonStyle.image_padding = nk_vec2(combo->m_style->m_buttonStyle->m_imagePadding.fX, combo->m_style->m_buttonStyle->m_imagePadding.fY);
                            comboButtonStyle.touch_padding = nk_vec2(combo->m_style->m_buttonStyle->m_touchPadding.fX, combo->m_style->m_buttonStyle->m_touchPadding.fY);
                            sstNuklear.stContext.style.combo.button = comboButtonStyle;
                            sstNuklear.stContext.style.combo.sym_normal = nk_symbol_type(combo->m_style->m_symbolNormal);
                            sstNuklear.stContext.style.combo.sym_hover = nk_symbol_type(combo->m_style->m_symbolHover);
                            sstNuklear.stContext.style.combo.sym_active = nk_symbol_type(combo->m_style->m_symbolActive);
                            // Properties.
                            sstNuklear.stContext.style.combo.border = combo->m_style->m_borderThickness;
                            sstNuklear.stContext.style.combo.rounding = combo->m_style->m_borderRounding;
                            sstNuklear.stContext.style.combo.content_padding = { combo->m_style->m_contentPadding.fX, combo->m_style->m_contentPadding.fY };
                            sstNuklear.stContext.style.combo.button_padding = { combo->m_style->m_buttonPadding.fX, combo->m_style->m_buttonPadding.fY };
                            sstNuklear.stContext.style.combo.spacing = { combo->m_style->m_spacing.fX, combo->m_style->m_spacing.fY };
                            // Set the font.
                            nk_style_set_font(&sstNuklear.stContext, &sstNuklear.apstFonts[combo->m_fontIndex]->handle);

                            // DRAW THE COMBO.
                            if (combo->m_bUsesCustomSkin)
                            {
                                if (nk_combo_begin_image(
                                    &sstNuklear.stContext,
                                    sstNuklear.astSkins[combo->m_skinIndex].stImage,
                                    { combo->m_comboSize.fX, combo->m_comboSize.fY }))
                                {
                                    combo->Interact();

                                    nk_combo_end(&sstNuklear.stContext);
                                }
                            }
                            else
                            {
                                /*if (nk_combo(
                                    &sstNuklear.stContext,
                                    combo->m_elements.data(),
                                    combo->m_elements.size(),
                                    combo->m_selectedIndex,
                                    combo->m_elementHeight,
                                    { combo->m_comboSize.fX, combo->m_comboSize.fY }))
                                {
                                    combo->Interact();
                                }*/
                                ///////////////////////////////////////////////////////////////////////////
                                struct nk_context *ctx = &sstNuklear.stContext;
                                const char **items = combo->m_elements.data();
                                int count = combo->m_elements.size();
                                int selected = combo->m_selectedIndex;
                                int item_height = combo->m_elementHeight;
                                struct nk_vec2 size = { combo->m_comboSize.fX, combo->m_comboSize.fY };

                                int i = 0;
                                int max_height;
                                struct nk_vec2 item_spacing;
                                struct nk_vec2 window_padding;

                                item_spacing = ctx->style.window.spacing;
                                window_padding = nk_panel_get_padding(&ctx->style, ctx->current->layout->type);
                                max_height = count * item_height + count * (int)item_spacing.y;
                                max_height += (int)item_spacing.y * 2 + (int)window_padding.y * 2;
                                size.y = NK_MIN(size.y, (float)max_height);

                                const struct nk_input *in;
                                struct nk_window *win;
                                struct nk_style *style;

                                enum nk_widget_layout_states s;
                                int is_clicked = nk_false;
                                struct nk_rect header;
                                const struct nk_style_item *background;
                                struct nk_text text;

                                win = ctx->current;
                                style = &ctx->style;
                                s = nk_widget(&header, ctx);

                                in = (win->layout->flags & NK_WINDOW_ROM || s == NK_WIDGET_ROM) ? 0 : &ctx->input;
                                if (nk_button_behavior(&ctx->last_widget_state, header, in, NK_BUTTON_DEFAULT))
                                    is_clicked = nk_true;

                                /* draw combo box header background and border */
                                if (ctx->last_widget_state & NK_WIDGET_STATE_ACTIVED) {
                                    background = &style->combo.active;
                                    text.text = style->combo.label_active;
                                }
                                else if (ctx->last_widget_state & NK_WIDGET_STATE_HOVER) {
                                    background = &style->combo.hover;
                                    text.text = style->combo.label_hover;
                                }
                                else {
                                    background = &style->combo.normal;
                                    text.text = style->combo.label_normal;
                                }
                                if (background->type == NK_STYLE_ITEM_IMAGE) {
                                    text.background = nk_rgba(0, 0, 0, 0);
                                    nk_draw_image(&win->buffer, header, &background->data.image, nk_white);
                                }
                                else {
                                    text.background = background->data.color;
                                    nk_fill_rect(&win->buffer, header, style->combo.rounding, background->data.color);
                                    nk_stroke_rect(&win->buffer, header, style->combo.rounding, style->combo.border, style->combo.border_color);
                                }
                                {
                                    /* print currently selected text item */
                                    struct nk_rect label;
                                    struct nk_rect button;
                                    struct nk_rect content;

                                    enum nk_symbol_type sym;
                                    if (ctx->last_widget_state & NK_WIDGET_STATE_HOVER)
                                        sym = style->combo.sym_hover;
                                    else if (is_clicked)
                                        sym = style->combo.sym_active;
                                    else
                                        sym = style->combo.sym_normal;

                                    /* represents whether or not the combo's button symbol should be drawn */
                                    int draw_button_symbol = sym != NK_SYMBOL_NONE;

                                    /* calculate button */
                                    button.w = header.h - 2 * style->combo.button_padding.y;
                                    button.x = (header.x + header.w - header.h) - style->combo.button_padding.x;
                                    button.y = header.y + style->combo.button_padding.y;
                                    button.h = button.w;

                                    content.x = button.x + style->combo.button.padding.x;
                                    content.y = button.y + style->combo.button.padding.y;
                                    content.w = button.w - 2 * style->combo.button.padding.x;
                                    content.h = button.h - 2 * style->combo.button.padding.y;

                                    /* draw selected label */
                                    text.padding = nk_vec2(0, 0);
                                    label.x = header.x + style->combo.content_padding.x;
                                    label.y = header.y + style->combo.content_padding.y;
                                    if (draw_button_symbol)
                                        label.w = button.x - (style->combo.content_padding.x + style->combo.spacing.x) - label.x;
                                    else
                                        label.w = header.w - 2 * style->combo.content_padding.x;
                                    label.h = header.h - 2 * style->combo.content_padding.y;
                                    nk_widget_text(&win->buffer, label, items[selected], nk_strlen(items[selected]), &text,
                                        NK_TEXT_LEFT, ctx->style.font);

                                    /* draw open/close button */
                                    if (draw_button_symbol)
                                        nk_draw_button_symbol(&win->buffer, &button, &content, ctx->last_widget_state,
                                            &ctx->style.combo.button, sym, style->font);
                                }
                                int nkComboBegin = nk_combo_begin(ctx, win, size, is_clicked, header);
                                if (nkComboBegin)
                                {
                                    nk_layout_row_dynamic(ctx, (float)item_height, 1);
                                    for (i = 0; i < count; ++i) {
                                        if (nk_combo_item_label(ctx, items[i], NK_TEXT_LEFT))
                                            selected = i;
                                    }
                                    nk_combo_end(ctx);
                                }
                                if (selected)
                                {
                                    combo->Interact();
                                }
                                ///////////////////////////////////////////////////////////////
                            }
                        }
                        else
                        {
                            NuklearText *text = dynamic_cast<NuklearText*>(ele);
                            if (text != nullptr)
                            {
                                // STYLE THE TEXT.
                                sstNuklear.stContext.style.text.color =
                                    nk_rgba(
                                        text->m_style->m_textColor.fR,
                                        text->m_style->m_textColor.fG,
                                        text->m_style->m_textColor.fB,
                                        text->m_style->m_textAlpha);
                                sstNuklear.stContext.style.text.padding = { text->m_style->m_padding.fX, text->m_style->m_padding.fY };
                                nk_style_set_font(&sstNuklear.stContext, &sstNuklear.apstFonts[text->m_fontIndex]->handle);

                                // DRAW THE TEXT.
                                if (text->m_wrap)
                                {
                                    nk_text_wrap_colored(
                                        &sstNuklear.stContext,
                                        text->m_staticContent.c_str(),
                                        text->m_staticContent.length(),
                                        nk_rgba(text->m_style->m_textColor.fR, text->m_style->m_textColor.fG, text->m_style->m_textColor.fB, text->m_style->m_textAlpha));
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
                                        nk_rgba(text->m_style->m_textColor.fR, text->m_style->m_textColor.fG, text->m_style->m_textColor.fB, text->m_style->m_textAlpha));
                                }
                            }
                            else
                            {
                                NuklearTextField *textField = dynamic_cast<NuklearTextField*>(ele);
                                if (textField != nullptr)
                                {
                                    // STYLE THE TEXT FIELD.
                                    // Background.
                                    sstNuklear.stContext.style.edit.normal =
                                        nk_style_item_color(
                                            nk_rgba(
                                                textField->m_style->m_normalColor.fR,
                                                textField->m_style->m_normalColor.fG,
                                                textField->m_style->m_normalColor.fB,
                                                textField->m_style->m_normalAlpha));
                                    sstNuklear.stContext.style.edit.hover =
                                        nk_style_item_color(
                                            nk_rgba(
                                                textField->m_style->m_hoverColor.fR,
                                                textField->m_style->m_hoverColor.fG,
                                                textField->m_style->m_hoverColor.fB,
                                                textField->m_style->m_hoverAlpha));
                                    sstNuklear.stContext.style.edit.active =
                                        nk_style_item_color(
                                            nk_rgba(
                                                textField->m_style->m_activeColor.fR,
                                                textField->m_style->m_activeColor.fG,
                                                textField->m_style->m_activeColor.fB,
                                                textField->m_style->m_activeAlpha));
                                    sstNuklear.stContext.style.edit.border_color =
                                        nk_rgba(
                                            textField->m_style->m_borderColor.fR,
                                            textField->m_style->m_borderColor.fG,
                                            textField->m_style->m_borderColor.fB,
                                            textField->m_style->m_borderAlpha);
                                    // Cursor.
                                    sstNuklear.stContext.style.edit.cursor_normal =
                                        nk_rgba(
                                            textField->m_style->m_cursorNormalColor.fR,
                                            textField->m_style->m_cursorNormalColor.fG,
                                            textField->m_style->m_cursorNormalColor.fB,
                                            textField->m_style->m_cursorNormalAlpha);
                                    sstNuklear.stContext.style.edit.cursor_hover =
                                        nk_rgba(
                                            textField->m_style->m_cursorHoverColor.fR,
                                            textField->m_style->m_cursorHoverColor.fG,
                                            textField->m_style->m_cursorHoverColor.fB,
                                            textField->m_style->m_cursorHoverAlpha);
                                    sstNuklear.stContext.style.edit.cursor_text_normal =
                                        nk_rgba(
                                            textField->m_style->m_cursorTextNormalColor.fR,
                                            textField->m_style->m_cursorTextNormalColor.fG,
                                            textField->m_style->m_cursorTextNormalColor.fB,
                                            textField->m_style->m_cursorTextNormalAlpha);
                                    sstNuklear.stContext.style.edit.cursor_text_hover =
                                        nk_rgba(
                                            textField->m_style->m_cursorTextHoverColor.fR,
                                            textField->m_style->m_cursorTextHoverColor.fG,
                                            textField->m_style->m_cursorTextHoverColor.fB,
                                            textField->m_style->m_cursorTextHoverAlpha);
                                    // Text (unselected).
                                    sstNuklear.stContext.style.edit.text_normal =
                                        nk_rgba(
                                            textField->m_style->m_textNormalColor.fR,
                                            textField->m_style->m_textNormalColor.fG,
                                            textField->m_style->m_textNormalColor.fB,
                                            textField->m_style->m_textNormalAlpha);
                                    sstNuklear.stContext.style.edit.text_hover =
                                        nk_rgba(
                                            textField->m_style->m_textHoverColor.fR,
                                            textField->m_style->m_textHoverColor.fG,
                                            textField->m_style->m_textHoverColor.fB,
                                            textField->m_style->m_textHoverAlpha);
                                    sstNuklear.stContext.style.edit.text_active =
                                        nk_rgba(
                                            textField->m_style->m_textActiveColor.fR,
                                            textField->m_style->m_textActiveColor.fG,
                                            textField->m_style->m_textActiveColor.fB,
                                            textField->m_style->m_textActiveAlpha);
                                    // Text (selected).
                                    sstNuklear.stContext.style.edit.selected_normal =
                                        nk_rgba(
                                            textField->m_style->m_selectedNormalColor.fR,
                                            textField->m_style->m_selectedNormalColor.fG,
                                            textField->m_style->m_selectedNormalColor.fB,
                                            textField->m_style->m_selectedNormalAlpha);
                                    sstNuklear.stContext.style.edit.selected_hover =
                                        nk_rgba(
                                            textField->m_style->m_selectedHoverColor.fR,
                                            textField->m_style->m_selectedHoverColor.fG,
                                            textField->m_style->m_selectedHoverColor.fB,
                                            textField->m_style->m_selectedHoverAlpha);
                                    sstNuklear.stContext.style.edit.selected_text_normal =
                                        nk_rgba(
                                            textField->m_style->m_selectedTextNormalColor.fR,
                                            textField->m_style->m_selectedTextNormalColor.fG,
                                            textField->m_style->m_selectedTextNormalColor.fB,
                                            textField->m_style->m_selectedTextNormalAlpha);
                                    sstNuklear.stContext.style.edit.selected_text_hover =
                                        nk_rgba(
                                            textField->m_style->m_selectedTextHoverColor.fR,
                                            textField->m_style->m_selectedTextHoverColor.fG,
                                            textField->m_style->m_selectedTextHoverColor.fB,
                                            textField->m_style->m_selectedTextHoverAlpha);
                                    // Properties.
                                    sstNuklear.stContext.style.edit.border = textField->m_style->m_borderThickness;
                                    sstNuklear.stContext.style.edit.rounding = textField->m_style->m_borderRounding;
                                    sstNuklear.stContext.style.edit.cursor_size = textField->m_style->m_cursorSize;
                                    sstNuklear.stContext.style.edit.scrollbar_size = nk_vec2(textField->m_style->m_scrollbarSize.fX, textField->m_style->m_scrollbarSize.fY);
                                    sstNuklear.stContext.style.edit.padding = nk_vec2(textField->m_style->m_padding.fX, textField->m_style->m_padding.fY);
                                    sstNuklear.stContext.style.edit.row_padding = textField->m_style->m_rowPadding;
                                    // Font.
                                    nk_style_set_font(&sstNuklear.stContext, &sstNuklear.apstFonts[textField->m_fontIndex]->handle);

                                    // DRAW THE TEXT FIELD.
                                    // Set the text field flags as appropriate.
                                    nk_flags fieldFlags = 0;
                                    if (textField->m_bReadOnly)
                                    {
                                        fieldFlags |= NK_EDIT_READ_ONLY;
                                    }
                                    if (textField->m_bAutoSelect)
                                    {
                                        fieldFlags |= NK_EDIT_AUTO_SELECT;
                                    }
                                    if (textField->m_bSigEnter)
                                    {
                                        fieldFlags |= NK_EDIT_SIG_ENTER;
                                    }
                                    if (textField->m_bAllowTab)
                                    {
                                        fieldFlags |= NK_EDIT_ALLOW_TAB;
                                    }
                                    if (textField->m_bNoCursor)
                                    {
                                        fieldFlags |= NK_EDIT_NO_CURSOR;
                                    }
                                    if (textField->m_bSelectable)
                                    {
                                        fieldFlags |= NK_EDIT_SELECTABLE;
                                    }
                                    if (textField->m_bClipboard)
                                    {
                                        fieldFlags |= NK_EDIT_CLIPBOARD;
                                    }
                                    if (textField->m_bControlEnterNewline)
                                    {
                                        fieldFlags |= NK_EDIT_CTRL_ENTER_NEWLINE;
                                    }
                                    if (textField->m_bNoHorizontalScroll)
                                    {
                                        fieldFlags |= NK_EDIT_NO_HORIZONTAL_SCROLL;
                                    }
                                    if (textField->m_bAlwaysInsertMode)
                                    {
                                        fieldFlags |= NK_EDIT_ALWAYS_INSERT_MODE;
                                    }
                                    if (textField->m_bMultiline)
                                    {
                                        fieldFlags |= NK_EDIT_MULTILINE;
                                    }
                                    if (textField->m_bGoToEndOnActivate)
                                    {
                                        fieldFlags |= NK_EDIT_GOTO_END_ON_ACTIVATE;
                                    }
                                    // Set the filter as appropriate.
                                    nk_plugin_filter textFieldFilter;
                                    switch (textField->m_filter)
                                    {
                                    case NuklearTextFieldFilter::Default:
                                        textFieldFilter = nk_filter_default;
                                        break;
                                    case NuklearTextFieldFilter::ASCII:
                                        textFieldFilter = nk_filter_ascii;
                                        break;
                                    case NuklearTextFieldFilter::Binary:
                                        textFieldFilter = nk_filter_binary;
                                        break;
                                    case NuklearTextFieldFilter::Decimal:
                                        textFieldFilter = nk_filter_decimal;
                                        break;
                                    case NuklearTextFieldFilter::Float:
                                        textFieldFilter = nk_filter_float;
                                        break;
                                    case NuklearTextFieldFilter::Hex:
                                        textFieldFilter = nk_filter_hex;
                                        break;
                                    case NuklearTextFieldFilter::Oct:
                                        textFieldFilter = nk_filter_oct;
                                        break;
                                    }
                                    // Draw.
                                    const int textFieldStatus =
                                        nk_edit_string_zero_terminated(
                                            &sstNuklear.stContext,
                                            fieldFlags,
                                            const_cast<char*>(textField->m_content.c_str()),
                                            textField->m_maxLength + 1,
                                            textFieldFilter);
                                    // Handle status.
                                    // TODO: Associate each status with appropriate behavior.
                                    switch (textFieldStatus)
                                    {
                                    case NK_EDIT_ACTIVE:
                                        break;
                                    case NK_EDIT_INACTIVE:
                                        break;
                                    case NK_EDIT_ACTIVATED:
                                        break;
                                    case NK_EDIT_DEACTIVATED:
                                        break;
                                    case NK_EDIT_COMMITED:
                                        break;
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
