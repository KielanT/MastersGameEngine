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
            // Sets the entity to be spawned
            entity = FindEntityByName(entitySpawnName);

        }

        public void OnUpdate(float deltaTime)
        {
            timer += deltaTime;

            // Spawns a entity every 2 seconds
            if (timer > delay)
            {
                SpawnNewEntity();
                timer = 0;
            }
        }

        private void SpawnNewEntity()
        {
            // Spawn entities with a position, scale, visibility and force
            if (spawnCount < spawnAmount)
            {
                Entity entity1 = CreateNewEntity(entity, new Vector3(0, -1.8f, 0));
                entity1.transformComponent.Scale = new Vector3(0.5f, 0.5f, 0.5f);
                entity1.rendererComponent.Visible = true;
                entity1.physicsComponent.AddForce(new Vector3(40, 40, 0), ForceType.eFORCE);
                spawnCount++;
            }
        }
        
    }
}
