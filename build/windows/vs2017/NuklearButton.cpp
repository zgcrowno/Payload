#include "NuklearButton.h"

using namespace payload;

void NuklearButton::OnCreate()
{
    NuklearInteractable::OnCreate();

    m_textureBitmap = orxTexture_GetBitmap(orxTexture_CreateFromFile(GetString("TexturePath", GetModelName()).c_str(), true));
}

void NuklearButton::OnDelete()
{

}

orxBOOL NuklearButton::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void NuklearButton::Update(const orxCLOCK_INFO &_rstInfo)
{

}

void NuklearButton::Interact()
{

}
