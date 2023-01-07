#pragma once
#include <foundation/PxErrorCallback.h>
#include <foundation/PxErrors.h>


class PhysXCustomErrorCallback : public physx::PxErrorCallback
{
public:
    virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override;
};

