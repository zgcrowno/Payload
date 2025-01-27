#include "Dashboard.h"

using namespace payload;

void Dashboard::OnCreate()
{
    m_animStartTimeLeafBunch = GetFloat("AnimStartTimeLeafBunch", GetModelName());
    m_animEndTimeLeafBunch = GetFloat("AnimEndTimeLeafBunch", GetModelName());
    m_animStartTimeMoon = GetFloat("AnimStartTimeMoon", GetModelName());
    m_animEndTimeMoon = GetFloat("AnimEndTimeMoon", GetModelName());
    m_animStartTimeScroll = GetFloat("AnimStartTimeScroll", GetModelName());
    m_animEndTimeScroll = GetFloat("AnimEndTimeScroll", GetModelName());
    m_startingPosition = GetVector("StartingPosition", GetModelName());
    m_endingPosition = GetVector("EndingPosition", GetModelName());
    m_minLeafBunchScale = GetFloat("MinLeafBunchScale", GetModelName());
    m_maxLeafBunchScale = GetFloat("MaxLeafBunchScale", GetModelName());
    m_moon = static_cast<Moon*>(GetChildByName("O-Moon"));
    for (int i = 0; i < GetListCount("LeafBunchCoordinates", GetModelName()); i++)
    {
        orxVECTOR coord = GetListVector("LeafBunchCoordinates", i, GetModelName());
        ScrollMod *leafBunch = CreateChild("O-LeafBunch");
        leafBunch->SetPosition({ coord.fX, coord.fY, leafBunch->GetPosition().fZ });
        m_leafBunches.push_back(leafBunch);
    }
    for (ScrollObject *child = GetOwnedChild(); child != nullptr; child = child->GetOwnedSibling())
    {
        if (orxString_SearchString(child->GetModelName(), "O-Knot") != nullptr)
        {
            m_knots.push_back(static_cast<Knot*>(child));
        }
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
    
}

void Dashboard::GrowLeafBunches()
{
    float activeTime = GetActiveTime();

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

void Dashboard::PhaseMoon()
{
    float activeTime = GetActiveTime();
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

void Dashboard::Scroll()
{
    float activeTime = GetActiveTime();
    orxVECTOR curPos = GetPosition();

    float newY = orxCLAMP(
        orxLERP(m_startingPosition.fY,
            m_endingPosition.fY,
            (activeTime - m_animStartTimeScroll) / (m_animEndTimeScroll - m_animStartTimeScroll)),
        m_endingPosition.fY,
        m_startingPosition.fY);
    SetPosition({ curPos.fX, newY, curPos.fZ });
}

void Dashboard::FinalizeLeafBunches()
{
    for (ScrollMod *leafBunch : m_leafBunches)
    {
        leafBunch->SetScale({ m_maxLeafBunchScale, m_maxLeafBunchScale, leafBunch->GetScale().fZ });
    }
}

void Dashboard::FinalizeMoon()
{
    m_moon->m_mutableCircleX = 1.0f - m_moon->m_mutableCircleXStart;
}

void Dashboard::FinalizeScroll()
{
    orxVECTOR curPos = GetPosition();
    SetPosition({ curPos.fX, m_endingPosition.fY, curPos.fZ });
}
