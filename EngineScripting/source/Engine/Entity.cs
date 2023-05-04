using Engine.Maths;
using System;

// All entities must inherit this class
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
            // Creates all components
            // Would be better to only create them if the entity actually has them
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

        // Find entity by name
        public Entity FindEntityByName(string name)
        {
            ulong id = InternalCalls.Entity_FindByName(name);
            if(id == 0)
                return null;

            return new Entity(id);
        }

        // Creates a new entity by copy
        public Entity CreateNewEntity(Entity entity, Vector3 pos)
        {
            ulong id = InternalCalls.Entity_CreateEntityByCopy(entity.ID, ref pos);
            if (id == 0)
                return null;

            return new Entity(id);
        }

        // virtual function to make sure the child has the correct parameters 
        public virtual void OnContact(ulong id)
        {
            Utility.Logger(LogType.Info, "OnContact Parent");
        }
    }
}
