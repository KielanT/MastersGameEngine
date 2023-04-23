
using System;


namespace Engine
{
    public class TransformComponent
    {
        public Maths.Vector3 Position
        {
            get
            {
                InternalCalls.Transform_GetPostition(out Maths.Vector3 pos);
                return pos;
            }
            set 
            {
                InternalCalls.Transform_SetPostition(value);
            }
        }
    }
}
