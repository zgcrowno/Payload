#include "TileInhabitant.h"

using namespace payload;

void TileInhabitant::OnCreate()
{
    m_precedence = GetU32("Precedence", GetModelName());
    m_tileRatio = GetU32("TileRatio", GetModelName());
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
    
}

void TileInhabitant::Cohabitate(TileInhabitant *_cohabitant)
{

}
