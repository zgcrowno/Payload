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
        bool m_bCartesian;
        int m_row;
        int m_col;
        float m_priorMemSetTheta;
        TileSetState m_priorTileSetState;
        orxVECTOR m_priorParentSpacePos;
        orxVECTOR m_targetParentSpacePos;
        orxVECTOR m_priorVisualScale;
        orxVECTOR m_targetVisualScale;
        orxVECTOR m_visualScale;

        static const orxVECTOR GetGridRelativeCartesianPosition(
            const int &_row,
            const int &_col,
            const float &_normalizedBorderSize,
            const float &_normalizedTileSize);
        static const int GetUnitDistanceFromTileSetCenter(
            const int &_row,
            const int &_col,
            const int &_square,
            const int &_payloadRow,
            const TileSetState &_tileSetState);

        const int GetNumTilesInPolarRow(
            const int &_square,
            const int &_unitDistanceFromOrigin,
            const bool &_tileSetIs2D);
        void SetUp(
            const int &_row,
            const int &_col,
            const int &_square,
            const float &_tileSetRadius,
            const float &_normalizedTileSize,
            const float &_normalizedBorderSize,
            const int &_payloadRow,
            const orxVECTOR &_tileSetPos,
            const TileSetState &_tileSetState);
        void Shift(
            const int &_square,
            const int &_greatest1DUnitDistanceOfPayloadRowFromThreshold,
            const float &_tileSetRadius,
            const float &_normalizedTileSize,
            const float &_normalizedBorderSize,
            const float &_lerpWeight,
            const int &_payloadRow,
            const orxVECTOR &_tileSetPos,
            const TileSetState &_tileSetState,
            const TileSetShiftStatus &_tileSetShiftStatus);
        void Reconfigure(const float &_lerpWeight, const float &_normalizedBorderSize, const float &_normalizedTileSize, const orxVECTOR &_memorySetPos);
        const orxVECTOR CalculateVisualScale(
            const int &_square,
            const int &_tilesInRow,
            const float &_tileSetRadius,
            const float &_normalizedTileSize,
            const orxVECTOR &_tileSetPos,
            const TileSetState &_tileSetState);
    };
}
