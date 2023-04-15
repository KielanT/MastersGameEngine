using System;
using System.Runtime.CompilerServices;

namespace Engine
{
    public static class InternalCalls
    {
        //************************
        //      Utilities
        //************************

        // Logs
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Utility_Log_Info(string message);

    }
}
