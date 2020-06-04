#include "Knot.h"

using namespace payload;

void Knot::OnCreate()
{
    m_bIsUnlocked = GetBool("IsUnlocked", GetModelName());
    m_sceneTransition = GetString("SceneTransition", GetModelName());
}

void Knot::OnDelete()
{

}

orxBOOL Knot::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void Knot::Update(const orxCLOCK_INFO &_rstInfo)
{
    if (orxInput_HasBeenActivated("LeftClick"))
    {
        orxVECTOR mouseScreenPos;
        orxVECTOR mouseWorldPos;
        orxMouse_GetPosition(&mouseScreenPos);
        orxRender_GetWorldPosition(&mouseScreenPos, nullptr, &mouseWorldPos);
        ScrollObject *picked = Payload::GetInstance().PickObject(mouseWorldPos, orxString_GetID("knot"));
        if (picked != nullptr)
        {
            CreateObject(m_sceneTransition);
        }
    }
}
