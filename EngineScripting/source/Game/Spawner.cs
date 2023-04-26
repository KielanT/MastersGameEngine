using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Engine;

namespace Game
{
    public class Spawner : Entity
    {
        
        public void OnBegin()
        {
            Entity entity = FindEntityByName("Cube");
            Utility.Logger(LogType.Debug, "OnBegin");
        }

        public void OnUpdate(float deltaTime)
        {
            Utility.Logger(LogType.Debug, stringfield);
        }

        public int intfield = 5;
        public int intfield2 = 6;
        public float floatfield = 7.0f;
        public string stringfield = "test";
    }
}
