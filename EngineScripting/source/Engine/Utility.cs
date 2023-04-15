using System;


namespace Engine
{
    public class Utility
    {
        public static void UT_LogInfo(string message)
        {
            InternalCalls.Utility_Log_Info(message);
            Console.WriteLine("LOG but console");
        }
    }
}
