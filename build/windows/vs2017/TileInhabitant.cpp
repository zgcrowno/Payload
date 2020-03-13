#include "TileInhabitant.h"

using namespace payload;

void TileInhabitant::OnCreate()
{
    m_precedence = GetU32("Precedence", GetModelName());
    m_tileRatio = GetFloat("TileRatio", GetModelName());
}

void TileInhabitant::OnDelete()
{

}

orxBOOL TileInhabitant::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void TileInhabitant::Update(const orxCLOCK_INFO &_rstInfo)
{
    // TARGET SCALING
    if (m_target != nullptr)
    {
        float minAxis = orxMIN(m_target->m_visualScale.fX, m_target->m_visualScale.fY);
        float scaleRatio = minAxis * m_tileRatio;
        orxVECTOR targetScale = m_target->GetScale();
        SetScale({ targetScale.fX * scaleRatio, targetScale.fY * scaleRatio });
    }
}

void TileInhabitant::Cohabitate(TileInhabitant *_cohabitant)
{

}
