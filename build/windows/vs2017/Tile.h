#pragma once

#include "Payload.h"
#include "ScrollMod.h"
#include "TileSetShiftStatus.h"
#include "TileSetState.h"

namespace payload
{
    class Tile : public ScrollMod
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
        TileSetState m_priorTileSetState;
        orxVECTOR m_priorParentSpacePos;
        orxVECTOR m_targetParentSpacePos;
        orxVECTOR m_priorVisualScale;
        orxVECTOR m_targetVisualScale;
        bool m_shifting;
        bool m_bCartesian;
        // FOREGROUND DATA
        float m_topRadius;
        float m_bottomRadius;
        orxVECTOR m_leftEdgeTopPoint;
        orxVECTOR m_leftEdgeBottomPoint;
        orxVECTOR m_rightEdgeTopPoint;
        orxVECTOR m_rightEdgeBottomPoint;
        orxVECTOR m_visualCenter;
        orxVECTOR m_visualScale;
        // BACKGROUND DATA
        float m_topRadiusBG;
        float m_bottomRadiusBG;
        orxVECTOR m_leftEdgeTopPointBG;
        orxVECTOR m_leftEdgeBottomPointBG;
        orxVECTOR m_rightEdgeTopPointBG;
        orxVECTOR m_rightEdgeBottomPointBG;

        const orxVECTOR GetGridRelativeCartesianPosition(const int &_row, const int &_col, const float &_normalizedBorderSize, const float &_normalizedTileSize);
        const int GetQuadrant(
            const int &_row,
            const int &_col,
            const int &_square,
            const orxVECTOR &_payloadRowAndCol,
            const bool _background);
        const int GetUnitDistanceFromTileSetCenter(
            const int &_row,
            const int &_col,
            const int &_square,
            const orxVECTOR &_payloadRowAndCol,
            const bool _background,
            const TileSetState &_tileSetState);
        const int GetUnitDistanceFromTileSetPolarAxis(
            const int &_row,
            const int &_col,
            const int &_square,
            const int &_unitDistanceFromOrigin,
            const orxVECTOR &_payloadRowAndCol,
            const bool &_tileSetIs2D);
        const int GetNumTilesInPolarRow(
            const int &_square,
            const int &_unitDistanceFromOrigin,
            const bool &_tileSetIs2D);
        const float GetDistanceFromTileSetCenter(
            const int &_square,
            const int &_unitDistanceFromOrigin,
            const float &_tileSetRadius,
            const orxVECTOR &_payloadRowAndCol,
            const bool &_tileSetIs2D,
            const bool &_background,
            const TileSetState &_tileSetState);
        const float GetPolarTheta(
            const int &_square,
            const int &_unitDistanceFromPolarAxis,
            const int &_tilesInPolarRow,
            const bool &_tileSetIs2D);
        void SetUp(
            const int &_row,
            const int &_col,
            const int &_square,
            const float &_tileSetRadius,
            const float &_normalizedTileSize,
            const float &_normalizedBorderSize,
            const orxVECTOR &_payloadRowAndCol,
            const orxVECTOR &_tileSetPos,
            const orxVECTOR &_tileSetSize,
            const TileSetState &_tileSetState);
        void Shift(
            const int &_row,
            const int &_col,
            const int &_square,
            const int &_greatest1DUnitDistanceOfPayloadRowFromThreshold,
            const float &_tileSetRadius,
            const float &_normalizedTileSize,
            const float &_normalizedBorderSize,
            const float &_lerpWeight,
            const orxVECTOR &_payloadRowAndCol,
            const orxVECTOR &_tileSetPos,
            const TileSetState &_tileSetState,
            const TileSetShiftStatus &_tileSetShiftStatus);
        const orxVECTOR CalculateVisualScale(
            const int &_square,
            const int &_tilesInRow,
            const float &_tileSetRadius,
            const float &_normalizedTileSize,
            const orxVECTOR &_tileSetPos,
            const TileSetState &_tileSetState);
    };
}
