#include "epch.h"
#include "PhysXCustomCallbacks.h"


void PhysXCustomErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
{
	// Log the error to the console
	LOG_ERROR("<PhysX Error Code {}>  <Message: {}> <File: {}> <Line: >", code, message, file, line);
}
