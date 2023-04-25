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
            Utility.Logger(LogType.Debug, floatfield.ToString());
        }

        public int field;
        public float floatfield = 5.0f;
        
        public int Id { get { return field; } set { field = value; } }
    }
}
