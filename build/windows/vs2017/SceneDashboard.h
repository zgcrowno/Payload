#pragma once

#include "Payload.h"
#include "Scene.h"
#include "Dashboard.h"
#include "NuklearWindow.h"

namespace payload
{
    // Using virtual inheritance for ScrollMod to prevent member ambiguity errors when also inheriting from InputHandler.
    class SceneDashboard : public Scene
    {
    protected:
        //! Called on object creation
        virtual void OnCreate();
        //! Called on object deletion
        virtual void OnDelete();
        //! Called on object collision
        virtual orxBOOL OnCollide(
            ScrollObject *_poCollider,
            const orxSTRING _zPartName,
            const orxSTRING _zColliderPartName,
            const orxVECTOR &_rvPosition,
            const orxVECTOR &_rvNormal);
        //! Called on clock update
        virtual void Update(const orxCLOCK_INFO &_rstInfo);
    public:
        Dashboard *m_dashboard;
        NuklearWindow *m_dashboardHeader;
        NuklearWindow *m_dashboardFooter;

        virtual void Skip();
    };
}
