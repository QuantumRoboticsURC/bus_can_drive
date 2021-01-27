#pragma once

#include "ctre/phoenix/cci/CCI.h"
#include "ctre/phoenix/platform/Platform.h"
#include <stdint.h>

using namespace ctre::phoenix::platform;

extern "C" {
    CCIEXPORT int32_t c_SimCreate(DeviceType type, int id);
    CCIEXPORT int32_t c_SimDestroy(DeviceType type, int id);
    CCIEXPORT int32_t c_SimDestroyAll();
    CCIEXPORT int32_t c_SimSetPhysicsInput(DeviceType type, int id, std::string const &physicsType, double value);
    CCIEXPORT int32_t c_SimGetPhysicsValue(DeviceType type, int id, std::string const &physicsType, double &value);
    CCIEXPORT int32_t c_SimGetLastError(DeviceType type, int id);
}
