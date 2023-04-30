
using Engine.Maths;
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

    public class RendererComponent : Component
    {
        public RendererComponent(Entity entity)
        {
            _entity = entity;
        }

        public bool Visible 
        {
            get
            {
                InternalCalls.Renderer_GetVisible(_entity.ID, out bool vis);
                return vis;
            }
            set 
            {
                InternalCalls.Renderer_SetVisible(_entity.ID, ref value);
            }
        }
    }

    public class PhysicsComponent : Component
    {
        public PhysicsComponent(Entity entity)
        {
            _entity = entity;
        }

        public void AddForce(Vector3 force)
        {
            InternalCalls.Physics_AddForce(_entity.ID, force);
        }
    }
}
