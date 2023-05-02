using Engine.Maths;
using System;


namespace Engine
{
    public class Entity
    {
        public readonly ulong ID;

        // TODO: Only add components if the entity has them
        public TransformComponent transformComponent;
        public RendererComponent rendererComponent;
        public PhysicsComponent physicsComponent;

        // TODO: set this automatically to attached entity
        protected Entity() 
        { 
            ID = 0;
            transformComponent = new TransformComponent(this);
            rendererComponent = new RendererComponent(this);
            physicsComponent = new PhysicsComponent(this);
        }
       
        public Entity(ulong id) 
        { 
            ID = id;
            transformComponent = new TransformComponent(this);
            rendererComponent = new RendererComponent(this);
            physicsComponent = new PhysicsComponent(this);
        }

        public Entity FindEntityByName(string name)
        {
            ulong id = InternalCalls.Entity_FindByName(name);
            if(id == 0)
                return null;

            return new Entity(id);
        }

        public Entity CreateNewEntity(Entity entity, Vector3 pos)
        {
            ulong id = InternalCalls.Entity_CreateEntityByCopy(entity.ID, ref pos);
            if (id == 0)
                return null;

            return new Entity(id);
        }

        public virtual void OnContact(ulong id)
        {
            Utility.Logger(LogType.Info, "OnContact Parent");
        }
    }
}
