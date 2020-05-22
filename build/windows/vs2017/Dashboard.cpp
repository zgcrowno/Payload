#include "Dashboard.h"

using namespace payload;

void Dashboard::OnCreate()
{
    m_animStartTimeLeafBunch = GetFloat("AnimStartTimeLeafBunch", GetModelName());
    m_animEndTimeLeafBunch = GetFloat("AnimEndTimeLeafBunch", GetModelName());
    m_animStartTimeMoon = GetFloat("AnimStartTimeMoon", GetModelName());
    m_animEndTimeMoon = GetFloat("AnimEndTimeMoon", GetModelName());

    m_minLeafBunchScale = GetFloat("MinLeafBunchScale", GetModelName());
    m_maxLeafBunchScale = GetFloat("MaxLeafBunchScale", GetModelName());
    m_moon = static_cast<Moon*>(GetChildByName("O-Moon"));
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
    float activeTime = GetActiveTime();

    // Grow the leaf bunches.
    if (activeTime >= m_animStartTimeLeafBunch && activeTime <= m_animEndTimeLeafBunch)
    {
        for (ScrollMod *leafBunch : m_leafBunches)
        {
            float newScaleXY = orxCLAMP(
                orxLERP(m_minLeafBunchScale,
                    m_maxLeafBunchScale,
                    (activeTime - m_animStartTimeLeafBunch) / (m_animEndTimeLeafBunch - m_animStartTimeLeafBunch)),
                m_minLeafBunchScale,
                m_maxLeafBunchScale);
            leafBunch->SetScale({ newScaleXY, newScaleXY, leafBunch->GetScale().fZ });
        }
    }
    // Phase the moon.
    if (activeTime >= m_animStartTimeMoon && activeTime <= m_animEndTimeMoon)
    {
        m_moon->m_waning = (activeTime - m_animStartTimeMoon) > (m_animEndTimeMoon - m_animStartTimeMoon) / 2.0f;
        if (!m_moon->m_waning)
        {
            m_moon->m_mutableCircleX = orxCLAMP(
                orxLERP(
                    m_moon->m_mutableCircleXStart,
                    0.0f,
                    ((activeTime - m_animStartTimeMoon) / (m_animEndTimeMoon - m_animStartTimeMoon)) * 2.0f),
                0.0f,
                1.0f);
        }
        else
        {
            m_moon->m_mutableCircleX = orxCLAMP(
                orxLERP(
                    1.0f,
                    1.0f - m_moon->m_mutableCircleXStart,
                    ((activeTime - m_animStartTimeMoon - ((m_animEndTimeMoon - m_animStartTimeMoon) / 2.0f)) / (m_animEndTimeMoon - m_animStartTimeMoon)) * 2.0f),
                0.0f,
                1.0f);
        }
    }
}
