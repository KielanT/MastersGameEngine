﻿using System;
using System.Runtime.CompilerServices;

namespace Engine
{
    public static class InternalCalls
    {
        //************************
        //      Utilities
        //************************

        // Logs 
        // TODO: Be best to have the enum check done in c++ rather than c#
        // because this file is going to become huge
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Utility_Log_Debug(string message);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Utility_Log_Error(string message);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Utility_Log_Info(string message);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Utility_Log_Warn(string message);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static ulong Entity_FindByName(string name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static ulong Entity_CreateEntityByCopy(ulong id);

        //************************
        //      Transform
        //************************

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Transform_SetPostition(ulong ID, ref Maths.Vector3 vector3);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Transform_GetPostition(ulong ID, out Maths.Vector3 vector3);
    }
}
