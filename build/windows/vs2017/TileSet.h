#pragma once

#include "Payload.h"
#include "Goal.h"
#include "MemorySetCartesian1D.h"
#include "MemorySetCartesian2D.h"
#include "MemorySetPolar1D.h"
#include "MemorySetPolar2D.h"
#include "PlayerPayload.h"
#include "ScrollMod.h"
#include "Tile.h"
#include "TileSetShiftStatus.h"
#include "TileSetState.h"
#include <vector>

#define NATIVE_TEXTURE_SIZE 1024.0f
#define NATIVE_BORDER_SIZE 5.0f
#define NORMALIZED_BORDER_SIZE (NATIVE_BORDER_SIZE / NATIVE_TEXTURE_SIZE)

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
        int m_square;
        int m_halfSquare;
        float m_normalizedTileSize;
        float m_width;
        float m_height;
        float m_radius;
        float m_timeToShift;
        float m_timeSpentShifting;
        float m_timeToReconfigure;
        float m_timeSpentReconfiguring;
        TileSetState m_state;
        TileSetState m_priorState;
        TileSetShiftStatus m_shiftStatus;
        MemorySet *m_memorySetToReconfigure;
        PlayerPayload *m_payload;
        Goal *m_goal;
        std::vector<MemorySetCartesian1D*> m_memorySetsCartesian1D;
        std::vector<MemorySetCartesian2D*> m_memorySetsCartesian2D;
        std::vector<MemorySetPolar1D*> m_memorySetsPolar1D;
        std::vector<MemorySetPolar2D*> m_memorySetsPolar2D;
        std::vector<std::vector<Tile*>> m_tileRows;

        void SetMemorySetToReconfigure(MemorySet *_memSet);
        void Shift(TileSetShiftStatus _shiftStatus);
        void ShiftTiles();
        void Reconfigure();
        void ReconfigureTiles();
        void FinalizeTileAndInhabitantLerps();
        const bool Is2D();
        const bool PriorStateIs2D();
        const bool IsCartesian();
        const bool InputAllowed();
        const bool NeedToShiftD1Tiles();
        const int GetUnitDistanceFromOrigin(
            const int &_row,
            const int &_col,
            const int &_payloadRow);
        const int GetUnitDistanceFromPolarAxis(const int &_col);
        const int GetGreatest1DUnitDistanceOfPayloadRowFromThreshold();
        const float GetPolarTheta(const int &_unitDistanceFromPolarAxis, const int &_tilesInPolarRow, const bool _d2);
        const orxVECTOR GetNormalizedPosition(const orxVECTOR &_vec);
        Tile *GetTileToRight(const Tile *_tile, const int &_payloadRow);
        Tile *GetTileToLeft(const Tile *_tile, const int &_payloadRow);
        Tile *GetTileAbove(const Tile *_tile, const int &_payloadRow);
        Tile *GetTileBelow(const Tile *_tile, const int &_payloadRow);
    };
}
