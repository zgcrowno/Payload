#pragma once

#include "Payload.h"
#include "Goal.h"
#include "PlayerPayload.h"
#include "ScrollMod.h"
#include "Tile.h"
#include "TileSetState.h"
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
        //! Called on shader event
        virtual orxBOOL OnShader(orxSHADER_EVENT_PAYLOAD &_rstPayload);
        //! Called on clock update
        virtual void Update(const orxCLOCK_INFO &_rstInfo);
    private:
        enum TileSetShiftStatus
        {
            None,
            D1,
            D2,
            Cartesian,
            Polar
        };

        int m_square;
        int m_halfSquare;
        float m_width;
        float m_height;
        float m_radius;
        float m_timeToShift;
        float m_timeSpentShifting;
        TileSetState m_state;
        TileSetState m_priorState;
        TileSetShiftStatus m_shiftStatus;
        // BEGIN TODO: Probably don't need m_payloadOrigin and m_goalOrigin instance data since they'll likely only be referenced in OnCreate().
        orxVECTOR m_payloadOrigin;
        orxVECTOR m_goalOrigin;
        // END TODO
        PlayerPayload *m_payload;
        Goal *m_goal;
        std::vector<std::vector<Tile*>> m_tileRows;

        void Shift(TileSetShiftStatus _shiftStatus);
        const bool Is2D();
        const bool IsCartesian();
        // BEGIN TODO: Get rid of these methods if I never use them.
        const int GetQuadrant(const int &_row, const int &_col, const orxVECTOR &_payloadRowAndCol, const bool _background);
        const int GetUnitDistanceFromOrigin(const int &_row, const int &_col, const orxVECTOR &_payloadRowAndCol, const bool _background);
        const int GetUnitDistanceFromPolarAxis(const int &_row, const int &_col, const orxVECTOR &_payloadRowAndCol, const int &_unitDistanceFromOrigin);
        const int GetNumTilesInPolarRow(const int &_unitDistanceFromOrigin);
        const float GetRadialDistance(const int &_unitDistanceFromOrigin, const orxVECTOR &_payloadRowAndCol, const bool _background);
        const float GetPolarTheta(const int &_unitDistanceFromPolarAxis, const int &_tilesInPolarRow);
        const orxVECTOR GetCartesianUnitDistancesFromOrigin(const int &_row, const int &_col, const orxVECTOR &_payloadRowAndCol, const bool _background);
        // END TODO.
        const orxVECTOR GetPayloadRowAndColumn();
        const orxVECTOR GetPayloadNormalizedPosition();
        Tile *GetTileToRight(const int &_row, const int &_col, const orxVECTOR &_payloadRowAndCol);
        Tile *GetTileToLeft(const int &_row, const int &_col, const orxVECTOR &_payloadRowAndCol);
        Tile *GetTileAbove(const int &_row, const int &_col, const orxVECTOR &_payloadRowAndCol);
        Tile *GetTileBelow(const int &_row, const int &_col, const orxVECTOR &_payloadRowAndCol);
    };
}
