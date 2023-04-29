using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Engine;
using Engine.Maths;

namespace Game
{
    public class Spawner : Entity
    {
        
        public void OnBegin()
        {
            Entity entity = FindEntityByName("Cube");

            TransformComponent transformComponent = new TransformComponent();
            transformComponent.entity = entity;
            Utility.Logger(LogType.Debug, transformComponent.Position.X.ToString());
            Utility.Logger(LogType.Debug, "int " + intfield.ToString());

            Vector3 vector;
            vector.X = 0;
            vector.Y = 0;
            vector.Z = 0;
            transformComponent.Position = vector;

        }

        public void OnUpdate(float deltaTime)
        {
           // Utility.Logger(LogType.Debug, stringfield);
        }

        public int intfield = 5;
        public int intfield2 = 6;
        public float floatfield = 7.0f;
        public string stringfield = "test";
    }
}
