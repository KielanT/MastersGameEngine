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
            
        }

        public void OnUpdate(float deltaTime)
        {

        }
    }
}
