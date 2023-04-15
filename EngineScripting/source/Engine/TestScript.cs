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
            Utility.UT_LogInfo("OnBegin");
            Console.WriteLine("OnBegin but console");
        }

        public void OnUpdate(float deltaTime)
        {

        }
    }
}
