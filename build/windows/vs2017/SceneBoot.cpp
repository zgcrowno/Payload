#include "SceneBoot.h"

using namespace payload;

void SceneBoot::OnCreate()
{
    m_bootGreeting = static_cast<NuklearWindow*>(GetChildByName("O-BootGreeting"));
    m_nextSceneTransition = GetString("NextSceneTransition", GetModelName());
}

void SceneBoot::OnDelete()
{

}

orxBOOL SceneBoot::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void SceneBoot::Update(const orxCLOCK_INFO &_rstInfo)
{
    Scene::Update(_rstInfo);
}

void SceneBoot::Skip()
{
    CreateObject(m_nextSceneTransition);
}
