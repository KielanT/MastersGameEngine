
using System;


namespace Engine
{
    public abstract class Component
    {
        public Entity entity { get; set; }
    }

    public class TransformComponent : Component
    {
        public Maths.Vector3 Position
        {
            get
            {
                InternalCalls.Transform_GetPostition(entity.ID, out Maths.Vector3 pos);
                return pos;
            }
            set 
            {
                InternalCalls.Transform_SetPostition(entity.ID, ref value);
            }
        }
    }
}
