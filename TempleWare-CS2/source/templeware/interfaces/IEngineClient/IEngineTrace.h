#pragma once

#include "../../utils/memory/vfunc/vfunc.h"
#include "../../utils/math/vector/vector.h"
#include "../../../cs2/entity/C_CSPlayerPawn/C_CSPlayerPawn.h"

// Simplified trace filter
struct TraceFilter {
    C_CSPlayerPawn* skipEntity; // Entity to ignore (e.g., local player)
};

// Simplified trace result
struct TraceResult {
    bool hit;                  // Did the ray hit something?
    C_CSPlayerPawn* entity;    // Entity hit (if any)
    Vector_t endpos;           // Final position of the ray
    float fraction;            // Fraction of the ray completed (0.0 to 1.0)
};

class IEngineTrace {
public:
    // Trace a ray from start to end, checking for collisions
    void TraceRay(const Vector_t& start, const Vector_t& end, unsigned int mask, TraceFilter* filter, TraceResult* result) {
        M::vfunc<void, 5U>(this, std::cref(start), std::cref(end), mask, filter, result);
    }
};