
using System;


namespace Engine
{
    public abstract class Component
    {
        public Entity _entity { get; set; }
    }

    public class TransformComponent : Component
    {
        public TransformComponent(Entity entity)
        {
            _entity = entity;
        }


        public Maths.Vector3 Position
        {
            get
            {
                InternalCalls.Transform_GetPostition(_entity.ID, out Maths.Vector3 pos);
                return pos;
            }
            set 
            {
                InternalCalls.Transform_SetPostition(_entity.ID, ref value);
            }
        }

        public Maths.Vector3 Rotation 
        {
            get 
            {
                InternalCalls.Transform_GetRotation(_entity.ID, out Maths.Vector3 rot);
                return rot;
            }
            set
            {
                InternalCalls.Transform_SetRotation(_entity.ID, ref value);
            }
        }

        public Maths.Vector3 Scale 
        {
            get
            {
                InternalCalls.Transform_GetScale(_entity.ID, out Maths.Vector3 scale);
                return scale;
            }
            set 
            {
                InternalCalls.Transform_SetScale(_entity.ID, ref value);
            }
        }
    }
}
