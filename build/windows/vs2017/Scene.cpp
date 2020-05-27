#include "Scene.h"

using namespace payload;

void Scene::OnCreate()
{
    
}

void Scene::OnDelete()
{
    
}

orxBOOL Scene::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void Scene::Update(const orxCLOCK_INFO &_rstInfo)
{
    if (!m_bUnskippable && orxInput_HasBeenActivated("Recon"))
    {
        Skip();
    }
}

void Scene::Skip()
{

}
