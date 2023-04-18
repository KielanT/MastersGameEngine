
using System;

using Engine;

namespace Game
{
    public class TestScriptTwo
    {
        public void OnBegin()
        {
            Utility.Logger(LogType.Info, "OnBegin C# call to C++");

            Console.WriteLine("OnBegin from C# ");
        }

        public void OnUpdate(float deltaTime)
        {
            Utility.Logger(LogType.Error, "OnUpdate from script two");
            Console.WriteLine("OnUpdate:  {0}", deltaTime);
        }
    }
}