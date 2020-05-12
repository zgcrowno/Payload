#pragma once

// Nuklear definitions.
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_IMPLEMENTATION

#include "Payload.h"
#include "nuklear.h"

namespace payload
{
    class NuklearUtil
    {
    private:
        //! Static Nuklear module data.
        typedef struct _nuklearModuleData
        {
            //! Current font texture
            orxBITMAP *                 pstFontTexture;
            //! clock used to check wheel delta
            orxCLOCK *                  pstMainClock;
            //! Draw commands buffer
            struct nk_buffer            stCommands;
            //! Null texture
            struct nk_draw_null_texture stNullTexture;
            //! Nuklear context
            struct nk_context           stContext;
            //! Nuklear font atlas
            struct nk_font_atlas        stFontAtlas;
            //! Last button pressed
            unsigned long               ulLastButtonClick;
            //! Mouse wheel info
            struct nk_vec2              stWheel;
        } nuklearModuleData;
    public:
        //! Global Nuklear module data instance.
        static nuklearModuleData gModuleData;

        static struct nk_context *Init();
        static void FontStashBegin(struct nk_font_atlas **atlas);
        static void FontStashEnd();
        static void orxFASTCALL MouseWheelUpdate(const orxCLOCK_INFO *_pstClockInfo, void *_pContext);
        static orxBITMAP *UploadFontBitmap(const void *image, int width, int height);
        static void Render(enum nk_anti_aliasing eAntiAliasing);
    };
}
