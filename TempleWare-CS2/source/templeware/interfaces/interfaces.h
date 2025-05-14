// interfaces.h
#pragma once

#include "IEngineClient/IEngineClient.h"
#include "IEngineClient/ISource2EngineToClient.h"   // 
#include "IEngineClient/IEngineTrace.h"            // 
#include "CGameEntitySystem/CGameEntitySystem.h"
#include "..\..\cs2\entity\C_CSPlayerPawn\C_CSPlayerPawn.h"
#include "..\..\cs2\datatypes\cutlbuffer\cutlbuffer.h"
#include "..\..\cs2\datatypes\keyvalues\keyvalues.h"
#include "..\..\cs2\entity\C_Material\C_Material.h"

namespace I
{
    // UtlBuffer
    inline void(__fastcall* EnsureCapacityBuffer)(CUtlBuffer*, int) = nullptr;
    inline CUtlBuffer* (__fastcall* ConstructUtlBuffer)(CUtlBuffer*, int, int, int) = nullptr;
    inline void(__fastcall* PutUtlString)(CUtlBuffer*, const char*) = nullptr;
    inline std::int64_t(__fastcall* CreateMaterial)(void*, void*, const char*, void*, unsigned int, unsigned int) = nullptr;
    inline bool(__fastcall* LoadKeyValues)(CKeyValues3*, void*, const char*, const KV3ID_t*, const char*) = nullptr;
    inline void(__fastcall* ConMsg)(const char*, ...) = nullptr;
    inline void(__fastcall* ConColorMsg)(const Color&, const char*, ...) = nullptr;
    inline void(__fastcall* TraceShape) (/*TODO: fill in args*/) = nullptr;


    
    inline IEngineClient* EngineClient = nullptr;
    inline IGameResourceService* GameEntity = nullptr;
    inline ISource2EngineToClient* EngineToClient = nullptr; 
    inline IEngineTrace* EngineTrace = nullptr; 

    class Interfaces {
    public:
        bool init();
    };
}