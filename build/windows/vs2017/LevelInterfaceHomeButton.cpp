#include "LevelInterfaceHomeButton.h"

using namespace payload;

void LevelInterfaceHomeButton::OnCreate()
{
    NuklearButton::OnCreate();
}

void LevelInterfaceHomeButton::OnDelete()
{

}

orxBOOL LevelInterfaceHomeButton::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void LevelInterfaceHomeButton::Update(const orxCLOCK_INFO &_rstInfo)
{

}

void LevelInterfaceHomeButton::Interact()
{
    SetBool("SkipIntro", true, "O-SceneDashboard");
    CreateObject("O-ToSceneDashboard");
}
