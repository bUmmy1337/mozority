// interfaces.cpp
#include "interfaces.h"
#include "CGameEntitySystem/CGameEntitySystem.h"
#include "..\..\templeware\utils\memory\Interface\Interface.h"

bool I::Interfaces::init()
{
    const HMODULE tier0_base = GetModuleHandleA("tier0.dll");
    if (!tier0_base)
        return false;

    bool success = true;

    // 1 EngineClient
    EngineClient = I::Get<IEngineClient>(
        "engine2.dll",
        "Source2EngineToClient00"
    );
    success &= (EngineClient != nullptr);

    // 2 GameResourceService
    GameEntity = I::Get<IGameResourceService>(
        "engine2.dll",
        "GameResourceServiceClientV00"
    );
    success &= (GameEntity != nullptr);

    // 3 Variant A: Source2EngineToClient - IEngineTrace
    EngineToClient = I::Get<ISource2EngineToClient>(
        "engine2.dll",
        "Source2EngineToClient001"
    );
    success &= (EngineToClient != nullptr);

    // Приводим к IEngineTrace
    EngineTrace = reinterpret_cast<IEngineTrace*>(EngineToClient);
    success &= (EngineTrace != nullptr);

    // 4 tier0.dll
    ConstructUtlBuffer = reinterpret_cast<decltype(ConstructUtlBuffer)>(
        GetProcAddress(tier0_base, "??0CUtlBuffer@@QEAA@HHH@Z")
        );
    EnsureCapacityBuffer = reinterpret_cast<decltype(EnsureCapacityBuffer)>(
        GetProcAddress(tier0_base, "?EnsureCapacity@CUtlBuffer@@QEAAXH@Z")
        );
    PutUtlString = reinterpret_cast<decltype(PutUtlString)>(
        GetProcAddress(tier0_base, "?PutString@CUtlBuffer@@QEAAXPEBD@Z")
        );
    CreateMaterial = reinterpret_cast<decltype(CreateMaterial)>(
        M::FindPattern("materialsystem2.dll", "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 81 EC ? ? ? ? 48 8B 05")
        );
    LoadKeyValues = reinterpret_cast<decltype(LoadKeyValues)>(
        GetProcAddress(tier0_base, "?LoadKV3@@YA_NPEAVKeyValues3@@PEAVCUtlString@@PEBDAEBUKV3ID_t@@2@Z")
        );
    ConMsg = reinterpret_cast<decltype(ConMsg)>(
        GetProcAddress(tier0_base, "?ConMsg@@YAXPEBDZZ")
        );
    ConColorMsg = reinterpret_cast<decltype(ConColorMsg)>(
        GetProcAddress(tier0_base, "?ConColorMsg@@YAXAEBVColor@@PEBDZZ")
        );

    // log addresses
    printf("Source2EngineToClient001: 0x%p\n", reinterpret_cast<void*>(EngineToClient));
    printf("IEngineTrace (from EngineToClient): 0x%p\n", reinterpret_cast<void*>(EngineTrace));
    printf("Source2EngineToClient00: 0x%p\n", reinterpret_cast<void*>(EngineClient));
    printf("GameResourceServiceClientV00: 0x%p\n", reinterpret_cast<void*>(GameEntity));
    printf("CreateMaterial: 0x%p\n", reinterpret_cast<void*>(CreateMaterial));

    return success;
}