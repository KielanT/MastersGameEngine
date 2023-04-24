using System;


namespace Engine
{
    public class Entity
    {
        // TODO: set this automatically to attached entity
        protected Entity() { ID = 0; }

        public Entity(ulong id) 
        { 
            ID = id;
        }

        public readonly ulong ID;

        public Entity FindEntityByName(string name)
        {
            ulong id = InternalCalls.Entity_FindByName(name);
            if(id == 0)
                return null;

            return new Entity(id);
        }
    }
}
