using System;


using Engine;

namespace Game
{
    public class TestScript
    {
        public void OnBegin()
        {
            Utility.Logger(LogType.Info, "OnBegin C# call to C++");
            Utility.Logger(LogType.Warn, "OnBegin C# call to C++");
            Utility.Logger(LogType.Debug, "OnBegin C# call to C++");
            Utility.Logger(LogType.Error, "OnBegin C# call to C++");
            Console.WriteLine("OnBegin from C# ");
        }

        public void OnUpdate(float deltaTime)
        {
            Utility.Logger(LogType.Debug, "OnUpdate from script one");
            Console.WriteLine("OnUpdate:  {0}", deltaTime);
        }
    }
}
