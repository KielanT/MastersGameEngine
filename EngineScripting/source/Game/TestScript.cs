using System;

using Engine;

namespace Game
{

    // Test script for logging to console
    public class TestScript : Entity
    {
        public void OnBegin()
        {
            Utility.Logger(LogType.Info, "OnBegin C# call to C++");
        }

        public void OnUpdate(float deltaTime)
        {
          //  Utility.Logger(LogType.Debug, "OnUpdate from script one");
           // Console.WriteLine("OnUpdate:  {0}", deltaTime);
        }
    }
}
