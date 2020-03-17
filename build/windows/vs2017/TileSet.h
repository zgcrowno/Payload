#pragma once

#include "Payload.h"
#include "Goal.h"
#include "PlayerPayload.h"
#include "ScrollMod.h"
#include "Tile.h"
#include <vector>

namespace payload
{
    class TileSet : public ScrollMod
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
    private:
        enum ShiftStatus
        {
            None,
            D1,
            D2,
            Cartesian,
            Polar
        };

        bool m_2D;
        bool m_cartesian;
        int m_square;
        int m_halfSquare;
        float m_shiftLerpT;
        float m_width;
        float m_height;
        float m_radius;
        ShiftStatus m_shiftStatus;
        // TODO: Probably don't need m_payloadOrigin and m_goalOrigin instance data since they'll likely only be referenced in OnCreate().
        orxVECTOR m_payloadOrigin;
        orxVECTOR m_goalOrigin;
        PlayerPayload *m_payload;
        Goal *m_goal;
        std::vector<std::vector<Tile*>> m_tileRows;

        void Shift(ShiftStatus _shiftStatus);
        const bool Cartesian2D();
        const bool Cartesian1D();
        const bool Polar2D();
        const bool Polar1D();
        const int GetQuadrant(const int &_row, const int &_col);
        const int GetUnitDistanceFromOrigin(const int &_row, const int &_col);
        const int GetUnitDistanceFromPolarAxis(const int &_row, const int &_col, const int &_unitDistanceFromOrigin);
        const int GetNumTilesInPolarRow(const int &_unitDistanceFromOrigin);
        const float GetRadialDistance(const int &_unitDistanceFromOrigin);
        const float GetPolarTheta(const int &_unitDistanceFromPolarAxis, const int &_tilesInPolarRow);
        const orxVECTOR GetPayloadRowAndColumn();
    };
}
