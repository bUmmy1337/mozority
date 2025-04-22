// File: IEngineClient/ISource2EngineToClient.h
#pragma once
#include "IEngineTrace.h"

// Заглушка: наследуем от IEngineTrace, чтобы vtable совпадал и можно было сразу
// приводить к IEngineTrace*
class ISource2EngineToClient : public IEngineTrace
{
public:
    // здесь можно добавить специфичные для Source2EngineToClient методы,
    // если они вам нужны
};