#include "SceneDashboard.h"

using namespace payload;

void SceneDashboard::OnCreate()
{
    m_dashboard = static_cast<Dashboard*>(GetChildByName("O-Dashboard"));
    m_dashboardHeader = static_cast<NuklearWindow*>(GetChildByName("O-DashboardHeader"));
    m_dashboardFooter = static_cast<NuklearWindow*>(GetChildByName("O-DashboardFooter"));
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

    float dashboardActiveTime = m_dashboard->GetActiveTime();
    if (dashboardActiveTime >= m_dashboard->m_animStartTimeScroll && dashboardActiveTime - _rstInfo.fDT <= m_dashboard->m_animStartTimeScroll)
    {
        DisableHeaderAndFooter();
    }
}

void SceneDashboard::Skip()
{
    DisableHeaderAndFooter();
    m_dashboard->FinalizeLeafBunches();
    m_dashboard->FinalizeMoon();
    m_dashboard->FinalizeScroll();
}

void SceneDashboard::DisableHeaderAndFooter()
{
    m_dashboardHeader->Enable(false);
    m_dashboardFooter->Enable(false);
}
