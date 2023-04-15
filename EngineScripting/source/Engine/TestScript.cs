using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Engine;

namespace Game
{
    public class TestScript
    {
        public void OnBegin()
        {
            Utility.UT_LogInfo("OnBegin C# call to C++");
            Console.WriteLine("OnBegin from C# ");
        }

        public void OnUpdate(float deltaTime)
        {

        }
    }
}
