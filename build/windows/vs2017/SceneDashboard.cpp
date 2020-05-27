#include "SceneDashboard.h"

using namespace payload;

void SceneDashboard::OnCreate()
{
    m_dashboard = static_cast<Dashboard*>(GetChildByName("O-Dashboard"));
    m_dashboardHeader = static_cast<NuklearWindow*>(GetChildByName("O-DashboardHeader"));
    m_dashboardFooter = static_cast<NuklearWindow*>(GetChildByName("O-DashboardFooter"));
    m_toolbar = static_cast<NuklearWindow*>(GetChildByName("O-DashboardToolbar"));
    m_toolbar->Enable(false);
}

void SceneDashboard::OnDelete()
{

}

orxBOOL SceneDashboard::OnCollide(
    ScrollObject *_poCollider,
    const orxSTRING _zPartName,
    const orxSTRING _zColliderPartName,
    const orxVECTOR &_rvPosition,
    const orxVECTOR &_rvNormal)
{
    return orxTRUE;
}

void SceneDashboard::Update(const orxCLOCK_INFO &_rstInfo)
{
    Scene::Update(_rstInfo);

    if (!m_bUnskippable)
    {
        float dashboardActiveTime = m_dashboard->GetActiveTime();

        // Grow the Dashboard's leaf bunches.
        if (dashboardActiveTime >= m_dashboard->m_animStartTimeLeafBunch && dashboardActiveTime <= m_dashboard->m_animEndTimeLeafBunch)
        {
            m_dashboard->GrowLeafBunches();
        }
        // Phase the Dashboard's moon.
        if (dashboardActiveTime >= m_dashboard->m_animStartTimeMoon && dashboardActiveTime <= m_dashboard->m_animEndTimeMoon)
        {
            m_dashboard->PhaseMoon();
        }
        // Scroll the Dashboard.
        if (dashboardActiveTime >= m_dashboard->m_animStartTimeScroll && dashboardActiveTime <= m_dashboard->m_animEndTimeScroll)
        {
            m_dashboard->Scroll();
        }
        // Finalize the Dashboard's leaf bunches as appropriate.
        if (dashboardActiveTime >= m_dashboard->m_animEndTimeLeafBunch)
        {
            m_dashboard->FinalizeLeafBunches();
        }
        // Finalize the Dashboard's moon as appropriate.
        if (dashboardActiveTime >= m_dashboard->m_animEndTimeMoon)
        {
            m_dashboard->FinalizeMoon();
        }
        // Finalize the Dashboard's scroll as appropriate.
        if (dashboardActiveTime >= m_dashboard->m_animEndTimeScroll)
        {
            m_dashboard->FinalizeScroll();
            m_toolbar->Enable(true);
            m_bUnskippable = true;
        }
        // Disable header and footer as appropriate.
        if (dashboardActiveTime >= m_dashboard->m_animStartTimeScroll)
        {
            DisableHeaderAndFooter();
        }
    }
}

void SceneDashboard::Skip()
{
    if (m_dashboard->GetActiveTime() < m_dashboard->m_animEndTimeScroll)
    {
        DisableHeaderAndFooter();
        m_dashboard->FinalizeLeafBunches();
        m_dashboard->FinalizeMoon();
        m_dashboard->FinalizeScroll();
        m_toolbar->Enable(true);
        m_bUnskippable = true;
    }
}

void SceneDashboard::DisableHeaderAndFooter()
{
    m_dashboardHeader->Enable(false);
    m_dashboardFooter->Enable(false);
}
