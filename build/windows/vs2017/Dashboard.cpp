#include "Dashboard.h"
#include <iostream>

using namespace payload;

void Dashboard::OnCreate()
{
    m_minLeafBunchScale = GetFloat("MinLeafBunchScale", GetModelName());
    m_maxLeafBunchScale = GetFloat("MaxLeafBunchScale", GetModelName());
    m_timeToGrowLeaves = GetFloat("TimeToGrowLeaves", GetModelName());
    for (int i = 0; i < GetListCount("LeafBunchCoordinates", GetModelName()); i++)
    {
        orxVECTOR coord = GetListVector("LeafBunchCoordinates", i, GetModelName());
        ScrollMod *leafBunch = CreateObject("O-LeafBunch");
        leafBunch->SetPosition({ coord.fX, coord.fY, leafBunch->GetPosition().fZ });
        leafBunch->SetParent(this);
        m_leafBunches.push_back(leafBunch);
    }
}

void Dashboard::OnDelete()
{
    
}

orxBOOL Dashboard::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void Dashboard::Update(const orxCLOCK_INFO &_rstInfo)
{
    // Grow the leaf bunches.
    for (ScrollMod *leafBunch : m_leafBunches)
    {
        float newScaleXY = orxCLAMP(
                               orxLERP(m_minLeafBunchScale,
                                       m_maxLeafBunchScale,
                                       GetActiveTime() / m_timeToGrowLeaves),
                               m_minLeafBunchScale,
                               m_maxLeafBunchScale);
        leafBunch->SetScale({ newScaleXY, newScaleXY, leafBunch->GetScale().fZ });
    }
}
