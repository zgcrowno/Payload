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

#include "NuklearUtil.h"

using namespace payload;

struct nk_context *NuklearUtil::Init()
{
    /* clear the module data */
    memset(&gModuleData, 0, sizeof(nuklearModuleData));

    /* initialize commands buffer */
    nk_buffer_init_default(&gModuleData.stCommands);

    /* initialize default context */
    nk_init_default(&gModuleData.stContext, 0);

    /* install orx clock callback to update mouse wheel status */
    gModuleData.pstMainClock = orxClock_FindFirst(orx2F(-1.0f), orxCLOCK_TYPE_CORE);
    if (gModuleData.pstMainClock != orxNULL)
        orxClock_Register(gModuleData.pstMainClock, MouseWheelUpdate, orxNULL, orxMODULE_ID_INPUT, orxCLOCK_PRIORITY_HIGH);

    return &gModuleData.stContext;
}

void NuklearUtil::FontStashBegin(struct nk_font_atlas **atlas)
{
    nk_font_atlas_init_default(&gModuleData.stFontAtlas);
    nk_font_atlas_begin(&gModuleData.stFontAtlas);
    *atlas = &gModuleData.stFontAtlas;
}

void NuklearUtil::FontStashEnd()
{
    const void * image;
    int imageWidth = 0;
    int imageHeight = 0;

    /* create font atlas bake */
    image = nk_font_atlas_bake(&gModuleData.stFontAtlas, &imageWidth, &imageHeight, NK_FONT_ATLAS_RGBA32);

    /* upload the bitmap to the device */
    gModuleData.pstFontTexture = UploadFontBitmap(image, imageWidth, imageHeight);

    /* end of font config */
    nk_font_atlas_end(&gModuleData.stFontAtlas, nk_handle_ptr((void *)gModuleData.pstFontTexture), &gModuleData.stNullTexture);

    /* set the font as default font */
    if (gModuleData.stFontAtlas.default_font)
        nk_style_set_font(&gModuleData.stContext, &gModuleData.stFontAtlas.default_font->handle);
}

void orxFASTCALL NuklearUtil::MouseWheelUpdate(const orxCLOCK_INFO *_pstClockInfo, void *_pContext)
{
    orxFLOAT wheel_delta = orxMouse_GetWheelDelta();
    if (wheel_delta != orxFLOAT_0)
        gModuleData.stWheel = nk_vec2(0, wheel_delta);
}

orxBITMAP *NuklearUtil::UploadFontBitmap(const void *image, int width, int height)
{
    orxTEXTURE *pstTexture;
    orxBITMAP  *pstBitmap;

    // Creates background texture
    pstTexture = orxTexture_Create();
    pstBitmap = orxDisplay_CreateBitmap(width, height);
    orxDisplay_SetBitmapData(pstBitmap, (orxU8 *)image, width * height * sizeof(orxRGBA));
    orxTexture_LinkBitmap(pstTexture, pstBitmap, "Texture", orxTRUE);

    return pstBitmap;
}

void NuklearUtil::Render(enum nk_anti_aliasing eAntiAliasing)
{
    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    struct nk_vec2 fb_scale = { 1, 1 };

    /* get screen size */
    orxFLOAT display_w, display_h;
    orxDisplay_GetScreenSize(&display_w, &display_h);

    // Gets screen bitmap
    orxBITMAP * pstScreen = orxDisplay_GetScreenBitmap();
    // Restores screen as destination bitmap
    orxDisplay_SetDestinationBitmaps(&pstScreen, 1);
    // Restores screen bitmap clipping
    orxDisplay_SetBitmapClipping(orxDisplay_GetScreenBitmap(), 0, 0, orxF2U(display_w), orxF2U(display_h));

    orxU32 vs = sizeof(orxDISPLAY_VERTEX);
    size_t vp = offsetof(orxDISPLAY_VERTEX, fX);
    size_t vt = offsetof(orxDISPLAY_VERTEX, fU);
    size_t vc = offsetof(orxDISPLAY_VERTEX, stRGBA);

    /* convert from command queue into draw list and draw to screen */

    /* fill convert configuration */
    nk_convert_config config;
    static const nk_draw_vertex_layout_element vertex_layout[] = {
        { NK_VERTEX_POSITION, NK_FORMAT_FLOAT, NK_OFFSETOF(orxDISPLAY_VERTEX, fX) },
        { NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, NK_OFFSETOF(orxDISPLAY_VERTEX, fU) },
        { NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, NK_OFFSETOF(orxDISPLAY_VERTEX, stRGBA) },
        { NK_VERTEX_LAYOUT_END }
    };

    NK_MEMSET(&config, 0, sizeof(config));
    config.vertex_layout = vertex_layout;
    config.vertex_size = sizeof(orxDISPLAY_VERTEX);
    config.vertex_alignment = NK_ALIGNOF(orxDISPLAY_VERTEX);
    config.null = gModuleData.stNullTexture;
    config.circle_segment_count = 22;
    config.curve_segment_count = 22;
    config.arc_segment_count = 22;
    config.global_alpha = 1.0f;
    config.shape_AA = eAntiAliasing;
    config.line_AA = eAntiAliasing;

    nk_buffer vbuf, ebuf;
    /* convert shapes into vertexes */
    nk_buffer_init_default(&vbuf);
    nk_buffer_init_default(&ebuf);
    nk_convert(&gModuleData.stContext, &gModuleData.stCommands, &vbuf, &ebuf, &config);

    /* setup vertex buffer pointer */
    const void *vertices = nk_buffer_memory_const(&vbuf);
    const void *elements = nk_buffer_memory_const(&ebuf);

    const nk_draw_command *cmd;
    /* iterate over and execute each draw command */
    const nk_draw_index * offset = (const nk_draw_index*)nk_buffer_memory_const(&ebuf);
    nk_draw_foreach(cmd, &gModuleData.stContext, &gModuleData.stCommands)
    {
        if (!cmd->elem_count)
            continue;

        orxDISPLAY_MESH stMesh = {};
        stMesh.ePrimitive = orxDISPLAY_PRIMITIVE_TRIANGLES;
        stMesh.astVertexList = (orxDISPLAY_VERTEX *)((nk_byte*)vertices + vp);
        stMesh.u32VertexNumber = gModuleData.stContext.draw_list.vertex_count;
        stMesh.au16IndexList = (orxU16 *)offset;
        stMesh.u32IndexNumber = cmd->elem_count;
        orxDisplay_SetBitmapClipping(orxNULL, (orxU32)cmd->clip_rect.x, (orxU32)cmd->clip_rect.y, (orxU32)(cmd->clip_rect.x + cmd->clip_rect.w), (orxU32)(cmd->clip_rect.y + cmd->clip_rect.h));
        orxDisplay_DrawMesh(&stMesh, (orxBITMAP *)cmd->texture.ptr, orxDISPLAY_SMOOTHING_OFF, orxDISPLAY_BLEND_MODE_ALPHA);

        offset += cmd->elem_count;
    }

    nk_clear(&gModuleData.stContext);
    nk_buffer_free(&vbuf);
    nk_buffer_free(&ebuf);
}
