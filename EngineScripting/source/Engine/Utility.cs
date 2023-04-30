using System;


namespace Engine
{

    public enum LogType
    {
        Debug = 0,
        Error = 1,
        Info = 2,
        Warn = 3,
    }

    public class Utility
    {

        public static void Logger(LogType type, string message)
        {
            if (type == LogType.Debug)
            {
                InternalCalls.Utility_Log_Debug(message);
            }
            else if (type == LogType.Error)
            {
                InternalCalls.Utility_Log_Error(message);
            }
            else if (type == LogType.Info)
            {
                InternalCalls.Utility_Log_Info(message);
            }
            else if (type == LogType.Warn)
            {
                InternalCalls.Utility_Log_Warn(message);
            }
        }
    }
}
