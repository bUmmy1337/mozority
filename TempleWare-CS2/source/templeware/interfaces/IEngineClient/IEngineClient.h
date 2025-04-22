// File: IEngineClient.h
#pragma once

#include "..\..\utils\memory\vfunc\vfunc.h"
#include <type_traits>
#include "../../utils/math/vector/vector.h" // qangles

class IEngineClient
{
public:
    int maxClients()
    {
        return M::vfunc<int, 34U>(this);
    }

    bool in_game()
    {
        return M::vfunc<bool, 35U>(this);
    }

    bool connected()
    {
        return M::vfunc<bool, 36U>(this);
    }

    int get_local_player()
    {
        int nIndex = -1;
        M::vfunc<void, 49U>(this, std::ref(nIndex), 0);
        return nIndex + 1;
    }

    //
    // viewóãëû
    void GetViewAngles(QAngle_t& angles)
    {
        M::vfunc<void, 18U>(this, std::ref(angles));
    }

    // viewóãëû
    void SetViewAngles(const QAngle_t& angles)
    {
        M::vfunc<void, 19U>(this, angles);
    }
    // END NEW

public:
    inline bool valid()
    {
        return connected() && in_game();
    }
};