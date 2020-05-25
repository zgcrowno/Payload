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
}

void SceneDashboard::Skip()
{
    
}
