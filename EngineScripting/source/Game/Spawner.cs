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
        public int spawnAmount = 5;
        public string entitySpawnName = "Cube";

        // TODO: hide private fields in editor
        private float timer = 0;
        private float delay = 2;
        private int spawnCount = 0;
        private Entity entity;

        public void OnBegin()
        {
            entity = FindEntityByName(entitySpawnName);
           
        }

        public void OnUpdate(float deltaTime)
        {
            timer += deltaTime;

            if (timer > delay)
            {
                SpawnNewEntity();
                timer = 0;
            }
        }

        private void SpawnNewEntity()
        {
            if (spawnCount < spawnAmount)
            {
                Entity entity1 = CreateNewEntity(entity);
                TransformComponent transformComponent = new TransformComponent();
                transformComponent.entity = entity1;
                transformComponent.Position = new Vector3(0, 0, 0);

                spawnCount++;
            }
        }
        
    }
}
